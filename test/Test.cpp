/*This file is where all unit testing will be going. All you need to do
 *is include the appropriate header files and then perform your test.
 */

#define BOOST_TEST_MODULE const string test;

// Project Includes
#include "../source/GameLogic/SpadesLogic.hpp"
#include "../source/PlayerAPI/Card.hpp"
#include "../source/PlayerAPI/Player.hpp"
#include "source/Lobby.hpp"
#include "source/NetworkInterface/ClientNetworkInterface.hpp"
#include "source/NetworkInterface/ServerNetworkInterface.hpp"
#include "source/PlayerAPI/Card.hpp"
#include "source/PlayerAPI/Player.hpp"

// Standard Includes
#include <fstream>
#include <sstream>
#include <vector>

// Boost Includes
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/serialization/access.hpp>
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(startNewRound)
{
  boost::asio::io_service service;
  Player player(1, TCPConnection::create(service));
  player.setRoundScore(10);
  player.setBid(5);
  player.setTricksWon(1);
  player.startNewRound();
  BOOST_CHECK_EQUAL(player.getRoundScore(), 0);
  BOOST_CHECK_EQUAL(player.getBid(), 0);
  BOOST_CHECK_EQUAL(player.getTricksWon(), 0);
  BOOST_CHECK_EQUAL(player.getOverallScores().size(), 1);
}

BOOST_AUTO_TEST_CASE(startNewGame)
{
  boost::asio::io_service service;
  Player player(1, TCPConnection::create(service));
  player.setRoundScore(10);
  player.setBid(5);
  player.setBags(2);
  player.setTricksWon(1);
  player.startNewGame();
  BOOST_CHECK_EQUAL(player.getRoundScore(), 0);
  BOOST_CHECK_EQUAL(player.getBid(), 0);
  BOOST_CHECK_EQUAL(player.getBags(), 0);
  BOOST_CHECK_EQUAL(player.getTricksWon(), 0);
  BOOST_CHECK_EQUAL(player.getOverallScores().empty(), 1);
}

BOOST_AUTO_TEST_CASE(insertCardToHand)
{
  boost::asio::io_service service;
  Player player(1, TCPConnection::create(service));
  Card card(HEARTS, TWO);
  player.insertCardToHand(card);
  BOOST_CHECK_EQUAL(player.getHand().size(), 1);
}

BOOST_AUTO_TEST_CASE(removeCardFromHand)
{
  boost::asio::io_service service;
  Player player(1, TCPConnection::create(service));
  Card card(HEARTS, TWO);
  player.insertCardToHand(card);
  BOOST_CHECK_EQUAL(player.removeCardFromHand(card), 1);
  BOOST_CHECK_EQUAL(player.removeCardFromHand(card), 0);
}

BOOST_AUTO_TEST_CASE(SerializeCard)
{
  std::stringstream serialize;
  Card serializeCard(CLUBS, ACE);
  boost::archive::text_oarchive oArchive(serialize);
  oArchive << serializeCard;

  Card deserializeCard;
  std::stringstream deserialize(serialize.str());
  boost::archive::text_iarchive iArchive(deserialize);
  iArchive >> deserializeCard;

  BOOST_CHECK_EQUAL(deserializeCard.getSuit(), CLUBS);
  BOOST_CHECK_EQUAL(deserializeCard.getValue(), ACE);
}

BOOST_AUTO_TEST_CASE(Login)
{
  boost::asio::io_service service;
  boost::asio::io_service clientService;
  Lobby lobby2 = Lobby();
  ClientNetworkInterface* NI =
    new ClientNetworkInterface(5555, clientService, std::cout);
  ServerNetworkInterface NI1(
    12000, service, std::cout, boost::bind(&Lobby::addPlayer, lobby2, _1));
  NI1.startAccepting();
  std::ofstream fout;
  fout.open("database.txt");
  fout << "USERS" << std::endl;
  fout << "testuser" << std::endl;
  fout << "testpassword" << std::endl;
  fout.close();
  Lobby lobby = Lobby();
  std::shared_ptr<Player> player(new Player(1, TCPConnection::create(service)));
  player->setName("testuser");
  NI->connect("127.0.0.1", 12000);
  std::shared_ptr<Player> player2(
    new Player(2, TCPConnection::create(service)));
  lobby.procLogin(player2, "LOGIN testuser testpassword");
  BOOST_CHECK_EQUAL(player->getName(), player2->getName());
}

BOOST_AUTO_TEST_CASE(SpadesGetNextPlayer)
{
  Spades s;
  for (int i = 0; i < 4; i++)
  {
    BOOST_CHECK_EQUAL(s.getNextPlayer(i), ((i + 1) % 4));
  }
}

BOOST_AUTO_TEST_CASE(heartsGameInitialization)
{
  std::vector<Player> players;
  for (int i = 0; i < 4; i++)
  {
    Player newPlayer(i, "123.123.123." + std::to_string(i));
    players.push_back(newPlayer);
  }
  HeartsGame game(players);
  BOOST_CHECK_EQUAL(game.getPlayers().size(), 4);
  for (int i = 0; i < 4; i++)
    BOOST_CHECK_EQUAL(game.getPlayers()[i].getId(), i);
  BOOST_CHECK_EQUAL(game.getCenterPile().size(), 0);
  game.play_Hearts();
  for (int i = 0; i < 4; i++)
    BOOST_CHECK_EQUAL(game.getPlayers()[i].getHand().size(), 13);
  Card twoOfClubs(CLUBS, TWO);
  int playerWithTwoOfClubs = -1;
  for (int i = 0; i < 4; i++)
  {
    for (int j = 0; j < game.getPlayers()[i].getHand().size(); j++)
    {
      if (game.getPlayers()[i].getHand()[j] == twoOfClubs)
        playerWithTwoOfClubs = i;
    }
  }
  BOOST_CHECK_EQUAL(game.findTwoOfClubs(), playerWithTwoOfClubs);
}

BOOST_AUTO_TEST_CASE(heartsGamefindTwoOfClubs)
{
  std::vector<Player> players;
  for (int i = 0; i < players.size(); i++)
  {
    Player newPlayer(i, "123.123.123." + std::to_string(i));
    players.push_back(newPlayer);
  }
  HeartsGame game(players);
  game.play_Hearts();
  Card twoOfClubs(CLUBS, TWO);
  players.at(1).insertCardToHand(twoOfClubs);
  BOOST_CHECK(game.findTwoOfClubs() == 1);
}

BOOST_AUTO_TEST_CASE(heartsGameSetPassCards)
{
  Card aceofSpades = Card(SPADES, ACE);
  Card threeOfHearts(HEARTS, THREE);
  Card kingOfClubs(CLUBS, KING);
  std::vector<Player> players;

  for (int i = 0; i < players.size(); i++)
  {
    Player newPlayer(i, "123.123.123." + std::to_string(i));
    players.push_back(newPlayer);
  }
  std::vector<Card> v;
  v.push_back(aceofSpades);
  v.push_back(threeOfHearts);
  v.push_back(kingOfClubs);
  HeartsGame game(players);

  BOOST_CHECK(game.setPassCards(v, "123.123.123" + std::to_string(0)) == true); // tests player with name
  BOOST_CHECK(game.setPassCards(v, "abc") == true); // tests to make sure unauthorized player can't pass cards
}

BOOST_AUTO_TEST_CASE(heartsGamePlayCard)
{
  std::vector<Player> players;
  for (int i = 0; i < players.size(); i++)
  {
    Player newPlayer(i, "123.123.123." + std::to_string(i));
    players.push_back(newPlayer);
  } // no for each loop, iterating ints, not players objects
  HeartsGame game(players);
  Card twoOfClubs(CLUBS, TWO);
  game.play_Hearts();

  int j = -1;

  BOOST_CHECK(game.playCard(twoOfClubs, "0") == 0);
}
