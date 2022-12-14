#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <cstdio>
#include "input.h"

//debug macro
#define DEBUG = true;


enum class State
{
	STATE_START,
	STATE_INITIATIVE,
	STATE_COMBAT,
	STATE_COMBAT_END,
	STATE_PLAYER_TURN,
	STATE_ENEMY_TURN,
	STATE_END
};

enum class PlayerStates
{
	STATE_ATTACK,
	STATE_MOVE,
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
	bool attack(Entity* entity) {
		entity->health -= this->damage;
		if (entity->health <= 0) {
			entity->isAlive = false;
		}
		return true;
	}
};

enum class eDirecton {LEFT = 'a', RIGHT = 'd', UP = 'w', DOWN = 's' };


void clearScreen() {
#ifdef _WIN32
	system("cls");
#else
	system("clear");
#endif
}

void Parse(char key)
{
	switch (key)
	{
	case 'a':
		//dir = LEFT;
		break;
	case 'd':
		//dir = RIGHT;
		break;
	case 'w':
		//dir = UP;
		break;
	case 's':
		//dir = DOWN;
		break;
	}
}

void printEntityValues(Entity& player, Entity& enemy) {
	//display player and enemy to the console
	std::cout << player.name << " Health: " << player.health << '\n';
	std::cout << enemy.name << " Health: " << enemy.health << '\n';
}

void refresh(std::vector<std::vector<char>> grid, Entity player, Entity enemy) {
	clearScreen();
	printEntityValues(player, enemy);
	printGrid(grid);
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
	int combat_counter = 0;
	int turn_counter = 0;

	//initialize player and enemy
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
			printEntityValues(player, enemy);
			printGrid(grid);

			//wait for input
			std::cout << "Press any key to continue." << std::endl;
			//fflush(stdin);
			int key = -1;
			while (key == -1) {
				key = Input();
			}

			//go to next state
			state = State::STATE_INITIATIVE;
			//clearScreen();
			break;
		}
		case State::STATE_INITIATIVE:
		{
			std::cout << "Initiative phase" << std::endl;
			//randomly decide who goes first
			std::cout << "Rolling Dice to decide turn order" << std::endl;
			player.initiative = rand() % 20;
			enemy.initiative = rand() % 20;
			std::cout << player.name << " initiative: " << player.initiative << std::endl;
			std::cout << enemy.name << " initiative: " << enemy.initiative << std::endl;

						
			//wait for input
			std::cout << "Press any key to continue." << std::endl;
			int key = -1;
			while (key == -1) {
				key = Input();
			}

			//go to next state
			state = State::STATE_COMBAT;
			clearScreen();
			break;
		}

		case State::STATE_COMBAT:
		{
			combat_counter++;

			std::cout << "Combat phase " << combat_counter <<  std::endl;
			printEntityValues(player, enemy);
			refresh(grid, player, enemy);

			//go to state based on turn order
			if (player.initiative > enemy.initiative) {
				state = State::STATE_PLAYER_TURN;
			}
			else {
				state = State::STATE_ENEMY_TURN;
			}

			break;
		}
		case State::STATE_PLAYER_TURN: {
			turn_counter++;
			std::cout << "Turn " << turn_counter << " " << player.name << "'S turn" << std::endl;
			//go to state based on turn order
			if (turn_counter == 1) {
				state = State::STATE_ENEMY_TURN;
			}
			else {
				turn_counter = 0;
				state = State::STATE_COMBAT;
			}
			//wait for input
			std::cout << "Press any key to continue." << std::endl;
			int key = -1;
			while (key == -1) {
				key = Input();
			}
			break; 
		}
		case State::STATE_ENEMY_TURN:
		{

			turn_counter++;
			//announcement
			std::cout << "Turn " << turn_counter << " " <<  enemy.name << "'S turn" << std::endl;
			
			
			//get random number
			int random_number = rand() % 2;

			//chose action randomly
			PlayerStates actions;
			if (random_number == 1) {
				actions = PlayerStates::STATE_ATTACK;
			}
			else {
				actions = PlayerStates::STATE_ATTACK;
			}

			int num_actions = 1;
			bool exit = false;

			//go to state based on turn order
			if (turn_counter == 1) {
				state = State::STATE_PLAYER_TURN;
			}
			else {
				turn_counter = 0;
				state = State::STATE_COMBAT;
			}

			while (exit == false) {
				//PLAYER STATES
				switch (actions)
				{
				case PlayerStates::STATE_ATTACK:
				{
					int past_health = player.health;
					enemy.attack(&player);
					std::cout << enemy.name << " attacked " << player.name << " and did " << enemy.damage
						<< " points of damage\n";

					// go to STATE_COMBAT_END STATE if player died from the attack
					if (!player.isAlive) {
						state = State::STATE_COMBAT_END;
						exit = true;
					}
					num_actions--;
					if (num_actions < 1) {
						exit = true;
					}
					break;
				}
				case PlayerStates::STATE_MOVE:
				{
					break;
				}
				}

			}

			break;

		}
		case State::STATE_COMBAT_END:
		{
			if (player.isAlive) {
				std::cout << "You Died!" << std::endl;
			}
			else {
				std::cout << "You Won!" << std::endl;
			}
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