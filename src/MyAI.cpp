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
	foundGold = false;
	bool spin = false;
	x = 1;
	y = 1;
	xLim = -1;
	yLim = -1;
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
				retrace.push(TURN_LEFT);
			}
			else if (temp == TURN_RIGHT)
			{
				cout << "turned right" << endl;
				retrace.push(TURN_RIGHT);
			}
			else if (temp == FORWARD)
			{
				cout << "moved forward" << endl;
			}
			return temp;
		}

		cout << "doStuff done" << endl;
	}
	

	if (goBack)
	{
		if (canExplore() && !foundGold && !(test.first == x && test.second == y))
		{
			goBack = false;
			retrace.push(TURN_LEFT);
			retrace.push(TURN_LEFT);
			Action temp = explore();
			return temp;
		}

		if (!retrace.empty())
		{
			cout << "testing" << endl;
			Action temp = retrace.top();
			retrace.pop();
			if (temp == TURN_LEFT)
			{
				updateLeft();
			}
			else if (temp == TURN_RIGHT)
			{
				updateRight();
			}
			else if (temp == FORWARD)
			{
				updatePos();
			}


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
			foundGold = true;
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
			//retrace.push(FORWARD);
			addGoMap();

			//updatePos();
			//return FORWARD;
			Action temp = explore();
			return temp;
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

			addGoMap();
			retrace.push(FORWARD);

			updatePos();
			return FORWARD;
		}

		if (wumpusFound || wumpusDead)
		{
			if (breeze)
			{
				cout << "theres a breeze" << endl;
				goBack = true;
				retrace.push(TURN_LEFT);
				updateLeft();
				test.first = x;
				test.second = y;
				return TURN_LEFT;
			}

			if (bump)
			{
				cout << "Hit wall" << endl;
				updateBump();
				retrace.push(TURN_RIGHT);
				moves++;
				updateLeft();
				return TURN_LEFT;
			}

			if (!breeze)
			{
				cout << "Moved once" << endl;
				//retrace.push(FORWARD);
				moves++;
				addGoMap();


				Action temp = explore();
				return temp;

			}
		}
		else
		{
			if (stench || breeze)
			{
				cout << "theres a breeze or stench" << endl;
				goBack = true;
				retrace.push(TURN_LEFT);
				updateLeft();
				test.first = x;
				test.second = y;
				return TURN_LEFT;
			}

			if (bump)
			{
				cout << "Hit wall" << endl;
				updateBump();
				retrace.push(TURN_RIGHT);
				moves++;
				updateLeft();
				return TURN_LEFT;
			}

			if (!stench && !breeze)
			{
				cout << "Moved once" << endl;
				//retrace.push(FORWARD);
				moves++;
				addGoMap();

				Action temp = explore();
				return temp;



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
		temp.second = y;
		if (checkAvoid(temp))
			goMap.push_back(temp);
	}
	else if (x == xLim && xLim > -1)
	{
		temp.first = x - 1;
		temp.second = y;
		if (checkAvoid(temp))
			goMap.push_back(temp);
	}
	else
	{
		temp.first = x + 1;
		temp.second = y;
		if (checkAvoid(temp))
			goMap.push_back(temp);

		temp.first = x - 1;
		temp.second = y;
		if (checkAvoid(temp))
			goMap.push_back(temp);
	}

	if (y == 1)
	{
		temp.first = x;
		temp.second = y + 1;
		if (checkAvoid(temp))
			goMap.push_back(temp);
	}
	else if (y == yLim && yLim > -1)
	{
		temp.first = x;
		temp.second = y - 1;
		if (checkAvoid(temp))
			goMap.push_back(temp);
	}
	else
	{
		temp.first = x;
		temp.second = y + 1;
		if (checkAvoid(temp))
			goMap.push_back(temp);

		temp.first = x;
		temp.second = y - 1;
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

void MyAI::updatePos()
{
	if (dir == 1)
	{
		y++;
	}
	else if (dir == 2)
	{
		x++;
	}
	else if (dir == 3)
	{
		y--;
	}
	else if (dir == 4)
	{
		x--;
	}
}

void MyAI::updateLeft()
{
	if (dir == 1)
	{
		dir = 4;
	}
	else if (dir == 2)
	{
		dir = 1;
	}
	else if (dir == 3)
	{
		dir = 2;
	}
	else if (dir == 4)
	{
		dir = 3;
	}
}

void MyAI::updateRight()
{
	if (dir == 1)
	{
		dir = 2;
	}
	else if (dir == 2)
	{
		dir = 3;
	}
	else if (dir == 3)
	{
		dir = 4;
	}
	else if (dir == 4)
	{
		dir = 1;
	}
}

void MyAI::updateBump()
{
	if (dir == 1)
	{
		y--;
		yLim = y;
	}
	else if (dir == 2)
	{
		x--;
		xLim = x;
	}
	
	vector< pair<int, int>>::iterator it;

	for (it = goMap.begin(); it != goMap.end(); it++)
	{
		if (xLim > -1)
		{
			if (it->first > xLim)
			{
				goMap.erase(it);
			}
		}

		if (yLim > -1)
		{
			if (it->second > yLim)
			{
				goMap.erase(it);
			}
		}
	}

}

bool MyAI::canExplore()
{
	for (auto& element : goMap)
	{
		if (abs(element.first - x) == 1)
		{
			return true;
		}

		if (abs(element.second - y) == 1)
		{
			return true;
		}

	}

	return false;
}

Agent::Action MyAI::explore()
{
	vector< pair<int, int>>::iterator it;
	for (it = goMap.begin(); it != goMap.end(); it++)
	{
		if (it->first - x == 1)
		{
			goMap.erase(it);
			Action temp = moveRight();
			return temp;
		}
		else if (x - it->first == 1)
		{
			goMap.erase(it);
			Action temp = moveLeft();
			return temp;
		}

		if (it->second - y == 1)
		{
			goMap.erase(it);
			Action temp = moveUp();
			return temp;
		}
		else if (y - it->second == 1)
		{
			goMap.erase(it);
			Action temp = moveDown();
			return temp;
		}
	}

	//turn 180 and start goBack if cannot make move (might have to use Queue)
	goBack == true;
	retrace.push(TURN_LEFT);
	updateLeft();
	return TURN_LEFT;
}

Agent::Action MyAI::moveUp()
{
	doStuff = true;
	y++;

	if (dir == 1)
	{
		doStuff = false;
		//stuff.push(FORWARD);
		retrace.push(FORWARD);
		return FORWARD;
	}
	else if (dir == 2)
	{
		dir = 1;
		//stuff.push(TURN_LEFT);
		stuff.push(FORWARD);

		retrace.push(TURN_RIGHT);
		return TURN_LEFT;

	}
	else if (dir == 3)
	{
		dir = 1;
		//stuff.push(TURN_LEFT);
		stuff.push(TURN_LEFT);
		stuff.push(FORWARD);

		retrace.push(TURN_RIGHT);
		return TURN_LEFT;

	}
	else if (dir == 4)
	{
		dir = 1;
		//stuff.push(TURN_RIGHT);
		stuff.push(FORWARD);

		retrace.push(TURN_LEFT);
		return TURN_RIGHT;

	}

	
}
Agent::Action MyAI::moveDown()
{
	doStuff = true;
	y--;
	

	if (dir == 1)
	{
		dir = 3;
		//stuff.push(TURN_LEFT);
		stuff.push(TURN_LEFT);
		stuff.push(FORWARD);

		retrace.push(TURN_RIGHT);
		return TURN_LEFT;

	}
	else if (dir == 2)
	{
		dir = 3;
		//stuff.push(TURN_RIGHT);
		stuff.push(FORWARD);

		retrace.push(TURN_LEFT);
		return TURN_RIGHT;

	}
	else if (dir == 3)
	{
		doStuff = false;
		//stuff.push(FORWARD);
		retrace.push(FORWARD);
		return FORWARD;

	}
	else if (dir == 4)
	{
		dir = 3;
		//stuff.push(TURN_LEFT);
		stuff.push(FORWARD);

		retrace.push(TURN_RIGHT);
		return TURN_LEFT;
	}

	
}
Agent::Action MyAI::moveLeft()
{
	doStuff = true;
	x--;
	
	if (dir == 1)
	{
		dir = 4;
		//stuff.push(TURN_LEFT);
		stuff.push(FORWARD);

		retrace.push(TURN_RIGHT);
		return TURN_LEFT;

	}
	else if (dir == 2)
	{
		dir = 4;
		//stuff.push(TURN_LEFT);
		stuff.push(TURN_LEFT);
		stuff.push(FORWARD);

		retrace.push(TURN_RIGHT);
		return TURN_LEFT;

	}
	else if (dir == 3)
	{
		dir = 4;
		//stuff.push(TURN_RIGHT);
		stuff.push(FORWARD);

		retrace.push(TURN_LEFT);
		return TURN_RIGHT;

	}
	else if (dir == 4)
	{
		doStuff = false;
		//stuff.push(FORWARD);
		retrace.push(FORWARD);
		return FORWARD;

	}

	
}
Agent::Action MyAI::moveRight()
{
	doStuff = true;
	x++;
	

	if (dir == 1)
	{
		dir = 2;
		//stuff.push(TURN_RIGHT);
		stuff.push(FORWARD);

		retrace.push(TURN_LEFT);
		return TURN_RIGHT;

	}
	else if (dir == 2)
	{
		doStuff = false;
		//stuff.push(FORWARD);
		retrace.push(FORWARD);
		return FORWARD;

	}
	else if (dir == 3)
	{
		dir = 2;
		//stuff.push(TURN_LEFT);
		stuff.push(FORWARD);

		retrace.push(TURN_RIGHT);
		return TURN_LEFT;

	}
	else if (dir == 4)
	{
		dir = 2;
		//stuff.push(TURN_LEFT);
		stuff.push(TURN_LEFT);
		stuff.push(FORWARD);

		retrace.push(TURN_RIGHT);
		return TURN_LEFT;

	}

	
}

// ======================================================================
// YOUR CODE ENDS
// ======================================================================