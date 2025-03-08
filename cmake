cmake_minimum_required(VERSION 3.15)
project(final_mario)

# If you have multiple .cpp files, list them all here
add_executable(final_mario
Animation.cpp
Block.cpp
Button.cpp
Coin.cpp
Command.cpp
CommandQueue.cpp
Component.cpp
Container.cpp
DataTables.cpp
EmitterNode.cpp
Enemy.cpp
Entity.cpp
Game.cpp
GameOverState.cpp
Label.cpp
Level1State.cpp
Level2State.cpp
Level3State.cpp
Level4State.cpp
LevelState.cpp
Mario.cpp
MenuState.cpp
ParticleNode.cpp
PauseState.cpp
Pickup.cpp
Player.cpp
Projectile.cpp
SceneNode.cpp
SettingsState.cpp
SpriteNode.cpp
State.cpp
StateStack.cpp
TextNode.cpp
TitleState.cpp
Utility.cpp
World.cpp
main.cpp

)

# If you need SDL2, for example
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIR})
target_link_libraries(final_mario ${SDL2_LIBRARY})

