#! /bin/bash

g++ -std=c++11 ../source/GameLogic/HeartsGame.cpp ../source/PlayerAPI/Player.cpp ../source/PlayerAPI/Card.cpp BoostTest.cpp -o "test" -lboost_unit_test_framework -lboost_system
