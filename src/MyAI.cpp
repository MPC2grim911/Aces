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
	
	Pos.x = 0; //player initial position, change if needed
	Pos.y = 0;
	dir = 0; //0 is E, 1 is N, 2 is W, 3 is S at the moment
	
	xWall = false; // true when bump into wall and establishes limits
	yWall = false;
	xLim = -1; //changed when bump
	yLim = -1;
	
	vector<Point> safe;
	vector<Point> testPos;
	vector<Point> explore;
	
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
			safe.push_back(Pos);
			
			if(Pos.y != yLim){
				Point n = Pos;
				n.y += 1;
				testPos.push_back(n);
			}
			if(Pos.y != 0){
				Point s = Pos;
				s.y -= 1;
				testPos.push_back(s);
			}
			if(Pos.x != xLim){
				Point e = Pos;
				e.x += 1;
				testPos.push_back(e);
			}
			if(Pos.x != 0){
				Point w = Pos;
				w.x -= 1;
				testPos.push_back(w);
			}
			
			for(int i = 0; i < testPos.size(); i++){
				for(int j = 0; j < safe.size(); j++){
					if(testPos.at(i) == safe.at(j)){
						testPos.erase(testPos.begin() + i);;
					}
				}
			}
			
			for(int i = 0; i < testPos.size(); i++){
				bool same = false;
				for(int j = -; j < explore.size(); j++){
					if(testPos.at(i) == explore.at(j)){
						same = true;
						break;
					}
				}
				if(!same)
					explore.push_back(testPos.at(i));
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
				
			cout << "Safe points: " << endl;
			for(int i = 0; i < safe.size(); i++){
				Point sf = safe.at(i);
				cout << "\t" << sf.x << ", " << sf.y << endl;
			}
			cout << "Explore points: " << endl;
			for(int j = 0; j < explore.size(); j++){
				Point ex = explore.at(j);
				cout << "\t" << ex.x << ", " << ex.y << endl;
			}
			
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
bool MyAI::Point::&operator==(const Point &p) const{
	return(this->x == p.x) && (this->y == p.y);
}

// ======================================================================
// YOUR CODE ENDS
// ======================================================================
