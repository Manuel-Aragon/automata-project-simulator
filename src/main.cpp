#include <iostream>
#include <vector>
#include <algorithm>

//character
class character{
    //health
    //action


};

//single square in grid
struct coordinate{
    //character to display
    char display_character = ' ';

    //function returns true if coordinate is occupied
    //places character in square
    //removes character from square

};

//2d grid
struct grid{

    int size;
    std::vector<std::vector<coordinate>> matrix;

    grid(int x){
        size = x;
        matrix.resize(size);
        for(int i = 0; i < size; i++){
            for(int j = 0; j < size; j++){
                matrix[i].emplace_back(coordinate{});
            }
        }
    };
    ~grid(){
        // for(int i = 0; i < size; i++){
        //     for(int j = 0; j < size; j++){
        //         delete matrix[i][j];
        //     }
        // }
    }

    //vector to hold grid


    //moves character form one square to another

    //display grid to console;
    void display(){
        std::cout << "Game Board\n";
        //display upper lines
        // for(int i = 0; i < size; i++){
        //     std::cout << '_';
        // }

        for(int i = 0; i < (size*2) + 1; i++){
            std::cout << '_';
        }
        std::cout << '\n';
        for(int i = 0; i < size; i++){
            //display coordinate marker
            std::cout << '|';
            for(int j = 0; j < size; j++){
                std::cout << matrix[i][j].display_character;
                std::cout << '|';
            }
            std::cout << '\n';
            for(int i = 0; i < (size*2) + 1; i++){
                std::cout << '-';
            }
            std::cout << '\n';
        }
    }
};


void simulate(int board_size){
    //anounce simulation start
    std::cout << "Simulation Start\n";

    //initialize gameboard
    grid gameBoard(board_size);
    //display baord
    gameBoard.display();

    //state machine
        //Start State
            //establish positions
            //roll initative
        //Combat
            //take turns until one is dead
        //End State
            //declare winner
            //clean up

    //announce simulation end
    std::cout << "Simulation Complete\n";
}

int main(){

    int board_size = 5;
    simulate(board_size);
}