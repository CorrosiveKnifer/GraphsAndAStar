#include <string>
#include <iostream>
#include "AStarDisplay.h"
#include "Graphs.h"

//Restrict the user to between these values (A*) [DO NOT EDIT]
static unsigned int WIDTH_LIMIT = 110;
static unsigned int HEIGHT_LIMIT = 70;

//GetIntInput ( unsigned int& _val )
//
// Description:	
//		Gets only an integer input that is between
//		0 - 9999999999 else it is invalid.
//
// @Param	unsigned int&	Reference to save the user's input to.
//
// @return	bool	Status of the input (if the input was valid and saved).
bool GetIntInput(unsigned int& _val)
{
	std::string input;
	bool isValid = true;

	//Get line as a String
	std::getline(std::cin, input);

	//Looping check: O(n) | n is the size of the input
	if (input.size() > 0 && input.size() < 10)
	{
		for (int i = 0; i < input.size(); i++)
		{
			//Check if non digit is detected.
			if (!isdigit(input.at(i)))
			{
				//Found a non digit
				isValid = false;
				break;
			}
		}
	}
	else
	{
		isValid = false;
	}
	
	if (isValid)
	{
		//Converts string to integer
		_val = stoi(input);
		return isValid;
	}
	
	return isValid;
}

int SetUpMenu()
{
	unsigned int val;
	bool cond = true;
	do //Display initial menu
	{
		system("CLS");
		std::cout << "    ~Console Set Up Menu ~     " << std::endl;
		std::cout << "===============================" << std::endl;
		std::cout << "Window options:                " << std::endl;
		std::cout << "   0) A* algorithm             " << std::endl;
		std::cout << "   1) Graphs                   " << std::endl;
		std::cout << "   2) Credits                  " << std::endl;
		std::cout << "   3) Quit                     " << std::endl;

		if (!cond) //Error message:
		{
			std::cout << std::endl;
			std::cout << "Error: Invalid Input, try again" << std::endl;
		}
		else
		{
			std::cout << std::endl;
			std::cout << std::endl;
		}

		//Get input:
		std::cout << "> ";
		cond = GetIntInput(val);

		//Double check valid input:
		if (cond)
		{
			cond = val <= 3;
		}

	} while (!cond);
	return val;
}

void RunAStar()
{
	unsigned int cVal = 0;
	unsigned int rVal = 0;
	bool cond = true;
	do //Display initial menu
	{
		system("CLS");
		std::cout << "        ~ Set Up Menu ~        " << std::endl;
		std::cout << "===============================" << std::endl;
		std::cout << "Limit: " << std::to_string(WIDTH_LIMIT) << "x" << std::to_string(HEIGHT_LIMIT) << std::endl;
		std::cout << "Window options:                " << std::endl;
		
		//Error Message
		if (!cond)
		{
			std::cout << std::endl;
			std::cout << "Error: Invalid Input, try again" << std::endl;
		}
		else
		{
			std::cout << std::endl;
			std::cout << std::endl;
		}
		//Get Columns
		std::cout << " - Columns across: ";
		cond = GetIntInput(cVal) && cVal <= WIDTH_LIMIT && cVal > 0;

		//Get Rows
		if (cond)
		{
			std::cout << " - Rows down: ";
			cond = GetIntInput(rVal) && rVal <= HEIGHT_LIMIT && rVal > 0;
		}

	} while (!cond);

	//Create program
	AStarDisplay* aStar = new AStarDisplay();
	if (aStar->Initialise(cVal, rVal))
	{
		aStar->DoWindowLoop();
	}
	aStar->Deinitialise();
	delete aStar;
}

void RunGraphs()
{
	system("CLS");
	std::cout << "Loading...";
	Graph* g = new Graph();
	if (g->Initialise())
	{
		g->DoWindowLoop();

	}
	g->Deinitialise();
	delete g;
}

void Credits()
{
	system("CLS");
	std::cout << "    Coded by: Michael Jordan     " << std::endl;
	system("pause");
}

int main()
{
	bool isRunning = true;
	do 
	{
		switch (SetUpMenu())
		{
		case 0:
			//Open A*
			RunAStar();
			break;
		case 1:
			//Open Graphs
			RunGraphs();
			break;
		case 2:
			Credits();
			break;

		case 3:
		default:
			isRunning = false;
			break;
		}
	} while (isRunning);
	

	return 1;
}