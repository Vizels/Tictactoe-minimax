#include <iostream>
#include <stdlib.h>
#include <vector>


int max(int a, int b)
{
	if (a > b)
	{
		return a;
	}
	else
	{
		return b;
	}
}

int min(int a, int b)
{
	if (a < b)
	{
		return a;
	}
	else
	{
		return b;
	}
}


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
			for (int j = 0; j < field_size - 1; j++)
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
			counterD_TLtoBR_up = 0;
			counterD_TLtoBR_down = 0;
			counterD_TRtoBL_up = 0;
			counterD_TRtoBL_down = 0;
		}

		return 1;
	}

	bool has_neighbours(int row, int col)
	{
		for (int i = row-1; i <= row+1; i++)
		{
			for (int j = col-1; j <= col+1; j++)
			{
				if (i >= 0 && j >= 0 && i < field_size && j < field_size)
				{
					if (field[i][j] == -turn ||  field[i][j] == -!turn)
					{
						return true;
					}
				}
			}
		}
		return false;
	}

	void AI_make_turn()
	{
		int best_val = INT_MIN;
		int best_move_row = -1;
		int best_move_col = -1;

		//define depth
		int depth;
		if (field_size < 7)
		{
			depth = 9 - field_size;
		}
		else
		{
			depth = 3;
		}

		for (int i = 0; i < field_size * field_size; i++)
		{
			if ((field[i / field_size][i % field_size] == i + 1) && has_neighbours(i/field_size, i % field_size))
			{
				field[i / field_size][i % field_size] = -turn;
				turn = !turn;
				round_counter++;
				
				int move_val = minimax(false, depth, INT_MIN, INT_MAX);

				field[i / field_size][i % field_size] = i + 1;
				turn = !turn;
				round_counter--;

				if (move_val > best_val)
				{
					if (i < 0)
					{
						std::cout << "Error";
					}
					else
					{
					best_move_row = i / field_size;
					best_move_col = i % field_size;
					best_val = move_val;
					std::cout << best_move_row << ' ' << best_move_col << i;
					}
				}
			}
		}

		if (round_counter < max_rounds)
		{
			field[best_move_row][best_move_col] = -turn;
			turn = !turn;
			round_counter++;
		}
	}

	int minimax(bool is_maximizing, int depth, int alpha, int beta)
	{

		int score = gamestate();

		if (round_counter == max_rounds || score != 1 || depth == 0)
		{
			return win_points(score, depth);
		}
		else if (is_maximizing)
		{
			int best = INT_MIN;

			for (int i = 0; i < field_size * field_size; i++)
			{
				//is cell empty
				if (field[i / field_size][i % field_size] == i + 1 && has_neighbours(i / field_size, i % field_size))
				{
					field[i / field_size][i % field_size] = -turn;
					turn = !turn;
					round_counter++;

					int minimax_val = minimax(!is_maximizing, depth -1, alpha, beta);

					best = max(best, minimax_val);

					field[i / field_size][i % field_size] = i + 1;
					turn = !turn;
					round_counter--;

					if (best > beta)
					{
						break;
					}

					alpha = max(alpha, best);

				}
			}
			return best;
		}
		else
		{
			int best = INT_MAX;

			for (int i = 0; i < field_size * field_size; i++)
			{
				//is cell empty
				if (field[i / field_size][i % field_size] == i + 1)
				{
					//make_move
					field[i / field_size][i % field_size] = -turn;
					turn = !turn;
					round_counter++;

					int minimax_val = minimax(!is_maximizing, depth -1, alpha, beta);
					
					best = min(best, minimax_val);

					field[i / field_size][i % field_size] = i + 1;
					turn = !turn;
					round_counter--;

					if (best < alpha)
					{
						break;
					}

					beta = min(beta, best);

					//undo the move
				}
			}
			return best;
		}
	}

	int win_points(int gamestate, int depth)
	{
		int result = 0;

		// depth heuristics 
		if (gamestate == -1)
		{
			return -100 + depth;
		}
		else if (gamestate == 0)
		{
			return 105 - depth;
		}
		else
		{
			//check if center is empty (only for odd field size)
			if (field[field_size / 2][field_size / 2] == 0 && field_size % 2 == 1)
			{
				result += 2;
			}

			//count x's and 0's in cols and rows to return points
			int counterH0 = 0;
			int counterHX = 0;
			int counterV0 = 0;
			int counterVX = 0;

			for (int i = 0; i < field_size; i++)
			{
				for (int j = 0; j < field_size ; j++)
				{
					if (field[i][j] == 0)
					{
						counterH0++;
					}
					else if (field[i][j] == -1)
					{
						counterHX++;
					}
					
					if (field[j][i] == 0)
					{
						counterV0++;
					}
					else if (field[j][i] == -1)
					{
						counterVX++;
					}

				}

				// checks how many Xs and 0s is on a line
				if (counterV0 == 0)
				{
					result -= counterVX * 2;
				}
				else
				{
					result += counterV0 - counterVX;
				}
				if (counterH0 == 0)
				{
					result -= counterHX * 2;
				}
				else
				{
					result += counterH0 - counterHX;
				}
				
				

				counterV0 = 0;
				counterVX = 0;
				counterH0 = 0;
				counterHX = 0;
			}
			
		}

		return result;
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

			
			if (state != -1)
			{
				AI_make_turn();
				print_field();
				state = gamestate();
			}
			

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