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

	//cout << x << ", " << y << ", " << dir << endl;
	
	if (moves > 400)
	{
		//cout << "lost" << endl;

	}
	
	if (doStuff)
	{
		//cout << "doing stuff" << endl;
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
				//cout << "turned left" << endl;
				updateLeft();
				retrace.push(TURN_RIGHT);
			}
			else if (temp == TURN_RIGHT)
			{
				//cout << "turned right" << endl;
				updateRight();
				retrace.push(TURN_LEFT);
			}
			else if (temp == FORWARD)
			{
				//cout << "moved forward" << endl;
				retrace.push(FORWARD);
			}
			return temp;
		}

		//cout << "doStuff done" << endl;
	}
	

	if (goBack)
	{
		if (canExplore() && !foundGold && !(test.first == x && test.second == y))
		{
			//cout << "explore more" << endl;
			goBack = false;
			retrace.push(TURN_LEFT);
			retrace.push(TURN_LEFT);
			Action temp = explore();
			return temp;
		}

		if (!retrace.empty())
		{
			//cout << "going back" << endl;
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

		//cout << " got Error" << endl;
	}
	else
	{
		//cout << "check glitter" << endl;
		if (glitter)
		{
			//cout << "Found gold" << endl;
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
				//cout << "I'm out first turn" << endl;
				return CLIMB;
			}
			else if (stench)
			{
			//	cout << "shot an arrow" << endl;
				moves++;
				arrowShot = true;
				return SHOOT;
			}

			//cout << "first turn move" << endl;
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
			//cout << "killed wumpus" << endl;
			wumpusDead = true;
		}

		if (arrowShot && !wumpusDead && moves == 1)
		{
			//cout << "found wumpus" << endl;
			wumpusFound = true;
			temp.first = 1;
			temp.second = 2;
			avoidMap.push_back(temp);
			
			moves++;

			addGoMap();
			removeDanger();
			//retrace.push(FORWARD);

			//updatePos();
			//return FORWARD;
			Action temp = explore();
			return temp;
		}

		if (wumpusFound || wumpusDead)
		{
			if (breeze)
			{
				//cout << "theres a breeze" << endl;
				goBack = true;
				retrace.push(TURN_LEFT);
				updateLeft();
				test.first = x;
				test.second = y;
				return TURN_LEFT;
			}

			if (bump)
			{
				//cout << "Hit wall" << endl;
				updateBump();
				//cout << "test1" << endl;
				retrace.push(TURN_RIGHT);
				//cout << "test2" << endl;
				moves++;
				updateLeft();
			//	cout << "test3" << endl;
				return TURN_LEFT;
			}

			if (!breeze)
			{
			//	cout << "Move" << endl;
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
			//	cout << "theres a breeze or stench" << endl;
				goBack = true;
				retrace.push(TURN_LEFT);
				updateLeft();
				test.first = x;
				test.second = y;
				return TURN_LEFT;
			}

			if (bump)
			{
				//cout << "Hit wall" << endl;
				updateBump();
			//	cout << "test4" << endl;
				retrace.push(TURN_RIGHT);
			//	cout << "test5" << endl;
				moves++;
				updateLeft();
			//	cout << "test6" << endl;
				return TURN_LEFT;
			}

			if (!stench && !breeze)
			{
			//	cout << "Move" << endl;
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
		{
			//cout << "added -1" << endl;
			goMap.push_back(temp);
		}
			
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
	if (test.first == 1 && test.second == 1)
	{
		//cout << "dont add 1st spot" << endl;
		return false;
	}
	for (auto& element : avoidMap)
	{
		if (test == element)
		{
			return false;
		}
	}

	for (auto& element : goMap)
	{
		if (test == element)
		{
		//	cout << "no duplicates" << endl;
			return false;
		}
	}

	for (auto& element : explored)
	{
		if (test == element)
		{
		//	cout << "already been here" << endl;
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
	//cout << "break 1" << endl;
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
	
	//cout << "break 2" << endl;
	vector< pair<int, int>>::iterator it;

	//cout << "break 3" << endl;
	for (it = goMap.begin(); it != goMap.end();)
	{
		/*
		size_t zero = 0;
		if (goMap.size() != zero)
		{
			if (xLim > -1)
			{
				if (it->first > xLim)
				{
					goMap.erase(it);
				}
			}

			cout << "break 5" << endl;
			if (yLim > -1)
			{
				if (it->second > yLim)
				{
					goMap.erase(it);
				}
			}
		}
		*/
		
		if (xLim > -1)
		{
			if (it->first > xLim)
			{
				goMap.erase(it);
			}
			else
			{
				it++;
			}
		}

	//	cout << "break 5" << endl;
		if (yLim > -1)
		{
			if (it->second > yLim)
			{
				goMap.erase(it);
			}
			else
			{
				it++;
			}
		}
		
	}
	//cout << "break 6" << endl;
}

bool MyAI::canExplore()
{
	for (auto& element : goMap)
	{
		if ( (abs(element.first - x) == 1) && element.second == y)
		{
			return true;
		}

		if ( (abs(element.second - y) == 1) && element.first == x)
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
		if (it->first - x == 1 && it->second == y)
		{
		//	cout << "move right, " << dir << endl;
			temp.first = it->first;
			temp.second = it->second;
			explored.push_back(temp);
			goMap.erase(it);
			
			Action temp = moveRight();
			return temp;
		}
		else if (x - it->first == 1 && it->second == y)
		{
		//	cout << "move left, " << dir << endl;
			temp.first = it->first;
			temp.second = it->second;
			explored.push_back(temp);
			goMap.erase(it);
			
			Action temp = moveLeft();
			return temp;
		}

		if (it->second - y == 1 && it->first == x)
		{
			//cout << "move up, "<< dir << endl;
			temp.first = it->first;
			temp.second = it->second;
			explored.push_back(temp);
			goMap.erase(it);
			Action temp = moveUp();
			return temp;
		}
		else if (y - it->second == 1 && it->first == x)
		{
			//cout << "move down, " << dir << endl;
			temp.first = it->first;
			temp.second = it->second;
			explored.push_back(temp);
			goMap.erase(it);
			Action temp = moveDown();
			return temp;
		}
	}

	

	//turn 180 and start goBack if cannot make move (might have to use Queue)
	goBack = true;
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
		//dir = 1;
		//stuff.push(TURN_LEFT);
		stuff.push(FORWARD);

		retrace.push(TURN_RIGHT);
		updateLeft();
		return TURN_LEFT;

	}
	else if (dir == 3)
	{
		//dir = 1;
		//stuff.push(TURN_LEFT);
		stuff.push(TURN_LEFT);
		stuff.push(FORWARD);

		retrace.push(TURN_RIGHT);
		updateLeft();
		return TURN_LEFT;

	}
	else if (dir == 4)
	{
		//dir = 1;
		//stuff.push(TURN_RIGHT);
		stuff.push(FORWARD);

		retrace.push(TURN_LEFT);
		updateRight();
		return TURN_RIGHT;

	}

	
}
Agent::Action MyAI::moveDown()
{
	doStuff = true;
	y--;
	

	if (dir == 1)
	{
		//dir = 3;
		//stuff.push(TURN_LEFT);
		stuff.push(TURN_LEFT);
		stuff.push(FORWARD);

		retrace.push(TURN_RIGHT);

		updateLeft();
		return TURN_LEFT;

	}
	else if (dir == 2)
	{
		//dir = 3;
		//stuff.push(TURN_RIGHT);
		stuff.push(FORWARD);

		retrace.push(TURN_LEFT);

		updateRight();
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
		//dir = 3;
		//stuff.push(TURN_LEFT);
		stuff.push(FORWARD);

		retrace.push(TURN_RIGHT);
		updateLeft();
		return TURN_LEFT;
	}

	
}
Agent::Action MyAI::moveLeft()
{
	doStuff = true;
	x--;
	
	if (dir == 1)
	{
		//dir = 4;
		//stuff.push(TURN_LEFT);
		stuff.push(FORWARD);

		retrace.push(TURN_RIGHT);
		updateLeft();
		return TURN_LEFT;

	}
	else if (dir == 2)
	{
		//dir = 4;
		//stuff.push(TURN_LEFT);
		stuff.push(TURN_LEFT);
		stuff.push(FORWARD);

		retrace.push(TURN_RIGHT);
		updateLeft();
		return TURN_LEFT;

	}
	else if (dir == 3)
	{
		//dir = 4;
		//stuff.push(TURN_RIGHT);
		stuff.push(FORWARD);

		retrace.push(TURN_LEFT);
		updateRight();
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
		//dir = 2;
		//stuff.push(TURN_RIGHT);
		stuff.push(FORWARD);

		retrace.push(TURN_LEFT);
		updateRight();
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
		//dir = 2;
		//stuff.push(TURN_LEFT);
		stuff.push(FORWARD);

		retrace.push(TURN_RIGHT);
		updateLeft();
		return TURN_LEFT;

	}
	else if (dir == 4)
	{
		//dir = 2;
		//stuff.push(TURN_LEFT);
		stuff.push(TURN_LEFT);
		stuff.push(FORWARD);

		retrace.push(TURN_RIGHT);
		updateLeft();
		return TURN_LEFT;

	}

	
}

void MyAI::removeDanger()
{
	vector< pair<int, int>>::iterator it;
	vector< pair<int, int>>::iterator it2;

	for (it = avoidMap.begin(); it != avoidMap.end(); it++)
	{
		for (it2 = goMap.begin(); it2 != goMap.end(); )
		{
			if (it->first == it2->first && it->second == it2->second)
			{
				goMap.erase(it2);
			}
			else
			{
				it2++;
			}
		}
	}

	
}
// ======================================================================
// YOUR CODE ENDS
// ======================================================================