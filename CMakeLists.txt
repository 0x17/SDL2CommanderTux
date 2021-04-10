cmake_minimum_required(VERSION 3.17)
project(SDL2CommanderTux)

set(CMAKE_CXX_STANDARD 17)

if (WIN32)
	include_directories(. ../../../Downloads/SDL2-devel-2.0.14-VC/SDL2-2.0.14/include/SDL)
	link_directories(../../../Downloads/SDL2-devel-2.0.14-VC/SDL2-2.0.14/lib/x64)
endif (WIN32)

if (APPLE)
include_directories(.
		/Library/Frameworks/SDL2.framework/Headers/
		/Library/Frameworks/SDL2_mixer.framework/Headers/)
endif (APPLE)

add_executable(SDL2CommanderTux
	Actor.cpp
	Duel.cpp
	Enemy.cpp
	Game.cpp
	Player.cpp
	Sprite.cpp
	Worldmap.cpp
	CommanderTux.cpp
	Editor.cpp
	Framework.cpp
	Level.cpp
	Splash.cpp
	Text.cpp)

target_link_libraries(SDL2CommanderTux SDL2 SDL2main SDL2_mixer SDL2_image SDL2_ttf)
