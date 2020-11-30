cmake_minimum_required(VERSION 3.15)

set(PROJECT_NAME BlackJack)
project(${PROJECT_NAME})

set(Boost_USE_STATIC_LIBS ON)
ADD_DEFINITIONS(-DBoost_USE_STATIC_LIBS=ON)

set (BOOST_ROOT "C:/Users/xomaq/Desktop/boost_1_71_0/boost_1_71_0")
set (BOOST_LIBRARYDIR "C:/Users/xomaq/Desktop/boost_1_71_0/boost_1_71_0")

set (Client BlackJack_Client)
set (Server BlackJack_Server)

find_package(Boost 1.71.0 REQUIRED)

if (Boost_FOUND)
    INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})
    ADD_DEFINITIONS( "-DHAS_BOOST" )

    add_executable(${Server}
            src/Server/main.cpp

            src/Server/Controller/Dealer.cpp
            src/Server/Controller/IHandler.cpp

            src/Model/Actors/IPlayer.cpp
            src/Model/Cards/CardStack.cpp
            src/Server/Controller/Game_Server.cpp

            src/Client/View/OutputManager.cpp

            src/Server/Engine/BlackJack.cpp

            src/Client/TCP_Player_Client.cpp

            src/Server/Controller/Game_Room.cpp
            )

    add_executable(${Client}
            src/Client/main.cpp

            src/Client/TCP_Player_Client.cpp)

    target_include_directories(${Server} PRIVATE src src/Model src/Server/Engine)

    target_include_directories(${Client} PRIVATE src src/Model src/Client/View)

    target_link_libraries(${Server}
            ${Boost_LIBRARIES}
            )

    target_link_libraries(${Client}
            ${Boost_LIBRARIES}
            )
endif()
if (WIN32)
    target_link_libraries(${Server} wsock32 ws2_32)
    target_link_libraries(${Client} wsock32 ws2_32)
endif()

#add_executable(tests test/simple_tests.cpp test/gamelogic.cpp
#
#        src/Game_Server/Controller/Dealer.cpp
#        src/Game_Server/Controller/IHandler.cpp
#
#        src/Model/Actors/IPlayer.cpp
#        src/Model/Cards/CardStack.cpp
#        src/Model/Actors/Game_Server.cpp
#
#        src/Client/View/OutputManager.cpp
#
#        src/Game_Server/Engine/BlackJack.cpp
#        )

#add_subdirectory(
#        "contrib/gtest"
#        "googletest"
#)

add_subdirectory("contrib/json")

target_link_libraries(${Server} nlohmann_json)
target_link_libraries(${Client} nlohmann_json)

#target_link_libraries(tests PRIVATE gtest gtest_main gmock)
#target_include_directories(tests PRIVATE src src/Model src/Client/View src/Game_Server/Engine)

set_target_properties(${Server} ${Client} PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
        COMPILE_OPTIONS "-Wall;-Wextra;-Wpedantic;"
        )

set_target_properties(${Server} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/)
set_target_properties(${Client} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/)

install(TARGETS ${Server} RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT xomageimer@gmail.com)

set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)

include(CPack)
#include(CTest)
#
#enable_testing()
#add_test(test tests)