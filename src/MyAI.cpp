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
#include <iterator>

using namespace std;


MyAI::MyAI() : Agent()
{
	// ======================================================================
	// YOUR CODE BEGINS
	// ======================================================================
	goBack = false;
	moves = 0;
	arrowShot = false;
	turnAround = false;
	turnCount = 0;
	
	xPos = 0; //player initial position, change if needed
	yPos = 0;
	dir = 0; //0 is E, 1 is N, 2 is W, 3 is S at the moment
	
	xWall = false; // true when bump into wall and establishes limits
	yWall = false;
	xLim = -1; //changed when bump
	yLim = -1;
	
	int xWump = -1; //changed when wumpus is found and confirmed
	int yWump = -1;
	wumpus = false;
	
	target = false;
	oneMv = false;
	hlfTurn = false;
	xDest = -1;
	yDest = -1;
	
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
	
	if(oneMv){ //if the explore point is in immediate area, then don't back out
	
	}
	
	if(target){ //setup target route
		if (turnAround) //turn around and take a step back
		{
			if (turnCount == 1)
			{
				turnCount == 0;
				turnAround = false;
				retrace.push(FORWARD);
				return FORWARD;
			}

			turnCount++;
			retrace.push(TURN_LEFT);
			retrace.push(TURN_LEFT);
			return TURN_LEFT;
		}
		
		//find safe route to xDest and yDest 
		 
		//Andre this is your section
	}
	
	

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


		
		if(scream)
			wumpus = true;

		if (arrowShot && !wumpus && moves == 1) //we know where the wumpus is if we shoot arrow on first turn and it does not die
		{
			xWump = 0;
			yWump = 1;
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
			else if(dir == 1){
				yPos -= 1;
				if(!yWall){
					yLim = yPos;
					yWall = true;
				}
				dir = 2;
			}
			else if(dir == 2){
				xPos = 0;
				dir = 3;
			}
			else if(dir == 3){
				yPos = 0;
				dir = 0;
			}
			return TURN_LEFT;
		}

		//cout << xPos << ", " << yPos << endl << dir << endl;

		if (moves == 0) //first block only
		{
			if (breeze)
			{
				return CLIMB;
			}
			else if (stench)
			{
				moves++;
				arrowShot = true; //added a new boolean to determine if we shot the arrow yet or not
				return SHOOT;
			}
			
			checkSafe(xPos, yPos, safe);//add to save list
			
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
		
		if (stench || breeze) //if sense a stench or breeze
		{
			checkSafe(xPos, yPos, safe); //add current positon to safe list
			exShorten(xPos, yPos, explore);//remove position from explore list

			surTiles(xPos, yPos, xLim, yLim, safe, testPos); //get surrounding unknowns

			if (stench && breeze) { //if there is both a stench and breeze

				compSelf(unknownWump, wump, testPos);//self checking function: if overlays, update wumpus
				compSelf(unknownPit, pits, testPos);//self checking function: pits version 
				exDelSB(explore, testPos); //takes out surrounding area from explore list
			}
			else if (stench) { //if there is only a stench

				compSelf(unknownWump, wump, testPos);
				exDelSB(explore, testPos);

				wCheckP(explore, unknownWump, unknownPit, pits, testPos);//check unknown wumpus with known & unknown pits
			}
			else if (breeze) { //if there is only a breeze

				compSelf(unknownPit, pits, testPos);
				exDelSB(explore, testPos);

				if (!wumpus)//if wumpus is not dead yet
					pCheckW(explore, unknownPit, unknownWump, testPos);//check unknown pits with unknown wumpus
			}

			testPos.clear();

			if (!wumpus) {//if wumpus is not dead
				if (unknownWump.size() == 1 || wump.size() == 1) { //if unknownWump list or wump list is size 1
					wumpus = true;
					map<int, int>::iterator it;

					if (wump.size() == 0) {		//getting wumpus location
						it = unknownWump.begin();
						xWump = it->first;
						yWump = it->second;
					}
					else {
						it = wump.begin();
						xWump = it->first;
						yWump = it->second;
					}
				}
			}


			if (explore.size() == 0) { 	//how the agent should move
				goBack = true;
				retrace.push(TURN_LEFT);
				return TURN_LEFT;
			}
			else {

				//need to check explore list for nearby tiles to go to

				//if find explore tile next to us, then go to it
				bool next = false;
				target = getTarget(xPos, yPos, explore, xDest, yDest, next); //gets target
						
				if (next) //if destination is right next to current tile
				{
					oneMv = true;
					if(xPos == xDest){ //if destination is above or below current tile
						if((yDest - yPos) == 1){
							if(dir == 0){
								dir = 1;
								return TURN_LEFT;
							}
							else if(dir == 1){
								oneMv = false;
								return FORWARD;
							}
							else if(dir == 2){
								dir = 1;
								return TURN_RIGHT;
							}
							else if(dir == 3){
								hlfTurn = true;
								dir = 1;
								return TURN_RIGHT;
							}
						}
						if(dir == 0){
							dir = 3;
							return TURN_RIGHT;
						}
						else if(dir == 1){
							hlfTurn = true;
							dir = 3;
							return TURN_RIGHT;
						}
						else if(dir == 2){
							dir = 3;
							return TURN_LEFT;
						}
						else if(dir == 3){
							oneMv = false;
							return FORWARD;
						}
					}
					if(yPos == yDest){ //if destination is to the left or right of tile
						if((xDest - xPos) == 1){
							if(dir == 0){
								oneMv = false;
								return FORWARD;
							}
							else if(dir == 1){
								dir = 0;
								return TURN_RIGHT;
							}
							else if(dir == 2){
								hlfTurn = true;
								dir = 0;
								return TURN_RIGHT;
							}
							else if(dir == 3){
								dir = 0;
								return TURN_LEFT;
							}
						}
						if(dir == 0){
							hlfTurn = true;
							dir = 2;
							return TURN_RIGHT;
						}
						else if(dir == 1){
							dir = 2;
							return TURN_LEFT;
						}
						else if(dir == 2){
							oneMv = false;
							return FORWARD;
						}
						else if(dir == 3){
							dir = 2;
							return TURN_RIGHT;
						}
					}
				}
				
				//else, if the destination is no where near take a step back and search for target 
				if (dir == 0){ //180 degrees turn
					dir = 2;
					xPos -= 1;
				}
				else if (dir == 1){
					dir = 3;
					yPos -= 1;
				}
				else if (dir == 2){
					dir = 0;
					xPos += 1;
				}
				else if (dir == 3){
					dir = 1;
					yPos += 1;
				}

				turnAround = true;
				return TURN_LEFT;
			}
		}
		else { //for safe tiles
			checkSafe(xPos, yPos, safe);//add to save list

			if (moves != 0)			//take position out of explore list
				exShorten(xPos, yPos, explore);

			surTiles(xPos, yPos, xLim, yLim, safe, testPos);

			addOnly(explore, testPos);//add test list to explore list

			testPos.clear();

			retrace.push(FORWARD);
			moves++;
			if (dir == 0)//player position changes
				xPos += 1;
			if (dir == 1)
				yPos += 1;
			if (dir == 2)
				xPos -= 1;
			if (dir == 3)
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
	if(e.count(x) == 1){
		auto exDel = e.find(x);
		if(exDel != e.end())
			if(exDel-> second == y)
				e.erase(x);
	}
	else{
		auto const& exDel = e.equal_range(x);
		
		map<int, int>::iterator it;
		for(it = exDel.first; it != exDel.second; it++){
			if(it->second == y){
				e.erase(it);
				break;
			}
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
				
	typename map <int, int>::iterator surIt;
	for(surIt = s.begin(); surIt != s.end(); surIt++){//take out test list from safe list
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

void MyAI::compSelf(multimap<int, int> &m, multimap<int, int> &m2, multimap<int, int> &t){ //checks if point overlays at any point

	if(m.size() == 0){	
		m.insert(t.begin(), t.end());
	}
	else{
	 	map <int, int>::iterator itC;
		for(itC = t.begin(); itC != t.end(); itC++){
			auto itr = m.find(itC->first);
					
			if(itr == m.end()){
				m.insert(pair<int, int> (itC->first, itC->second)); 
			}
			else{
				if(itr->second == itC->second){
					m2.insert(pair<int, int>(itC->first, itC->second)); //add point to known list
					m.erase(itr);	//take out point from unknown list
				}
			}
		}
	}
	return;
}

void MyAI::exDelSB(multimap<int, int> &e, multimap<int, int> &t){ //takes unknown tile locations out of explore list
	map<int, int>::iterator itE;
	for(itE = t.begin(); itE != t.end(); itE++){
		if(e.count(itE->first) == 1){
			auto exDel = e.find(itE->first);
			if(exDel != e.end())
				if(exDel-> second == itE->second)
					e.erase(exDel);
		}
		else{
			auto const& exDel = e.equal_range(itE->first);
		
			map<int, int>::iterator it;
			for(it = exDel.first; it != exDel.second; it++){
				if(it->second == itE->second){
					e.erase(it);
					break;	
				}
			}
		}
	}
	return;
}

//checks unknown wumpus to known and unknown pits and adds to explore
void MyAI::wCheckP(multimap<int, int> &e, multimap<int, int> &w, multimap<int, int> &p, multimap<int, int> &p2, multimap<int, int> &t){ 
	map<int,int>::iterator it;
	map<int,int>::iterator itr;
	map<int,int>::iterator i;
	
	if(p2.size() != 0){ 	//takes out known pits from wumpus list only
		for(it = t.begin(); it != t.end(); it++){
			auto const& wDel = p2.equal_range(it->first);
		
			for(itr = wDel.first; itr != wDel.second; itr++){
				if(itr->second == it->second){
					auto const& wump = w.equal_range(it->first);
		
					for(i = wump.first; i != wump.second; i++){
						if(i->second == it->second){
							w.erase(i);
							break;
						}
					}
				}
			}
		}
	}
	
	//takes out overlapping points from wumpus and unknown pit list and adds to explore list
	for(it = t.begin(); it != t.end(); it++){
		auto const& wComp = p.equal_range(it->first);
		
		for(itr = wComp.first; itr != wComp.second; itr++){
			if(itr->second == it->second){
				p.erase(itr);
				e.insert(pair<int, int>(it->first, it->second));
				
				auto const& wump = w.equal_range(it->first);
				for(i = wump.first; i != wump.second; i++){
					if(i->second == it->second){
						w.erase(i);
						break;
					}
				}
			}
		}
	}
	
	return;
}

//checks unknown pits to unknown wumpus and adds to explore
void MyAI::pCheckW(multimap<int, int> &e, multimap<int, int> &p, multimap<int, int> &w, multimap<int, int> &t){ 
	map<int,int>::iterator it;
	map<int,int>::iterator itr;
	map<int,int>::iterator i;
	
	for(it = t.begin(); it != t.end(); it++){
		auto const& pComp = w.equal_range(it->first);
		
		for(itr = pComp.first; itr != pComp.second; itr++){
			if(itr->second == it->second){
				w.erase(itr);
				e.insert(pair<int, int>(it->first, it->second));
				
				auto const& pit = p.equal_range(it->first);
				for(i = pit.first; i != pit.second; i++){
					if(i->second == it->second){
						p.erase(i);
						break;
					}
				}
			}
		}
	}
	
	return;
}

bool MyAI::getTarget(int x, int y, multimap<int, int> e, int& xD, int& yD, bool& next){
	
	map<int, int>::iterator itT;

	int dist = 1000;
		
	for(itT = e.begin(); itT != e.end(); itT++){
		int d = abs(itT->first - x) + abs(itT->second -y);
		
		if(d < dist){
			xD = itT->first;
			yD = itT->first;
			dist = d;
		}
	}
	
	if(dist == 1){
		n = true;
		return false;
	}
	return true;
}

// ======================================================================
// YOUR CODE ENDS
// ======================================================================
