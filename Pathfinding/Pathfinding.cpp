// Pathfinding.cpp : Defines the entry point for the console application.
//

// RANDOM GENERATION:
// This program generates a 10x10 grid with Xs as walls on all border squares.
// It then randomly generates a 'blank' spaces and 'wall' spaces on the inside squares of the grid.
// A starting point 'S' and endpoint 'O' token are then placed randomly on the inside squares of the grid.
// Finally, a random path is carved out from the starting point 'S' to the endpoint 'O'.
// The end result is a basic grid with at least one possible path from start to end.
// The grid is then used to test a basic pathfinding algorithm.

// PATHFINDING:
// This program uses a rough implementation of a pathfinding algorithm, based on the described algorithm on the
// wikipedia page "Pathfinding" under "Sample algorithm," accessed August 12, 2018.

// GOALS FOR REFINEMENT:
// - Refine pathfinding algorithm and/or create variations (Dijkstra's, A*, etc.)
// - Randomly generate grids with multiple possible paths, so that the Algorithm can be demonstrated/tested
// with multiple paths, rather than only one.


#include "stdafx.h"

struct coordinate
{
	int x;
	int y;
	int moveNum;

	void print() // For debugging info.
	{
		std::cout << "coordinate X: " << x << std::endl;
		std::cout << "coordinate Y: " << y << std::endl;
		std::cout << "moveNum: " << moveNum << std::endl;
	}
};

void generateRandomGrid(std::vector<std::vector<std::string>>& grid, const int &rows, const int &cols);
void printGrid(std::vector<std::vector<std::string>>& grid, const int &rows, const int &cols);
void eraseDuplicateNodes(const std::vector<std::vector<std::string>>& grid, std::vector<coordinate>& pathMovesList, std::vector<coordinate>& adjacents);

int main()
{
	// Number of Rows and Number of Cols can be changed before compiling to make a smaller or larger grid size for testing.
	const int rows = 10;
	const int cols = 10;
	std::vector<std::vector<std::string>> grid(cols, std::vector<std::string>(rows));

	generateRandomGrid(grid, rows, cols);

	std::cin.get();

	return 0;
}

void generateRandomGrid(std::vector<std::vector<std::string>>& grid, const int &rows, const int &cols)
{
	std::random_device rd;
	std::mt19937 mt(rd());
	enum NodeType { BLANK = 1, WALL = 2 };
	int randRangeMin = NodeType::BLANK;
	int randRangeMax = NodeType::WALL;

	for (int colN = 0; colN < cols; colN++)
	{
		for (int rowN = 0; rowN < rows; rowN++)
		{
			if (colN == 0 || colN == cols - 1)
			{
				grid[colN][rowN] = "X";
			}
			else if (rowN == 0 || rowN == rows - 1)
			{
				grid[colN][rowN] = "X";
			}
			else
			{
				std::uniform_int_distribution<int> dist(randRangeMin, randRangeMax);
				int typeToPlace = dist(mt);
				if (typeToPlace == NodeType::BLANK)
				{
					grid[colN][rowN] = "X";
				}
				else if (typeToPlace == NodeType::WALL)
				{
					grid[colN][rowN] = "_";
				}
			}
		}
	}

	int tracePosStartX;
	int tracePosStartY;
	int posStartX;
	int posStartY;
	int posDestX;
	int posDestY;

	// Place start token.
	{
		std::uniform_int_distribution<int> distRow(1, rows - 2);
		int posRow = distRow(mt);
		std::uniform_int_distribution<int> distCol(1, cols - 2);
		int posCol = distCol(mt);
		grid[posCol][posRow] = "S";
		posStartX = posRow;
		posStartY = posCol;
		tracePosStartX = posRow;
		tracePosStartY = posCol;
	}

	// Place destination token.
	{
		bool searching = true;
		while (searching)
		{
			std::uniform_int_distribution<int> distRow(1, rows - 2);
			int posRow = distRow(mt);
			std::uniform_int_distribution<int> distCol(1, cols - 2);
			int posCol = distCol(mt);
			if (grid[posCol][posRow] != "S")
			{
				grid[posCol][posRow] = "O";
				searching = false;
				posDestX = posRow;
				posDestY = posCol;
			}
		}
	}

	// Carve out path from start location to end location.
	{
		enum Axis { X = 1, Y = 2 };
		enum Inc { ADD = 1, SUB = 2 };
		while (tracePosStartX != posDestX ||
			tracePosStartY != posDestY)
		{
			std::uniform_int_distribution<int> distRow(Axis::X, Axis::Y);
			int axisToMove = distRow(mt);

			if (axisToMove == Axis::X)
			{
				if (tracePosStartX < posDestX)
				{
					tracePosStartX++;
					if (grid[tracePosStartY][tracePosStartX] != "O")
					{
						grid[tracePosStartY][tracePosStartX] = "_";
					}
				}
				else if (tracePosStartX > posDestX)
				{
					tracePosStartX--;
					if (grid[tracePosStartY][tracePosStartX] != "O")
					{
						grid[tracePosStartY][tracePosStartX] = "_";
					}
				}
				else
				{
					if (tracePosStartX > 1 && tracePosStartX < rows - 1 &&
						abs(tracePosStartX - posDestX) < 5)
					{
						std::uniform_int_distribution<int> distInc(Inc::ADD, Inc::SUB);
						int incToMove = distInc(mt);

						switch (incToMove)
						{
						case Inc::ADD:
							tracePosStartX++;
							if (grid[tracePosStartY][tracePosStartX] != "O")
							{
								grid[tracePosStartY][tracePosStartX] = "_";
							}
							break;
						case Inc::SUB:
							tracePosStartX--;
							if (grid[tracePosStartY][tracePosStartX] != "O")
							{
								grid[tracePosStartY][tracePosStartX] = "_";
							}
							break;
						}
					}
				}
			}
			else if (axisToMove == Axis::Y)
			{
				if (tracePosStartY < posDestY)
				{
					tracePosStartY++;
					if (grid[tracePosStartY][tracePosStartX] != "O")
					{
						grid[tracePosStartY][tracePosStartX] = "_";
					}
				}
				else if (tracePosStartY > posDestY)
				{
					tracePosStartY--;
					if (grid[tracePosStartY][tracePosStartX] != "O")
					{
						grid[tracePosStartY][tracePosStartX] = "_";
					}
				}
				else
				{
					if (tracePosStartY > 1 && tracePosStartY < cols - 1)
					{
						std::uniform_int_distribution<int> distInc(Inc::ADD, Inc::SUB);
						int incToMove = distInc(mt);

						switch (incToMove)
						{
						case Inc::ADD:
							tracePosStartY++;
							if (grid[tracePosStartY][tracePosStartX] != "O")
							{
								grid[tracePosStartY][tracePosStartX] = "_";
							}
							break;
						case Inc::SUB:
							tracePosStartY--;
							if (grid[tracePosStartY][tracePosStartX] != "O")
							{
								grid[tracePosStartY][tracePosStartX] = "_";
							}
							break;
						}
					}
				}
			}
		}
	}

	printGrid(grid, rows, cols);
	// ----- END RANDOM GRID GENERATION -----


	// ----- BEGIN PATHFINDING ALGORITHM -----
	std::vector<coordinate> pathMovesList;

	// Add starting position to moves list
	{
		coordinate posDestCoord;
		posDestCoord.x = posDestX;
		posDestCoord.y = posDestY;
		posDestCoord.moveNum = 0;
		pathMovesList.push_back(posDestCoord);
	}

	bool searching = true;
	while (searching)
	{
		const int movesList_Len = pathMovesList.size();
		for (int i = 0; i < movesList_Len; i++)
		{
			if (pathMovesList[i].x == posStartX &&
				pathMovesList[i].y == posStartY)
			{
				searching = false;
			}
			else
			{
				enum Dirs { UP = 0, DOWN = 1, LEFT = 2, RIGHT = 3 };
				const int numAdjs = 4;
				std::vector<coordinate> adjacents(numAdjs);

				adjacents[Dirs::UP].x = pathMovesList[i].x;
				adjacents[Dirs::UP].y = pathMovesList[i].y - 1;
				adjacents[Dirs::UP].moveNum = pathMovesList[i].moveNum + 1;

				adjacents[Dirs::DOWN].x = pathMovesList[i].x;
				adjacents[Dirs::DOWN].y = pathMovesList[i].y + 1;
				adjacents[Dirs::DOWN].moveNum = pathMovesList[i].moveNum + 1;

				adjacents[Dirs::LEFT].x = pathMovesList[i].x - 1;
				adjacents[Dirs::LEFT].y = pathMovesList[i].y;
				adjacents[Dirs::LEFT].moveNum = pathMovesList[i].moveNum + 1;

				adjacents[Dirs::RIGHT].x = pathMovesList[i].x + 1;
				adjacents[Dirs::RIGHT].y = pathMovesList[i].y;
				adjacents[Dirs::RIGHT].moveNum = pathMovesList[i].moveNum + 1;

				eraseDuplicateNodes(grid, pathMovesList, adjacents);

				int remainingAdjacents = adjacents.size();
				for (int remainingAdj = 0; remainingAdj < remainingAdjacents; remainingAdj++)
				{
					pathMovesList.push_back(adjacents[remainingAdj]);
				}
			}
		}

		//printGrid(grid, rows, cols);

		//for (int p = 0; p < pathMovesList.size(); p++)
		//{
		//	pathMovesList[p].print();
		//}
	}

	// Show the path mapping on the grid, visibly.
	//for (int coord = 1; coord < pathMovesList.size(); coord++)
	//{
	//	if (grid[pathMovesList[coord].y][pathMovesList[coord].x] != "S" &&
	//		grid[pathMovesList[coord].y][pathMovesList[coord].x] != "O")
	//	{
	//		grid[pathMovesList[coord].y][pathMovesList[coord].x] = std::to_string(pathMovesList[coord].moveNum);
	//	}
	//}

	printGrid(grid, rows, cols);


	// Path has been mapped, now trace out optimal one from starting point:
	int movesUsed = 0;
	const int complete_Len = pathMovesList.size();
	while (posStartX != posDestX || posStartY != posDestY)
	{
		for (int next = 0; next < complete_Len; next++)
		{
			// UP
			if (posStartY - 1 == pathMovesList[next].y && posStartX == pathMovesList[next].x)
			{
				grid[posStartY][posStartX] = "_";
				posStartY -= 1;
				grid[posStartY][posStartX] = "S";
				next = complete_Len;
				movesUsed++;
			}
			// DOWN
			else if (posStartY + 1 == pathMovesList[next].y && posStartX == pathMovesList[next].x)
			{
				grid[posStartY][posStartX] = "_";
				posStartY += 1;
				grid[posStartY][posStartX] = "S";
				next = complete_Len;
				movesUsed++;
			}
			// LEFT
			else if (posStartY == pathMovesList[next].y && posStartX - 1 == pathMovesList[next].x)
			{
				grid[posStartY][posStartX] = "_";
				posStartX -= 1;
				grid[posStartY][posStartX] = "S";
				next = complete_Len;
				movesUsed++;
			}
			// RIGHT
			else if (posStartY == pathMovesList[next].y && posStartX + 1 == pathMovesList[next].x)
			{
				grid[posStartY][posStartX] = "_";
				posStartX += 1;
				grid[posStartY][posStartX] = "S";
				next = complete_Len;
				movesUsed++;
			}
		}
		// Show the movement happening in real-time, at a speed we can watch, for demonstration/testing purposes.
		std::this_thread::sleep_for(std::chrono::seconds(1));
		printGrid(grid, rows, cols);
	}

	std::cout << "Done." << std::endl;
	std::cout << "Moves used: " << movesUsed << std::endl;
}

void printGrid(std::vector<std::vector<std::string>>& grid, const int &rows, const int &cols)
{
	for (int colN = 0; colN < cols; colN++)
	{
		for (int rowN = 0; rowN < rows; rowN++)
		{
			std::cout << grid[colN][rowN];
		}
		std::cout << std::endl;
	}

	std::cout << std::endl;
}

void eraseDuplicateNodes(const std::vector<std::vector<std::string>>& grid, std::vector<coordinate>& pathMovesList, std::vector<coordinate>& adjacents)
{
	const int movesList_Len = pathMovesList.size();
	const int adjacents_Len = adjacents.size();
	for (int adj = 0; adj < adjacents_Len; adj++)
	{
		if (grid[adjacents[adj].y][adjacents[adj].x] == "X" ||
			grid[adjacents[adj].y][adjacents[adj].x] == "O")
		{
			adjacents.erase(adjacents.begin() + adj);
			//std::cout << "Erasing list item: " << adj << std::endl;
			eraseDuplicateNodes(grid, pathMovesList, adjacents);
			//std::cout << "List no longer has items in need of removing. Returning from function." << std::endl;
			return;
		}
		else
		{
			for (int move = 0; move < movesList_Len; move++)
			{
				if (pathMovesList[move].x == adjacents[adj].x &&
					pathMovesList[move].y == adjacents[adj].y &&
					pathMovesList[move].moveNum <= adjacents[adj].moveNum)
				{
					adjacents.erase(adjacents.begin() + adj);
					//std::cout << "Erasing list item: " << adj << std::endl;
					eraseDuplicateNodes(grid, pathMovesList, adjacents);
					//std::cout << "List no longer has items in need of removing. Returning from function." << std::endl;
					return;
				}
			}
		}
	}
	//std::cout << "List no longer has items in need of removing. Returning from function." << std::endl;
	return;
}