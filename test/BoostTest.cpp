#define BOOST_TEST_MODULE const string test;
#define BOOST_TEST_DYN_LINK
// Boost Includes
#define BOOST_TEST_MODULE const string test;

// Boost Includes
#include <boost/test/unit_test.hpp>

#include "../source/PlayerAPI/Card.hpp"
#include "../source/PlayerAPI/Player.hpp"

// Standard Includes
#include <sstream>


//THIS TEST FAILS MISERABLY
//int playCard(Card card, std::string name);
BOOST_AUTO_TEST_CASE(heartsGamePlayCard){
    std::vector<Player> players;
    for (int i = 0; i< players.size(); i++){
        Player newPlayer(i,"123.123.123." + std::to_string(i));
        players.push_back(newPlayer);
    }//no for each loop, iterating ints, not players objects
    HeartsGame game(players);
    Card twoOfClubs(CLUBS, TWO);
    game.play_Hearts();

    BOOST_CHECK( std::string::compare(game.playCard(twoOfClubs, 0), "0") );
}


//EOF
