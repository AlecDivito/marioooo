cmake_minimum_required(VERSION 3.10)
project(final_mario)

# Use C++17 or whatever standard you need
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Gather all .cpp files into one variable
set(SOURCE_FILES
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

# Create the executable
add_executable(final_mario ${SOURCE_FILES})

# Find the SFML package (requires SFML installed, e.g. via brew)
find_package(SFML 2 COMPONENTS system window graphics audio network REQUIRED)

# Link SFML libraries to our executable
target_link_libraries(final_mario
    sfml-system
    sfml-window
    sfml-graphics
    sfml-audio
    sfml-network
)

