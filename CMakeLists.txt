cmake_minimum_required(VERSION 3.15)

set(PROJECT_NAME BlackJack)
project(${PROJECT_NAME})

add_executable(${PROJECT_NAME}
        src/main.cpp

        src/Model/Actors/Dealer.cpp
        src/Model/Actors/IActor.cpp
        src/Model/Cards/CardStack.cpp
        src/Model/Actors/IHandler.cpp

        src/View/OutputManager.cpp

        src/Controller/Controller.cpp

        src/Engine/BlackJack.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE src/Controller src/Model src/View src/Engine)

add_executable(tests test/simple_tests.cpp)

add_subdirectory(
        "contrib/gtest"
        "googletest"
)

target_link_libraries(tests PRIVATE gtest gtest_main gmock)

set_target_properties(${PROJECT_NAME} tests PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
        COMPILE_OPTIONS "-Wall;-Wextra;-Wpedantic;"
        )

set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/)

#add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
#        COMMAND ${CMAKE_COMMAND} -E copy_directory
#        ${CMAKE_SOURCE_DIR}/res $<TARGET_FILE_DIR:${PROJECT_NAME}>/res)

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)

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