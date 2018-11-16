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

		if ((moves > 1) &&stench || breeze) //revise
		{
			auto chkS = safe.find(xPos); //insert new safe position into list
			if(chkS != safe.end()){
				if(chkS->second != yPos)
					safe.insert(pair<int, int> (xPos, yPos));
			}
			
			map<int, int>::iterator it;
			
			if(explore.count(xPos) == 1){ //remove tile from explore list
				auto exD = explore.find(xPos);
				if(exD != explore.end() && exD->second == yPos)
					explore.erase(xPos);
			}
			else{
				auto const& exD = explore.equal_range(xPos);
				for(it = exD.first; it != exD.second; it++)
					if(it->second == yPos)
						explore.erase(it);
			}
			
			
			if(stench){ //working on this - Murphy
			
			}
			if(breeze){ //working on this - Murphy
			
			}
			
			if(explore.size == 0){
				goBack = true;
				retrace.push(TURN_LEFT);
				return TURN_LEFT;
			}
			else{
				
				//need to check explore list for nearby tiles to go to
				
				
				//else
				if(dir == 0) //180 degrees turn
					dir = 2;
				if(dir == 1)
					dir = 3;
				if(dir == 2)
					dir = 0;
				if(dir == 3)
					dir = 1;
			
				
				//need to have it turn 180 degrees and return forward
				//I am stuck trying to figure out this part - Murphy
			
			
			}
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
			safe.insert(pair<int, int>(xPos, yPos));//add to save list
			
			map<int, int>::iterator itr;
			
			if (moves != 0){			//take position out of explore list
				if(explore.count(xPos == 1){
					auto exDel = explore.find(xPos);
					if(exDel != explore.end())
						if(exDel-> second == yPos)
							explore.erase(xPos);
				}
				else{
					auto const& exDel = explore.equal_range(xPos);
					for(itr = exDel.first; itr != exDel.second; itr++){
						if(itr->second == yPos)
							explore.erase(itr);
					}
				}
			}
			
			if(yPos != yLim)		//get surrounding tiles
				testPos.insert(pair<int, int>(xPos, (yPos + 1)));
			if(yPos != 0)
				testPos.insert(pair<int, int>(xPos, (yPos - 1)));
			if(xPos != xLim)
				testPos.insert(pair<int, int>((xPos + 1), yPos));
			if(xPos != 0)
				testPos.insert(pair<int, int>((xPos - 1), yPos));
				
			for(itr = safe.begin(); itr != safe.end();itr++){//take out test list from safe list
				auto itrS = testPos.find(itr->first);
				
				if(itrS != testPos.end()){
					if(itrS->second == itr->second)
						testPos.erase(itrS);
				}
			}
			
			if(explore.size() == 0){	//add test list to explore list
				explore.insert(testPos.begin(), testPos.end());
			}
			else{
				for(itr = testPos.begin(); itr != testPos.end(); itr++){
					auto itrE = explore.find(itr->first);
					
					if(itrE == explore.end()){
						explore.insert(pair<int, int> (itr->first, itr->second));
					}
					else{
						if(itrE->second == itr->second)
							explore.insert(pair<int, int>(itr->first, itr->second));
					}
				}
			}
			
			testPos.clear();
			
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
