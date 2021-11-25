// Kunling Yang, 20912628
// Last Modified At 1821, 20211124

#include "board.h"
#include "game.h"
//#include "window.h"
#include <iostream>
#include <memory>

int main() {
    std::unique_ptr<Game> aGame;
    std::string command;

    while(true) {
        std::cin >> command;
        if(std::cin.fail() || command == "quit")    break;

        switch (command){
        case "game":
            std::string p1;
            std::string p2;
            std::cin >> p1 >> p2;
            aGame();
        case "resign":

            break;

        case "setup":

            break;
        default:
            std::cout << "Unrecognized command!" << std::endl;
        }
    }
}