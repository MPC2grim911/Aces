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
			}
			else if (temp == FORWARD)
			{
				//cout << "moved forward" << endl;
			}
			return temp;
		}

		//cout << "doStuff done" << endl;
	}

	if (goBack)
	{
		if (!retrace.empty())
		{
			//cout << "testing" << endl;
			Action temp = retrace.top();
			retrace.pop();
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
		if (glitter)
		{
			//cout << "Found gold" << endl;
			goBack = true;
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
				//cout << "shot an arrow" << endl;
				moves++;
				arrowShot = true;
				return SHOOT;
			}

			//cout << "first turn move forward" << endl;
			moves++;
			retrace.push(FORWARD);
			return FORWARD;
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
			doStuff = true;
			//stuff.push(TURN_LEFT);
			//retrace.push(TURN_RIGHT);
			moves++;
			stuff.push(FORWARD);
			retrace.push(FORWARD);
			return GRAB;
		}

		if (wumpusFound)
		{
			if (breeze)
			{
				//cout << "too many moves" << endl;
				goBack = true;
				retrace.push(TURN_LEFT);
				return TURN_LEFT;
			}

			if (bump)
			{
				//cout << "Hit wall" << endl;
				retrace.push(TURN_LEFT);
				moves++;
				return TURN_RIGHT;
			}

			if (!breeze)
			{
				//cout << "Moved once" << endl;
				retrace.push(FORWARD);
				moves++;
				//cout << moves << endl;
				return FORWARD;



			}
		}
		else
		{
			if (stench || breeze)
			{
				//cout << "too many moves" << endl;
				goBack = true;
				retrace.push(TURN_LEFT);
				return TURN_LEFT;
			}

			if (bump)
			{
				//cout << "Hit wall" << endl;
				retrace.push(TURN_RIGHT);
				moves++;
				return TURN_LEFT;
			}

			if (!stench && !breeze)
			{
				//cout << "Moved once" << endl;
				retrace.push(FORWARD);
				moves++;
				//cout << moves << endl;
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


// ======================================================================
// YOUR CODE ENDS
// ======================================================================