#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <cstdio>

#ifdef __linux__ 
#include "kbhit.h"
#include <unistd.h>
#elif _WIN32
#include <conio.h>
#else
#include "kbhit.h"
#include <unistd.h>
#endif

#include "input.h"

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

enum class EntityStates
{
	STATE_DECIDE,
	STATE_ATTACK,
	STATE_MOVE,
	STATE_END
};

enum class EnemyAIStates
{
	STATE_START,
	STATE_FAR,
	STATE_CLOSE
};

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
	int movement;
	Entity(std::string name = "",int x = 0, int y = 0, char symbol = 'x', int initiative = 0, int health = 10, int damage = 1, int movement = 3) {
		this->name = name;
		this->x = x;
		this->y = y;
		this->symbol = symbol;
		this->initiative = initiative;
		this->health = health;
		this->damage = damage;
		this->isAlive = true;
		this->movement = movement;
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

void clearScreen() {
#ifdef _WIN32
	system("cls");
#else
	system("clear");
#endif
}

class Grid{
public:
	// Constructor that initializes the grid to be a 10x10 vector of ' ' characters
	Grid() : m_grid(10, std::vector<char>(10, ' ')) {}

	// Method that clears the grid and fills it with ' ' characters
	void resetGrid() {
		m_grid.clear();
		m_grid.resize(10);
		std::fill(m_grid.begin(), m_grid.end(), std::vector<char>(10, ' '));
	}

	//print grid to console
	void printGrid() {
		//std::cout << "Grid:" << std::endl;
		std::cout << "----------------------" << std::endl;
		for (int i = 0; i < m_grid.size(); i++) {
			std::cout << "|";
			for (int j = 0; j < m_grid[i].size(); j++) {
				std::cout << m_grid[i][j] << " ";
			}
			std::cout << "|";
			std::cout << std::endl;
		}
		std::cout << "----------------------" << std::endl;
	}

	bool placeOnGrid(int x, int y, char symbol) {
		//bound check
		if (inBounds(x,y)) {	
			m_grid[y][x] = symbol;	//x and y are swampped due to vector being [y][x]
			return true;
		}
		return false;
	}

	bool inBounds(int x, int y) {
		if ((x < 0 or x > 9) or (y < 0 or y > 9)) {
			return false;
		}
		return true;
	}

private:
	std::vector<std::vector<char>> m_grid;
};



void printEntityValues(Entity& player, Entity& enemy) {
	//display player and enemy to the console
	std::cout << player.name << " Health: " << player.health << '\n';
	std::cout << enemy.name << " Health: " << enemy.health << '\n';
}

enum class eDirecton { LEFT = 'a', RIGHT = 'd', UP = 'w', DOWN = 's', ERROR };
eDirecton Parse(char key)
{
	switch (key)
	{
	case 'a':
		return eDirecton::LEFT;
	case 'd':
		return eDirecton::RIGHT;
	case 'w':
		return eDirecton::UP;
	case 's':
		return eDirecton::DOWN;
	default:
		return eDirecton::DOWN;
	}
}

int difference(int x, int y, int new_x, int new_y) {
	int x_difference = std::abs(new_x - x);
	// 8 - 9 = -1
	int y_difference = std::abs(new_y - y);
	// 0

	return x_difference + y_difference;
}

void simulator() {
	Grid grid;
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
	Entity player(name, 0,0, 'P', 0,10,1,3);
	Entity enemy("monster", 9,9,'M',0,10,2,2);
	
	while (true) {
		//transition between states
		switch (state)
		{
		case State::STATE_START:
		{
			//put player and monster on grid
			grid.placeOnGrid(player.x, player.y, player.symbol);
			grid.placeOnGrid(enemy.x, enemy.y, enemy.symbol);
			
			//display grid and text
			std::cout << "Starting simulation" << std::endl;
			std::cout << player.name << " vs " << enemy.name << std::endl;
			printEntityValues(player, enemy);
			grid.printGrid();

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
			//fflush(stdin);
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
			clearScreen();
			grid.printGrid();

			//go to state based on turn order
			if (player.initiative > enemy.initiative) {
				state = State::STATE_PLAYER_TURN;
			}
			else {
				state = State::STATE_ENEMY_TURN;
			}

			break;
		}
		case State::STATE_PLAYER_TURN:
		{
			turn_counter++;
			EntityStates action_state = EntityStates::STATE_DECIDE;
			bool has_moved = false;
			bool has_attacked = false;
			//announcement
			std::cout << "Turn " << turn_counter << " " << player.name << "'S turn" << std::endl;


			bool exit = false;
			while (exit == false) {
				//Entity States
				switch (action_state)
				{
				case EntityStates::STATE_DECIDE:
				{
					//get input on action
					char input = -1;
					while (input == -1) {
						std::cout << "What do you want to do?\n"
							<< "Move(1), Attack(2), GiveUp(3)\n"
							<< "Enter your choice.\n";
						std::cin >> input;
						switch (input)
						{
						case 1:
							action_state = EntityStates::STATE_ATTACK;
							break;
						case 2:
							action_state = EntityStates::STATE_MOVE;
							break;
						case 3:
							exit = true;
							break;
						}
					}
					break;
				}
				case EntityStates::STATE_ATTACK:
				{
					int past_health = enemy.health;
					player.attack(&enemy);
					has_attacked = true;
					std::cout << player.name << " attacked " << enemy.name << " and did " << player.damage
						<< " points of damage!\n";
								//wait for input

					// Check if enemy is  alive
					if (!enemy.isAlive) {
						//end action states and go to combat end state.
						state = State::STATE_COMBAT_END;
						action_state = EntityStates::STATE_END;
						break;
					}

					//if (has_moved == false) {
					//	action_state = EntityStates::STATE_MOVE;
					//}
					//else {
					//	action_state = EntityStates::STATE_END;
					//}
					action_state = EntityStates::STATE_END;
					break;
				}
				case EntityStates::STATE_MOVE:
				{

					//loop until confirmation
					int new_x = player.x;
					int new_y = player.y;
					char move_symbol = '@';
					int spaces_moved = 0;
					while (has_moved == false) {
						std::cout << "Use WASD to move your character and press 'x' to confirm \n";
						std::cout << "You can move " << player.movement - spaces_moved << " more spaces\n";
						clearScreen();
						//place two entities on grid
						grid.resetGrid();
						grid.placeOnGrid(enemy.x, enemy.y, enemy.symbol);
						grid.placeOnGrid(player.x, player.y, player.symbol);
						grid.placeOnGrid(new_x, new_y, move_symbol);
						grid.printGrid();
						int key = Input();
						switch (key)
						{
						case 'a'://left
							//check bounds and if movement possible
							if (grid.inBounds(new_x - 1, new_y) and difference(player.x, player.y, new_x - 1, new_y) <= player.movement) {
								new_x--;
								spaces_moved = difference(player.x, player.y, new_x, new_y);
							}
							break;
						case 'd'://right
							//check bounds and if movement possible
							if (grid.inBounds(new_x + 1, new_y) and difference(player.x, player.y, new_x + 1, new_y) <= player.movement) {
								new_x++;
								spaces_moved = difference(player.x, player.y, new_x, new_y);
							}
							break;
						case 'w'://up
							//check bounds and if movement possible
							if (grid.inBounds(new_x, new_y - 1) and difference(player.x, player.y, new_x, new_y - 1) <= player.movement) {
								new_y--;
								spaces_moved = difference(player.x, player.y, new_x, new_y);
							}
							break;
						case 's'://down
							//check bounds and if movement possible
							if (grid.inBounds(new_x, new_y + 1) and difference(player.x, player.y, new_x, new_y + 1) <= player.movement) {
								new_y++;
								spaces_moved = difference(player.x, player.y, new_x, new_y);
							}
							break;
						case 'x':
							player.move(new_x, new_y);
							has_moved = true;
							break;
						}

					}
					clearScreen();
					//place two entities on grid
					grid.resetGrid();
					grid.placeOnGrid(enemy.x, enemy.y, enemy.symbol);
					grid.placeOnGrid(player.x, player.y, player.symbol);
					//print to screen
					std::cout << "Turn " << turn_counter << " " << player.name << "'S turn" << std::endl;
					printEntityValues(player, enemy);
					grid.printGrid();
					std::cout << player.name << " moved!";
								//wait for input
			std::cout << "Press any key to continue." << std::endl;
			//fflush(stdin);
			int key = -1;
			while (key == -1) {
				key = Input();
			}


					//if (has_attacked == false) {
					//	action_state = EntityStates::STATE_MOVE;
					//}
					//else {
					//	action_state = EntityStates::STATE_END;
					//}
					action_state = EntityStates::STATE_END;
					break;
				}
				case EntityStates::STATE_END:
				{
					std::cout << player.name << "'S turn is ending." << std::endl;
								//wait for input
			std::cout << "Press any key to continue." << std::endl;
			//fflush(stdin);
			int key = -1;
			while (key == -1) {
				key = Input();
			}
					exit = true;
					break;
				}
				}
			}
			//go to state based on turn order
			if (turn_counter == 1) {
				state = State::STATE_PLAYER_TURN;
			}
			else {
				turn_counter = 0;
				state = State::STATE_COMBAT;
			}
			break;

		}
		case State::STATE_ENEMY_TURN:
		{
			turn_counter++;
			EntityStates actions = EntityStates::STATE_DECIDE;
			bool moved = false;
			bool attacked = false;
			EnemyAIStates current_AI_state = EnemyAIStates::STATE_START;


			//announcement
			std::cout << "Turn " << turn_counter << " " <<  enemy.name << "'S turn" << std::endl;

			bool exit = false;
			bool decision = false;
			while (exit == false) {
				//Entity States
				switch (actions)
				{
				case EntityStates::STATE_DECIDE:
				{

					//choose action randomly

					while (!decision) {
						switch (current_AI_state)
						{
						case EnemyAIStates::STATE_START:
						{
							if (difference(enemy.x, enemy.y, player.x, player.y) > 1) {
								current_AI_state = EnemyAIStates::STATE_FAR;
							}
							else {
								current_AI_state = EnemyAIStates::STATE_CLOSE;
							}

						}
						case EnemyAIStates::STATE_FAR:
						{
							actions = EntityStates::STATE_MOVE;
							decision = true;
							break;
						}
						case EnemyAIStates::STATE_CLOSE:
						{
							actions = EntityStates::STATE_ATTACK;
							decision = true;
							break;
						}
						}
					}
				case EntityStates::STATE_ATTACK:
				{
					int past_health = player.health;
					enemy.attack(&player);
					attacked = true;
					std::cout << enemy.name << " attacked " << player.name << " and did " << enemy.damage
						<< " points of damage!\n";
								//wait for input
			std::cout << "Press any key to continue." << std::endl;
			//fflush(stdin);
			int key = -1;
			while (key == -1) {
				key = Input();
			}

					// Check if player is  not alive
					if (!player.isAlive) {
						//end action states and go to combat end state.
						state = State::STATE_COMBAT_END;
						actions = EntityStates::STATE_END;
						break;
					}

					actions = EntityStates::STATE_END;
					break;
				}
				case EntityStates::STATE_MOVE:	//enemy
				{

					clearScreen();
					//place two entities on grid
					grid.resetGrid();
					grid.placeOnGrid(enemy.x, enemy.y, enemy.symbol);
					grid.placeOnGrid(player.x, player.y, player.symbol);
					//print to screen
					std::cout << "Turn " << turn_counter << " " << enemy.name << "'S turn" << std::endl;
					printEntityValues(player, enemy);
					grid.printGrid();
					std::cout << enemy.name << " moved!";
								//wait for input
			std::cout << "Press any key to continue." << std::endl;
			//fflush(stdin);
			int key = -1;
			while (key == -1) {
				key = Input();
			}


					actions = EntityStates::STATE_END;

					break;
				}
				case EntityStates::STATE_END:
				{
					std::cout << enemy.name << "'S turn is ending." << std::endl;
								//wait for input
			std::cout << "Press any key to continue." << std::endl;
			//fflush(stdin);
			int key = -1;
			while (key == -1) {
				key = Input();
			}
					exit = true;
					break;
				}
				}
				}
			}

			//go to state based on turn order
			if (turn_counter == 1) {
				state = State::STATE_PLAYER_TURN;
			}
			else {
				turn_counter = 0;
				state = State::STATE_COMBAT;
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