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
#include <map>

using namespace std;


MyAI::MyAI() : Agent()
{
	// ======================================================================
	// YOUR CODE BEGINS
	// ======================================================================
	goBack = false;
	moves = 0;
	
	xPos = 0; //player initial position, change if needed
	yPos = 0;
	dir = 0; //0 is E, 1 is N, 2 is W, 3 is S at the moment
	
	xWall = false; // true when bump into wall and establishes limits
	yWall = false;
	xLim = -1; //changed when bump
	yLim = -1;
	
	int xWump = -1; //changed when wumpus is found and confirmed
	int yWump = -1;
	
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
			Action temp = retrace.top();
			retrace.pop();
			return temp;
		}
		else
		{
			return CLIMB;
		}
	}
	else
	{
		if (glitter)
		{
			goBack = true;
			retrace.push(TURN_LEFT);
			retrace.push(TURN_LEFT);
			return (GRAB);
		}
		
		//cout << xPos << ", " << yPos << endl << dir << endl;

		if (moves == 0 && (stench || breeze)) //first block only
		{
			return CLIMB;
		}

		if ((moves > 1) || stench || breeze) //revise
		{
			goBack = true;
			if(dir == 0) //180 degrees turn
				dir = 2;
			if(dir == 1)
				dir = 3;
			if(dir == 2)
				dir = 0;
			if(dir == 3)
				dir = 1;
			retrace.push(TURN_LEFT);
			return TURN_LEFT;
		}

		if (bump)
		{
			retrace.push(TURN_RIGHT);
			moves++;
			if(dir == 0){ 		
				xPos -= 1; //to cancel out the FORWARD move position change in navigation
				if(!xWall){//finds max dimensions of the maze
					xLim = xPos;
					xWall = true;
				}
				dir = 1; //left turn directions
			}
			if(dir == 1){
				yPos -= 1;
				if(!yWall){
					yLim = yPos;
					yWall = true;
				}
				dir = 2;
			}
			if(dir == 2){
				xPos = 0;
				dir = 3;
			}
			if(dir == 3){
				yPos = 0;
				dir = 4;
			}
			return TURN_LEFT;
		}

		if (!stench && !breeze)
		{
			retrace.push(FORWARD);
			moves++;
			if(dir == 0)//player position changes
				xPos += 1;
			if(dir == 1)
				yPos += 1;
			if(dir == 2)
				xPos -= 1;
			if(dir == 3)
				yPos -= 1;
			return FORWARD;
		}
	}
	
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
