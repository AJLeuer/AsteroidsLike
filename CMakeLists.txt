cmake_minimum_required(VERSION 3.12)
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})
set(CMAKE_CXX_STANDARD 20)

project(AsteroidsLike)
set(EXECUTABLE_NAME "AsteroidsLike")
set(TEST_EXECUTABLE_NAME "AsteroidsLikeTest")

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)
#find_package(XCTest REQUIRED)

include_directories(Source/Control)
include_directories(Source/Output)
include_directories(Source/Util)
include_directories(Source/World)
include_directories(${SDL2_INCLUDE_DIR})
include_directories(/usr/local/include)
include_directories(Libraries/GoogleTest/googletest/include)
include_directories(${XCTest_INCLUDE_DIRS})

add_executable(${EXECUTABLE_NAME}
        Source/Control/Configuration.cpp
        Source/Control/Configuration.h
        Source/Control/DefaultConfig.h
        Source/Control/GlobalVar.h
        Source/Control/Input.cpp
        Source/Control/Input.hpp
        Source/Control/main.cpp
        Source/Control/MainControl.cpp
        Source/Control/MainControl.h
        Source/Control/Player.cpp
        Source/Control/Player.h
        Source/Output/Decl_Apple.h
        Source/Output/DisplayData.cpp
        Source/Output/DisplayData.h
        Source/Output/DisplayData.mm
        Source/Output/GameColor.h
        Source/Output/GraphicalOutput.cpp
        Source/Output/GraphicalOutput.h
        Source/Output/GraphicsData.cpp
        Source/Output/GraphicsData.hpp
        Source/Output/TextOutput.hpp
        Source/Test/TestSetup/GoogleTests.mm
        Source/Test/Util/UtilTest.cpp
        Source/Test/AsteroidsLikeTest.m
        Source/Test/ExampleTest.cpp
        Source/Util/AssetFileIO.cpp
        Source/Util/AssetFileIO.h
        Source/Util/BasicConcurrency.cpp
        Source/Util/BasicConcurrency.h
        Source/Util/BoundsCheck.hpp
        Source/Util/Debug.cpp
        Source/Util/Debug.h
        Source/Util/HardwareRand.c
        Source/Util/Line.hpp
        Source/Util/Navigator.hpp
        Source/Util/Random.cpp
        Source/Util/Random.hpp
        Source/Util/Rectangle.hpp
        Source/Util/Size.hpp
        Source/Util/Timer.cpp
        Source/Util/Timer.hpp
        Source/Util/Util.cpp
        Source/Util/Util.hpp
        Source/Util/Util2.h
        Source/Util/Vect.hpp
        Source/Util/Velocity.cpp
        Source/Util/Velocity.hpp
        Source/World/Ability.cpp
        Source/World/Ability.h
        Source/World/CharacterData.h
        Source/World/Enemy.cpp
        Source/World/Enemy.h
        Source/World/ForwardDecl.h
        Source/World/GameEvent.cpp
        Source/World/GameEvent.h
        Source/World/GameInterface.h
        Source/World/GameMap.hpp
        Source/World/GameObject.cpp
        Source/World/GameObject.h
        Source/World/GameState.cpp
        Source/World/GameState.hpp
        Source/World/NPC.cpp
        Source/World/NPC.h
        Source/World/Pawn.cpp
        Source/World/Pawn.h
        Source/World/PlayerCharacter.cpp
        Source/World/PlayerCharacter.h
        Source/World/Weapon.cpp
        Source/World/Weapon.h
        Source/World/WorldControl.cpp
        Source/World/WorldControl.h
)

add_executable(${TEST_EXECUTABLE_NAME} Source/Test/Util/UtilTest.cpp Source/Test/ExampleTest.cpp)

file(COPY Libraries/googletest-src DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/)

# Add googletest directly to our build. This defines
# the gtest and gtest_main targets.
add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/googletest-src
        ${CMAKE_CURRENT_BINARY_DIR}/googletest-build
        EXCLUDE_FROM_ALL)

target_link_libraries(${EXECUTABLE_NAME}
        ${SDL2_LIBRARY}
        ${SDL2_LIBRARIES}
        ${SDL2_IMAGE_LIBRARIES}
        ${SDL2_TTF_LIBRARIES})

target_link_libraries(${EXECUTABLE_NAME} gtest gtest_main)
target_link_libraries(${TEST_EXECUTABLE_NAME} gtest gtest_main)
#target_link_libraries(${EXECUTABLE_NAME} XCTest_LIBRARIES)

add_test(NAME example_test COMMAND example)

file(COPY Assets DESTINATION ${CMAKE_BINARY_DIR}/)