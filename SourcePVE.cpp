#include <iostream>
#include <stdlib.h>
#include <vector>

class TicTacToe
{
public:

	std::vector < std::vector<int> > field; //array with info about 0's and X's (O = 0, X = -1)S
	bool turn = 1; //player X - 1 or O - 0
	int field_size;
	int win_condition;
	int round_counter = 0;
	int max_rounds;

	//initialize field size, win condition and field
	void start_game()
	{
		//set field size
		std::cout << "TIC-TAC-TOE GAME\nPlease, choose the field size (NxN) from 3 to 9: ";
		std::cin >> field_size;
		max_rounds = field_size * field_size;

		//set win condition
		std::cout << "\nSet the number of marks to win (it can't be bigger than a field size): ";
		std::cin >> win_condition;

		std::vector<int> temp;

		//fill field from 1 to Size^2 
		for (int i = 0; i < field_size; i++)
		{
			for (int j = 0; j < field_size; j++)
			{
				temp.push_back(i * field_size + j + 1);
			}
			field.push_back(temp);
			temp.clear();

		}
	}

	// -1 - player X won | 0 - player O won | 1 - game in progress
	int gamestate()
	{
		//Horizontal, Vertical and Diagonal (right and left) counters
		int counterH = 0;
		int counterV = 0;
		int counterD_TLtoBR_up = 0; //top left to bot right
		int counterD_TLtoBR_down = 0;
		int counterD_TRtoBL_up = 0; //top right to bot left
		int counterD_TRtoBL_down = 0;

		//check horizontally and vertically
		for (int i = 0; i < field_size; i++)
		{
			for (int j = 0; j < field_size-1; j++)
			{
				//horizontal count
				if (field[i][j] == field[i][j + 1])
				{
					counterH++;
				}
				else
				{
					counterH = 0;
				}

				//vertical count
				if (field[j][i] == field[j + 1][i])
				{
					counterV++;
				}
				else
				{
					counterV = 0;
				}

				//check win condition
				if (counterH == win_condition - 1)
				{
					return field[i][j];
				}
				else if (counterV == win_condition - 1)
				{
					return field[j][i];
				}
			}
			counterV = 0;
			counterH = 0;
		}

		//check diagonal
		for (int i = 0; i < field_size - win_condition + 1; i++)
		{
			for (int j = 0; j < field_size - i - 1; j++)
			{
				//top left to bot right diagonals (upper and lower)
				if (field[i + j][j] == field[i + j + 1][j + 1])
				{
					counterD_TLtoBR_down++;
				}
				else
				{
					counterD_TLtoBR_down = 0;
				}

				if (field[j][i + j] == field[j + 1][i + j + 1])
				{
					counterD_TLtoBR_up++;
				}
				else
				{
					counterD_TLtoBR_up = 0;
				}


				//top right to bot left diagonal (upper and lower)
				if (field[i + j][field_size - 1 - j] == field[i + j + 1][field_size - 1 - j - 1])
				{
					counterD_TRtoBL_down++;
				}
				else
				{
					counterD_TRtoBL_down = 0;
				}

				if (field[j][field_size - 1 - i - j] == field[j + 1][field_size - 1 - i - j - 1])
				{
					counterD_TRtoBL_up++;
				}
				else
				{
					counterD_TRtoBL_up = 0;
				}


				//check diagonal counters if anyone win, then return value of winner cell
				if (counterD_TLtoBR_down == win_condition - 1)
				{
					return field[i + j][j];
				}
				else if (counterD_TLtoBR_up == win_condition - 1)
				{
					return field[j][i + j];
				}
				else if (counterD_TRtoBL_down == win_condition - 1)
				{
					return field[i + j][field_size - 1 - j];
				}
				else if (counterD_TRtoBL_up == win_condition - 1)
				{
					return field[j][field_size - 1 - i - j];
				}

			}
			int counterD_TLtoBR_up = 0;
			int counterD_TLtoBR_down = 0;
			int counterD_TRtoBL_up = 0;
			int counterD_TRtoBL_down = 0;
		}

		return 1;
	}

	void AI_make_turn()
	{
		for (int i = 0; i < field_size * field_size; i++)
		{
			if (field[i / field_size][i % field_size] == i + 1)
			{
				field[i / field_size][i % field_size] = -turn;
				turn = !turn;
				round_counter++;
				break;
			}
		}
	}


	//player X places -1, player O places 0
	void make_turn()
	{
		int input;

		std::cout << "\nPlayer " << turn << " turn: ";
		std::cin >> input;


		while (true)
		{
			if (input == field[(input - 1) / field_size][(input - 1) % field_size])
			{
				field[(input - 1) / field_size][(input - 1) % field_size] = -turn;
				break;
			}
			else
			{
				std::cout << "\nWrong place!\n";
				std::cin >> input;
			}
		}
		round_counter++;
		turn = !turn;
	}

	void print_field()
	{
		system("CLS");
		std::cout << '\n';
		for (int i = 0; i < field_size; i++)
		{
			//print cells with values
			for (int j = 0; j < field_size; j++)
			{
				std::cout << "| ";
				if (field[i][j] == 0)
				{
					std::cout << 'O';
				}
				else if (field[i][j] == -1)
				{
					std::cout << 'X';
				}
				else
				{
					std::cout << field[i][j];
				}

				if (field[i][j] < 10 && field[i][j] > -2)
				{
					std::cout << ' ';
				}

				std::cout << " |";
			}


			std::cout << '\n';

			//underline above cells
			for (int j = 0; j < field_size; j++)
			{
				std::cout << " ---- ";
			}

			std::cout << '\n';
		}

		std::cout << "\n";
	}

	void launch()
	{
		int state;
		start_game();
		print_field();
		while (true)
		{
			make_turn();
			print_field();
			state = gamestate();

			/*AI_make_turn();
			print_field();
			state = gamestate();*/

			if (state == -1)
			{
				std::cout << "\nPLAYER X WON!\n";
				break;
			}
			else if (state == 0)
			{
				std::cout << "\nPLAYER O WON!\n";
				break;
			}
			else if (round_counter >= max_rounds)
			{
				std::cout << "\nIT'S A DRAW\n";
				break;
			}
		}
	}


};



int main()
{
	TicTacToe test;
	test.launch();

	return(0);
}