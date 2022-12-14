#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <cstdio>

//debug macro
#define DEBUG = true;


enum class State
{
	STATE_START,
	STATE_INITIATIVE,
	STATE_END
};

//print grid to console
void printGrid(std::vector<std::vector<char>> grid) {
	//std::cout << "Grid:" << std::endl;
	std::cout << "----------------------" << std::endl;
	for (int i = 0; i < grid.size(); i++) {
		std::cout << "|";
		for (int j = 0; j < grid[i].size(); j++) {
			std::cout << grid[i][j] << " ";
		}
		std::cout << "|";
		std::cout << std::endl;
	}
	std::cout << "----------------------" << std::endl;
}

//create an enitity class
class Entity {
public:
	std::string name;
	int x;
	int y;
	char symbol;
	int initiative;
	int health;
	int damage;
	bool isAlive;
	Entity(std::string name = "",int x = 0, int y = 0, char symbol = 'x', int initiative = 0, int health = 10, int damage = 1) {
		this->name = name;
		this->x = x;
		this->y = y;
		this->symbol = symbol;
		this->initiative = initiative;
		this->health = health;
		this->damage = damage;
		this->isAlive = true;
	}
	void move(int x, int y) {
		this->x = x;
		this->y = y;
	}
	void attack(Entity* entity) {
		entity->health -= this->damage;
		if (entity->health <= 0) {
			entity->isAlive = false;
		}
	}
};

void clearScreen() {
#ifdef _WIN32
	system("cls");
#else
	system("clear");
#endif
}

void simulator() {
	std::vector<std::vector<char>> grid(10, std::vector<char>(10, ' '));
	State state = State::STATE_START;
	std::string name;

	//output "Enter name of player" to console
	std::cout << "Enter name of player" << std::endl;
	std::cin >> name;
	//clear console screen
	clearScreen();


	Entity player(name, 0,0, 'P');
	Entity enemy("monster", 9,9,'M',0,10,2);
	
	while (true) {
		//transition between states
		switch (state)
		{
		case State::STATE_START:
		{
			//put player and monster on grid
			grid[player.x][player.y] = player.symbol;
			grid[enemy.x][enemy.y] = enemy.symbol;
			
			//display grid and text
			std::cout << "Starting simulation" << std::endl;
			std::cout << player.name << " vs " << enemy.name << std::endl;
			printGrid(grid);

			//wait for input
			std::cout << "Press enter to continue." << std::endl;
			fflush(stdin);
			getchar();
			
			//go to next state
			state = State::STATE_INITIATIVE;
			clearScreen();
			break;
		}
		case State::STATE_INITIATIVE:
		{
			std::cout << "Initiative phase" << std::endl;
			//randomly decide who goes first
			std::cout << "Rolling Dice to decide turn order" << std::endl;
			player.initiative = rand() % 2;
			enemy.initiative = rand() % 2;
			std::cout << player.name << " initiative: " << player.initiative << std::endl;
			
			state = State::STATE_END;
			break;
		}

		case State::STATE_END:
		{
			std::cout << "Ending simulation" << std::endl;
			return;
		}
		}
	}
}


int main() {
	simulator();
	return 0;
}