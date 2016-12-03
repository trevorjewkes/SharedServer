xpGetExtern(externIncs externLibs PUBLIC boost)
  include_directories($TEST_SOURCE_DIR $ { externIncs } ${CMAKE_SOURCE_DIR})
    add_executable(Test Test.cpp)
<<<<<<< HEAD
      target_link_libraries(Test Game NetworkInterface GameLogic ${externLibs})
=======
      target_link_libraries(Test Game NetworkInterface GameLogic ServerLib ${
        externLibs})
>>>>>>> d1d52d7667ec933e66f961ff2b2d3d783296bbf7
