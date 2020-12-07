cmake_minimum_required(VERSION 3.15)

set(PROJECT_NAME BlackJack)
project(${PROJECT_NAME})

set(Boost_USE_STATIC_LIBS ON)
ADD_DEFINITIONS(-DBoost_USE_STATIC_LIBS=ON)

set (BOOST_ROOT "C:/Users/xomaq/Desktop/boost_1_71_0/boost_1_71_0")
set (BOOST_LIBRARYDIR "C:/Users/xomaq/Desktop/boost_1_71_0/boost_1_71_0")

set (Client_Console BlackJack_Console_Client)
set (Client BlackJack_Graphic_Client)
set (ClientBot BlackJack_Client_Bot)
set (Server BlackJack_Server)

find_package(Boost 1.71.0 REQUIRED)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)

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

            src/Server/Engine/BlackJack.cpp

            src/Server/Controller/Game_Room.cpp
            )

    add_executable(${Client_Console}
            src/Client/Player/ConsolePlayer/main.cpp

            src/Model/Actors/IPlayer.cpp

            src/Model/Cards/CardStack.cpp

            src/Client/View/OutputManager.cpp

            src/Client/Player/ConsolePlayer/TCP_Player_Client.cpp)

    add_executable(${Client}
            src/Client/Player/GraphicPlayer/main.cpp
            src/Client/Player/GraphicPlayer/TCP_Player_Client.cpp

            src/Model/Actors/IPlayer.cpp

            src/Model/Cards/CardStack.cpp

            src/Client/View/OutputManager.cpp

            src/Client/View/Graphic/Texture2D.cpp
            src/Client/View/Graphic/Sprite.cpp
            src/Client/View/Graphic/SpriteAnimator.cpp
            src/Client/View/Graphic/VertexBuffer.cpp
            src/Client/View/Graphic/VertexArray.cpp
            src/Client/View/Graphic/IndexBuffer.cpp
            src/Client/View/Graphic/Shader.cpp

            dependencies/glad/src/glad.c
            src/Client/View/stb_image.cpp

            src/Client/View/Objects/Table.cpp
            src/Client/View/Objects/Card.cpp
            src/Client/View/Objects/Stack.cpp
            src/Client/View/Objects/Net_Player.cpp
            src/Client/View/Objects/Button.cpp
            src/Client/View/Objects/Chips.cpp
            src/Client/View/Objects/IGameObject.cpp

            src/Client/Player/GraphicPlayer/Engine/Engine.cpp
            src/Client/Player/GraphicPlayer/Engine/Camera.cpp

            src/Client/Player/GraphicPlayer/graphic_config.cpp
            )

    add_executable(${ClientBot}
            src/Client/Bot/main.cpp

            src/Model/Actors/IPlayer.cpp

            src/Model/Cards/CardStack.cpp

            src/Client/View/OutputManager.cpp

            src/Client/Bot/TCP_Bot_Client.cpp src/Client/Bot/TCP_Bot_Client.h src/Client/View/stb_image.cpp)

    target_include_directories(${Server} PRIVATE src src/Model src/Server/Engine)

    target_include_directories(${Client_Console} PRIVATE src src/Model src/Client/View)

    target_include_directories(${Client} PRIVATE src src/Model src/Client/View src/Client/Player)

    target_include_directories(${ClientBot} PRIVATE src src/Model src/Client/View)

    target_link_libraries(${Server}
            ${Boost_LIBRARIES}
            )

    target_link_libraries(${Client_Console}
            ${Boost_LIBRARIES}
            )

    target_link_libraries(${Client}
            ${Boost_LIBRARIES}
            )

    target_link_libraries(${ClientBot}
            ${Boost_LIBRARIES}
            )
endif()
if (WIN32)
    target_link_libraries(${Server} wsock32 ws2_32)
    target_link_libraries(${Client_Console} wsock32 ws2_32)
    target_link_libraries(${Client} wsock32 ws2_32)
    target_link_libraries(${ClientBot} wsock32 ws2_32)
endif()

add_subdirectory(contrib/glfw)
target_link_libraries(${Client} glfw)

add_subdirectory(contrib/glm)
target_link_libraries(${Client} glm)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/shaders)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/res)
include_directories("glfw_repo/include")
include_directories("dependencies/glad/include")
include_directories("dependencies")

if (WIN32)
    target_link_libraries(${Client} OpenGL::GL -lopengl32)
elseif(UNIX)
    target_link_libraries(${Client} ${OPENGL_INCLUDE_DIRS})
    target_link_libraries(${Client} glfw)
else()
    message( FATAL_ERROR "could not determine the OS" )
endif()

target_link_libraries(${Client} ${SOURCE_FILES})
target_include_directories(${Client} PRIVATE dependencies)

find_package(OpenGL REQUIRED)

add_executable(tests test/simple_tests.cpp
        src/Model/Cards/CardStack.cpp
        )

add_subdirectory(
        "contrib/gtest"
        "googletest"
)

add_subdirectory("contrib/json")

target_link_libraries(${Server} nlohmann_json)
target_link_libraries(${Client_Console} nlohmann_json)
target_link_libraries(${Client} nlohmann_json)
target_link_libraries(${ClientBot} nlohmann_json)

target_link_libraries(tests PRIVATE gtest gtest_main gmock)
target_include_directories(tests PRIVATE src src/Model)

set_target_properties(${Server} ${Client} ${ClientBot} ${Client_Console} PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
        COMPILE_OPTIONS "-Wall;-Wextra;-Wpedantic;"
        )

add_custom_command(TARGET ${Client} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/res $<TARGET_FILE_DIR:${Client}>/res)
add_custom_command(TARGET ${Client} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/shaders $<TARGET_FILE_DIR:${Client}>/shaders)

set_target_properties(${Server} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/)
set_target_properties(${Client_Console} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/)
set_target_properties(${Client} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/)
set_target_properties(${ClientBot} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/)

install(TARGETS ${Server} RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT xomageimer@gmail.com)

set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)

include(CPack)
include(CTest)

enable_testing()
add_test(test tests)