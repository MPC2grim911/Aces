// ======================================================================
// FILE:        MyAI.cpp
//
// AUTHOR:      Abdullah Younis
//
// DESCRIPTION: This file contains your agent class, which you will
//              implement. You are responsible for implementing the
//              'getAction' function and any helper methods you feel you
//              need.
//
// NOTES:       - If you are having trouble understanding how the shell
//                works, look at the other parts of the code, as well as
//                the documentation.
//
//              - You are only allowed to make changes to this portion of
//                the code. Any changes to other portions of the code will
//                be lost when the tournament runs your code.
// ======================================================================

#include "MyAI.hpp"
#include <iostream>
#include <stack>

using namespace std;


MyAI::MyAI() : Agent()
{
	// ======================================================================
	// YOUR CODE BEGINS
	// ======================================================================
	goBack = false;
	moves = 0;
	arrowShot = false;
	wumpusDead = false;
	wumpusFound = false;
	doStuff = false;
	x = 1;
	y = 1;
	dir = 2; // north: 1, east: 2, south: 3, west: 4
	// ======================================================================
	// YOUR CODE ENDS
	// ======================================================================
}
	
Agent::Action MyAI::getAction
(
	bool stench,
	bool breeze,
	bool glitter,
	bool bump,
	bool scream
)
{
	// ======================================================================
	// YOUR CODE BEGINS
	// ======================================================================

	/*
	if (doStuff)
	{
		cout << "doing stuff" << endl;
		if (stuff.empty())
		{
			doStuff = false;
			//cout << "do stuff empty" << endl;

		}
		else
		{
			Action temp = stuff.front();
			stuff.pop();
			if (temp == TURN_LEFT)
			{
				cout << "turned left" << endl;
			}
			else if (temp == FORWARD)
			{
				cout << "moved forward" << endl;
			}
			return temp;
		}

		cout << "doStuff done" << endl;
	}
	*/

	if (goBack)
	{
		if (!retrace.empty())
		{
			cout << "testing" << endl;
			Action temp = retrace.top();
			retrace.pop();
			return temp;
		}
		else
		{
			return CLIMB;
		}

		cout << " got Error" << endl;
	}
	else
	{
		if (glitter)
		{
			cout << "Found gold" << endl;
			goBack = true;
			retrace.push(TURN_LEFT);
			retrace.push(TURN_LEFT);
			return (GRAB);

		}

		if (moves == 0)
		{
			if (breeze)
			{
				cout << "I'm out first turn" << endl;
				return CLIMB;
			}
			else if (stench)
			{
				cout << "shot an arrow" << endl;
				moves++;
				arrowShot = true;
				return SHOOT;
			}

			cout << "first turn move forward" << endl;
			moves++;
			retrace.push(FORWARD);
			
			addGoMap();

			return FORWARD;
		}

		if (scream)
		{
			cout << "killed wumpus" << endl;
			wumpusDead = true;
		}

		if (arrowShot && !wumpusDead && moves == 1)
		{
			cout << "found wumpus" << endl;
			wumpusFound = true;
			temp.first = 1;
			temp.second = 2;
			avoidMap.push_back(temp);
			moves++;
			//doStuff = true;
			//stuff.push(FORWARD);

			addGoMap();

			retrace.push(FORWARD);
			return FORWARD;
		}

		if (wumpusFound || wumpusDead)
		{
			if (breeze)
			{
				cout << "theres a breeze" << endl;
				goBack = true;
				retrace.push(TURN_LEFT);
				return TURN_LEFT;
			}

			if (bump)
			{
				cout << "Hit wall" << endl;
				retrace.push(TURN_RIGHT);
				moves++;
				return TURN_LEFT;
			}

			if (!breeze)
			{
				cout << "Moved once" << endl;
				retrace.push(FORWARD);
				moves++;
				return FORWARD;



			}
		}
		else
		{
			if (stench || breeze)
			{
				cout << "theres a breeze or stench" << endl;
				goBack = true;
				retrace.push(TURN_LEFT);
				return TURN_LEFT;
			}

			if (bump)
			{
				cout << "Hit wall" << endl;
				retrace.push(TURN_RIGHT);
				moves++;
				return TURN_LEFT;
			}

			if (!stench && !breeze)
			{
				cout << "Moved once" << endl;
				retrace.push(FORWARD);
				moves++;
				return FORWARD;



			}
		}

		
	}
	
	//cout << "climbed out" << endl;
	
	// ======================================================================
	// YOUR CODE ENDS
	// ======================================================================
}



// ======================================================================
// YOUR CODE BEGINS
// ======================================================================
void MyAI::addGoMap()
{
	if (x == 1)
	{
		temp.first = x + 1;
		temp.first = y;
		if (checkAvoid(temp))
			goMap.push_back(temp);
	}
	else
	{
		temp.first = x + 1;
		temp.first = y;
		if (checkAvoid(temp))
			goMap.push_back(temp);

		temp.first = x - 1;
		temp.first = y;
		if (checkAvoid(temp))
			goMap.push_back(temp);
	}

	if (y == 1)
	{
		temp.first = x;
		temp.first = y + 1;
		if (checkAvoid(temp))
			goMap.push_back(temp);
	}
	else
	{
		temp.first = x;
		temp.first = y + 1;
		if (checkAvoid(temp))
			goMap.push_back(temp);

		temp.first = x;
		temp.first = y - 1;
		if (checkAvoid(temp))
			goMap.push_back(temp);
	}
	
}

bool MyAI::checkAvoid(pair<int, int> test)
{
	for (auto& element : avoidMap)
	{
		if (test == element)
		{
			return false;
		}
	}

	return true;
}


// ======================================================================
// YOUR CODE ENDS
// ======================================================================