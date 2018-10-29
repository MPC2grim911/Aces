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

		if (moves == 0 && (stench || breeze))
		{
			cout << "1st turn" << endl;
			return CLIMB;
		}

		if ((moves > 1) || stench || breeze)
		{
			cout << "too many moves" << endl;
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
			cout << moves << endl;
			return FORWARD;
			


		}

		
	}
	
	cout << "climbed out" << endl;
	
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