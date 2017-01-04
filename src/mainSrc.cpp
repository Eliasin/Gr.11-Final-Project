#include "game.hpp"

int main(int argc, char * argv[]) {
    Main::GameInstance game;
    game.data = new Main::OverflowData;
    game.run();

    return 0;
}
