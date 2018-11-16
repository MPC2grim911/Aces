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
			checkSafe(xPos, yPos, safe); //add current positon to safe list
			exShorten(xPos, yPos, explore);//remove position from explore list
			
			surTiles(xPos, yPos, xLim, yLim, safe, testPos); //get surrounding unknowns
			
			map<int, int>::iterator it;
			
			
			if(stench && breeze){ //work on this
				
				if(unknownPit.size() == 0)
					addOnly(unknownPit, testPos);
				else{
					//self checking function: if overlays, take out position and add to pit list
				}
				
				if(unknownWump.size() == 0)
					addOnly(unknownWump, testPos);
				else{
					//self checking function: if overlays, update wumpus
				}
				
				//check unknown wumpus with known& unknown pits: 
					//if overlays, take out position from both unknown lists and add to explore list
			
			}
			else if(stench){ //working on this
				if(unknownWump.size() == 0)
					addOnly(unknownWump, testPos);
				else{
					//self checking function
				}
				
				//check unknown wumpus with known & unknown pits
			}
			else if(breeze){ //working on this
				if(unknownPit.size() == 0)
					addOnly(unknownPit, testPos);
				else{
					//self checking function
				}
				//check unknown pits with unknown wumpus
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
				dir = 0;
			}
			return TURN_LEFT;
		}

		if (!stench && !breeze)
		{
			checkSafe(xPos, yPos, safe);//add to save list
			
			map<int, int>::iterator itr;
			
			if (moves != 0)			//take position out of explore list
				exShorten(xPos, yPos, explore);
			
			surTiles(xPos, yPos, xLim, yLim, safe, testPos);
			
			addOnly(explore, testPos);//add test list to explore list
			
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
void MyAI::checkSafe(int x, int y, multimap<int, int> &s){ //insert position into safe list
	if(s.size() == 0)
		s.insert(pair<int, int> (x, y));
	else{
		auto chkS = s.find(x); 
		if(chkS != s.end()){
			if(chkS->second != y)
				s.insert(pair<int, int> (x, y));
		}
	}
	return;
}

void MyAI::exShorten(int x, int y, multimap<int, int> &e){ //takes position out of the explore list
	if(e.count(x == 1){
		auto exDel = e.find(x);
		if(exDel != e.end())
			if(exDel-> second == y)
				e.erase(x);
	}
	else{
		auto const& exDel = e.equal_range(x);
		
		map<int, int>::iterator it;
		for(it = exDel.first; it != exDel.second; it++){
			if(it->second == y)
				e.erase(it);
		}
	}
	return;
}

void MyAI::surTiles(int x, int y, int xL, int yL, multimap<int, int> &s, multimap<int, int> &t){
	
	if(y != yL)		//get surrounding tiles
		t.insert(pair<int, int>(x, (y + 1)));
	if(y != 0)
		t.insert(pair<int, int>(x, (y - 1)));
	if(x != xL)
		t.insert(pair<int, int>((x + 1), y));
	if(x != 0)
		t.insert(pair<int, int>((x - 1), y));
				
	map <int, int>::iterator surIt;
	for(surIt = s.begin(); surIT != s.end(); surIt++){//take out test list from safe list
		auto itrS = t.find(surIt->first);
				
		if(itrS != t.end()){
			if(itrS->second == surIt->second)
				t.erase(itrS);
		}
	}
	
	return;
}

void MyAI::addOnly(multimap<int, int> &m, multimap<int, int> &t){ //unknown surroundings to specified list

	if(m.size() == 0){	
		m.insert(t.begin(), t.end());
	}
	else{
	 	map <int, int>::iterator itA;
		for(itA = t.begin(); itA != t.end(); itA++){
			auto itr = m.find(itA->first);
					
			if(itr == m.end()){
				m.insert(pair<int, int> (itA->first, itA->second));
			}
			else{
				if(itr->second == itA->second)
					m.insert(pair<int, int>(itA->first, itA->second));
			}
		}
	}
	return;
}
// ======================================================================
// YOUR CODE ENDS
// ======================================================================
