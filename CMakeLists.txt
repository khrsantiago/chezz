set(CMAKE_CXX_COMPILER g++)
cmake_minimum_required(VERSION 3.28)
project(CMakeSFMLProject LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.2
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(SFML)

set(gamefiles src/main.cpp lib/src/Archer.cpp lib/src/SpriteManager.cpp lib/src/PieceInstantiator.cpp lib/src/Bishop.cpp lib/src/Board.cpp lib/src/Crook.cpp lib/src/Horse.cpp lib/src/IGameObject.cpp lib/src/IGameState.cpp lib/src/IPiece.cpp lib/src/Pawn.cpp lib/src/Portal.cpp lib/src/Queen.cpp lib/src/StateGameplay.cpp lib/src/Tower.cpp lib/src/Trapper.cpp lib/src/Gametree.cpp lib/src/StateTutorial.cpp lib/src/GameEvaluator.cpp lib/src/GameStateManager.cpp lib/src/Stats.cpp lib/src/StateStats.cpp lib/src/FileManager.cpp lib/src/StateMainMenu.cpp lib/src/ScreenTransition.cpp lib/src/IStatePlayable.cpp lib/src/StateVersus.cpp lib/src/StatePractice.cpp lib/src/SoundManager.cpp)
add_executable(main ${gamefiles})
include_directories(main PRIVATE lib/include)
target_compile_features(main PRIVATE cxx_std_17)
target_link_libraries(main PRIVATE SFML::Graphics SFML::Audio)

file(
    COPY assets
    DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
)

file(
    COPY kingpresets.json
    DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
)
