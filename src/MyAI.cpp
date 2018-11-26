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
	turnHlf = false;
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
	wFound = false;
	hunt = false;
	wHunt = false;
	
	target = false;
	travel = false;
	oneMv = false;
	hlfTurn = false;
	xDest = -1;
	yDest = -1;
	goldFound = false;
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
	
	cout << xPos << ", " << yPos << ", " << dir << endl;
	
	if (glitter) // if find gold
	{
		goBack = true;
		target = true;
		travel = true;
		xDest = 0;
		yDest = 0;
		goldFound = true;
			
		return (GRAB);
	}
	
	if(scream){
		wumpus = true;
		unknownWump.clear();
	}
	if (bump){
			
		if(dir == 0){ 		
			if(!xWall){//finds max dimensions of the maze
				xLim = xPos;
				xWall = true;
			}
			xPos  = xLim - 1; //to cancel out the FORWARD move position change in navigation
			if(yPos == yLim){
				turnRight(dir);
				return TURN_RIGHT;
			}
		}
		else if(dir == 1){
			if(!yWall){
				yLim = yPos;
				yWall = true;
			}
			yPos = yLim - 1;
			if(xPos == 0){
				turnRight(dir);
				return TURN_RIGHT;
			}
		}
		else if(dir == 2){
			xPos = 0;
			if(yPos == 0){
				turnRight(dir);
				return TURN_RIGHT;
			}
		}
		else if(dir == 3){
			yPos = 0;
			if(xPos == xLim){
				turnRight(dir);
				return TURN_RIGHT;
			}
		}
		turnLeft(dir);
		return TURN_LEFT;
	}

	
	if(oneMv){ //if the explore point is in immediate area, then don't back out
		if(hlfTurn){
			hlfTurn = false;
			turnLeft(dir);
			return TURN_RIGHT;
		}
		
		goForward(xPos, yPos, dir);
		oneMv = false;
		return FORWARD;
	}
	
	if(hunt){ //function for going to nearest location near wumpus and shooting wumpus
	
		if(arrowShot){
			hunt = false;
			wHunt = false;
			target = true; //FIXME: Agent goes into a loop after killing wumpus for some reason
			travel = true;
			
			if(wumpus){//sets a target destination from safe to explore
				wumpExp(xPos, yPos, dir, xLim, yLim, xDest, yDest, xWall, yWall, safe, explore, xWump, yWump); 
			}
			else if(!wumpus){
				goBack = true;
				xDest = 0;
				yDest = 0;
			}
		}
		
		if(wHunt) //goes to a location to kill wumpus
			return wumpHunt(xPos, yPos, dir, xLim, yLim, xWall, yWall, safe, xWump, yWump, arrowShot); 
	}
	
	if(target){ //setup target route and go to target
		if(goldFound){
			if(turnCount == 2){
				turnCount == 0;
				goldFound = false;
				goForward(xPos, yPos, dir);
				return FORWARD;
			}
			turnCount++;
			turnLeft(dir);
			return TURN_LEFT;
		}
	
		if (turnHlf) //turn around and take a step back
		{
			if (turnCount == 1)
			{
				turnCount == 0;
				turnAround = false;
				goForward(xPos, yPos, dir);
				return FORWARD;
			}

			turnCount++;
			turnLeft(dir);
			return TURN_LEFT;
		}
		
		if(xPos == xDest && yPos == yDest){ //if reached explore destination
			target = false;
			travel = false;
			prev.clear();
			if((xPos == 0 && yPos == 0) || goBack) //if we are turning back
				return CLIMB;
		}
		if(travel)
			return goToTarget(xPos, yPos, dir, xLim, yLim, xDest, yDest, xWall, yWall, safe, prev);
	}
	

	if (moves == 0) //first block only
	{
		if(breeze){
			return CLIMB;
		}
		else if (stench)
		{
			if(!arrowShot){
				arrowShot = true; //added a new boolean to determine if we shot the arrow yet or not
				return SHOOT;
			}
			if(!wumpus){
				xWump = 0; //if fails to hit wumpus
				yWump = 1;
				wFound = true;
			}
			else{
				explore.insert(pair<int, int> (0, 1));
			}
			safe.insert(pair<int, int>(0,0));
			move++;
			goForward(xPos, yPos, dir);
			return FORWARD;
		}
		
		checkSafe(xPos, yPos, safe);//add to save list
			
		if (moves != 0)			//take position out of explore list
			exShorten(xPos, yPos, explore);
			
		surTiles(xPos, yPos, xLim, yLim, xWall, yWall, safe, testPos);
			
		addOnly(explore, testPos);//add test list to explore list
			
		testPos.clear();
		
		moves++;
		goForward(xPos, yPos, dir);
		return FORWARD;		
	}
		
	if (stench || breeze) //if sense a stench or breeze
	{
		checkSafe(xPos, yPos, safe); //add current positon to safe list
		exShorten(xPos, yPos, explore);//remove position from explore list

		surTiles(xPos, yPos, xLim, yLim, xWall, yWall safe, testPos); //get surrounding unknowns

		if (stench && breeze) { //if there is both a stench and breeze
			if(!wumpus)
				compSelf(unknownWump, wump, testPos);//self checking function: if overlays, update wumpus
			compSelf(unknownPit, pits, testPos);//self checking function: pits version 
			exDelSB(explore, testPos); //takes out surrounding area from explore list
		}
		else if (stench) { //if there is only a stench
			if(!wumpus){
				compSelf(unknownWump, wump, testPos);
				exDelSB(explore, testPos);

				wCheckP(explore, unknownWump, unknownPit, pits, testPos);//check unknown wumpus with known & unknown pits
			}
			if(wumpus)
				addOnly(explore, testPos); //if wumpus is dead, add unknowns into explore
		}
		else if (breeze) { //if there is only a breeze

			compSelf(unknownPit, pits, testPos);
			exDelSB(explore, testPos);

			if (!wumpus && !wFound)//if wumpus is not dead yet and not found yet
				pCheckW(explore, unknownPit, unknownWump, testPos);//check unknown pits with unknown wumpus
		}

		testPos.clear();
		
		if(pits.size() != 0)
			exDelSB(explore, pits); //takes out known pits from explore list

		if (!wumpus && !wFound) {//if wumpus is not dead and not found yet
			if (unknownWump.size() == 1 || wump.size() == 1) { //if unknownWump list or wump list is size 1
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
			unknownWump.clear();
		}
		
		if(wFound){
			if(!wumpus)
				exShorten(xWump, yWump, explore); //take out wumpus point from explore list
			if(explore.size() == 0 && !arrowShot){ //if nothing left to explore, wumpus hunt if arrow has not been shot
				hunt = true;
				wHunt = true;
				turnRight(dir);
				return TURN_RIGHT;
			}
		}


		if (explore.size() == 0) { 	//how the agent should move
			goBack = true;
			turnAround = true;  
			xDest = 0;
			yDest = 0;
			target = true;
			travel = true;
			turnLeft(dir);
			return TURN_LEFT;
		}
		else {
			bool next = false;
			target = getTarget(xPos, yPos, explore, xDest, yDest, next); //gets target
					
			if (next) //if destination is right next to current tile
			{
				oneMv = true;
				if(xPos == xDest){ //if destination is above or below current tile
					if((yDest - yPos) == 1){
						if(dir == 0){
							turnLeft(dir);
							return TURN_LEFT;
						}
						else if(dir == 1){
							oneMv = false;
							goForward(xPos, yPos, dir);
							return FORWARD;
						}
						else if(dir == 2){
							turnRight(dir);
							return TURN_RIGHT;
						}
						else if(dir == 3){
							hlfTurn = true;
							turnRight(dir);
							return TURN_RIGHT;
						}
					}
					else{ 
						if(dir == 0){
							turnRight(dir);
							return TURN_RIGHT;
						}
						else if(dir == 1){
							hlfTurn = true;
							turnRight(dir);
							return TURN_RIGHT;
						}
						else if(dir == 2){
							turnLeft(dir);
							return TURN_LEFT;
						}
						else if(dir == 3){
							oneMv = false;
							goForward(xPos, yPos, dir);
							return FORWARD;
						}
					}
				}
				else if(yPos == yDest){ //if destination is to the left or right of tile
					if((xDest - xPos) == 1){
						if(dir == 0){
							oneMv = false;
							goForward(xPos, yPos, dir);
							return FORWARD;
						}
						else if(dir == 1){
							turnRight(dir);
							return TURN_RIGHT;
						}
						else if(dir == 2){
							hlfTurn = true;
							turnRight(dir);
							return TURN_RIGHT;
						}
						else if(dir == 3){
							turnLeft(dir);
							return TURN_LEFT;
						}
					}
					else{
						if(dir == 0){
							hlfTurn = true;
							turnRight(dir);
							return TURN_RIGHT;
						}
						else if(dir == 1){
							turnLeft(dir);
							return TURN_LEFT;
						}
						else if(dir == 2){
							oneMv = false;
							goForward(xPos, yPos, dir);
							return FORWARD;
						}
						else if(dir == 3){
							turnRight(dir);
							return TURN_RIGHT;
						}
					}
				}
			}
				
			//else, if the destination is no where near take a step back and search for target 
			travel = true;
			turnHlf = true;
			turnLeft(dir);
			return TURN_LEFT;
		}
	}
	else { //for safe tiles
		checkSafe(xPos, yPos, safe);//add to save list

		if (moves != 0)			//take position out of explore list
			exShorten(xPos, yPos, explore);

		surTiles(xPos, yPos, xLim, yLim, xWall, yWall, safe, testPos);

		addOnly(explore, testPos);//add test list to explore list

		testPos.clear();
		
		if(xPos == 0){
			if(yPos == 0){
				if(dir == 2){
					turnRight(dir);
					return TURN_RIGHT;
				}
				else if(dir == 3){
					turnLeft(dir);
					return TURN_LEFT;
				}
			}
			if(yWall){
				if(yPos == (yLim - 1)){
					if(dir == 1){
						turnRight(dir);
						return TURN_RIGHT;
					}
					else if(dir == 2){
						turnLeft(dir);
						return TURN_LEFT;
					}
				}
			}
			
			if(dir == 2){
				turnRight(dir);
				return TURN_RIGHT;
			}
		}
		
		if(xWall){
			if(xPos == (xLim - 1)){
				if(yPos == 0){
					if(dir == 3){
						turnRight(dir);
						return TURN_RIGHT;
					}
					else if(dir == 0){
						turnLeft(dir);
						return TURN_LEFT;
					}
				}
				if(yWall){
					if(yPos == (yLim - 1)){
						if(dir == 0){
							turnRight(dir);
							return TURN_RIGHT;
						}
						else if(dir == 1){
							turnLeft(dir);
							return TURN_LEFT;
						}
					}
				}

				if(dir == 0){
					turnLeft(dir);
					return TURN_LEFT;
				}
			}
		}
		
		if(yPos == 0){
			if(dir == 3){
				turnLeft(dir);
				return TURN_LEFT;
			}
		}
		
		if(yWall){
			if(yPos == (yLim - 1)){
				if(dir == 1){
					turnRight(dir);
					return TURN_RIGHT;
				}
			}
		}
		
		moves++;
		goForward(xPos, yPos, dir);
		return FORWARD;
	}
	
	// ======================================================================
	// YOUR CODE ENDS
	// ======================================================================
}



// ======================================================================
// YOUR CODE BEGINS
// ======================================================================
void MyAI::turnLeft(int &d){		//directions to keep track of agent
	if(d == 0){
		d = 1;
	}
	else if (d == 1){
		d = 2;
	}
	else if(d == 2){
		d = 3;
	}
	else if(d == 3){
		d = 0;
	}
	return;
}
void turnRight(int &d){
	if(d == 0){
		d = 3;
	}
	else if (d == 1){
		d = 0;
	}
	else if(d == 2){
		d = 1;
	}
	else if(d == 3){
		d = 2;
	}
	return;
}
void goForward(int &x, int &y, int d){
	if(d == 0){
		x++;
	}
	else if (d == 1){
		y++;
	}
	else if(d == 2){
		x--;
	}
	else if(d == 3){
		y--;
	}
	return;
}
void turn180(int &d){
	if(d == 0){
		d = 2;
	}
	else if (d == 1){
		d = 3;
	}
	else if(d == 2){
		d = 0;
	}
	else if(d == 3){
		d = 1;
	}
	return;
}


void MyAI::checkSafe(int x, int y, multimap<int, int> &s){ //insert position into safe list
	if(s.size() == 0)
		s.insert(pair<int, int> (x, y));
	else{
		auto const& chkS = s.equal_range(x);
		map<int, int>::iterator it;
		bool newS = true;
		for(it = chkS.first; it != chkS.second; it++){
			if(it->second == y){
				newS = false;
				break;
			}
		}
		
		if(newS){
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

void MyAI::surTiles(int x, int y, int xL, int yL, bool xW, bool yW, multimap<int, int> &s, multimap<int, int> &t){
	//get surrounding tiles
	if(yW){
		if((y+1) < yL)
			t.insert(pair<int, int>(x, (y + 1)));
	}
	else{
		if(y != yL)		
			t.insert(pair<int, int>(x, (y + 1)));
	}
	if((y-1) >= 0)
		t.insert(pair<int, int>(x, (y - 1)));
	if(xW){
		if((x+1) < xL)
			t.insert(pair<int, int>((x + 1), y));
	}
	else{
		if(x != xL)
			t.insert(pair<int, int>((x + 1), y));
	}
	if((x-1) >= 0)
		t.insert(pair<int, int>((x - 1), y));
				
	map <int, int>::iterator surIt;
	map<int, int>::iterator it;
	for(surIt = s.begin(); surIt != s.end(); surIt++){//take out test list from safe list
		auto const & itrS = t.equal_range(surIt->first);
			
		for(it = itrS.first; it != itrS.second; it++){
			if(it->second == surIt->second){
				t.erase(it);
				break;
			}
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
		map <int, int>::iterator it;
		for(itA = t.begin(); itA != t.end(); itA++){
			auto itr = m.find(itA->first);
					
			if(itr == m.end()){
				m.insert(pair<int, int> (itA->first, itA->second));
			}
			else{
				if(itr->second != itC->second){
					auto const & itM = m.equal_range(itA->first);
					bool exists = false;
					for(it = itM.first; it != itM.second; it++){
						if(it->second == itA->second){
							exists = true;
							break;
						}
					}
					
					if(!exists)
						m.insert(pair<int, int>(itA->first, itA->second)); //add point to list if it didn't exist
				}
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
		map <int, int>::iterator it;
		for(itC = t.begin(); itC != t.end(); itC++){
			auto itr = m.find(itC->first);
					
			if(itr == m.end()){
				m.insert(pair<int, int> (itC->first, itC->second)); 
			}
			else{
				if(itr->second == itC->second){
					auto const & itM = m2.equal_range(itC->first);
					bool exists = false;
					for(it = itM.first; it != itM.second; it++){
						if(it->second == itC->second){
							exists = true;
							break;
						}
					}
					
					if(!exists)
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
void MyAI::wCheckP(multimap<int, int> &e, multimap<int, int> &w, multimap<int, int> &p, multimap<int, int> &p2, multimap<int, int> t){ 
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
	int xDel;
	int yDel;
	bool found = false;
	for(it = t.begin(); it != t.end(); it++){
		auto const& wComp = p.equal_range(it->first);
		
		for(itr = wComp.first; itr != wComp.second; itr++){
			if(itr->second == it->second){
				p.erase(itr);
				e.insert(pair<int, int>(it->first, it->second));
				xDel = it->first;
				yDel = it->second; 
				found = true;
				break;
			}
		}
		if(found)
			break;
	}
	
	if(found){
		auto const& wump = w.equal_range(xDel);
		for (i = wump.first; i != wump.second; i++) {
			if (i->second == yDel) {
				w.erase(i);
				break;
			}
		}
	}
	return;
}

//checks unknown pits to unknown wumpus and adds to explore
void MyAI::pCheckW(multimap<int, int> &e, multimap<int, int> &p, multimap<int, int> &w, multimap<int, int> t){ 
	map<int,int>::iterator it;
	map<int,int>::iterator itr;
	map<int,int>::iterator i;
	
	int xDel;
	int yDel;
	bool found = false;
	for(it = t.begin(); it != t.end(); it++){
		auto const& pComp = w.equal_range(it->first);
		
		for(itr = pComp.first; itr != pComp.second; itr++){
			if(itr->second == it->second){
				w.erase(itr);
				e.insert(pair<int, int>(it->first, it->second));
				xDel = it->first;
				yDel = it->second; 
				found = true;
				break;
			}
		}
		if(found)
			break;
	}
	
	if(found){
		auto const& wump = p.equal_range(xDel);
		for (i = wump.first; i != wump.second; i++) {
			if (i->second == yDel) {
				p.erase(i);
				break;
			}
		}
	}
	
	return;
}

bool MyAI::getTarget(int x, int y, int dir, multimap<int, int> e, int& xD, int& yD, bool& n){
	
	map<int, int>::iterator itT;

	int dist = 1000;
	
	if(dir == 1 || dir == 3){
		auto xFind = e.find(x);
		if(xFind != e.end()){
			auto const & xRange = e.equal_range(x);
			for(itT = xRange.first; itT != xRange.second; itT++){
				int d = abs(itT->first - x) + abs(itT->second -y);

				if(d < dist){
					xD = itT->first;
					yD = itT->second;
					dist = d;
				}
			}
			
			if(dist == 1){
				n = true;  
				return false;
			}
			return true;
		}
	}
	if(dir == 0 || dir == 2){
	
		multimap<int, int> yRange;
		
		for(itT = e.begin(); itT != e.end(); itT++){
			if(itT->second == y)
				yRange.insert(pair<int, int>(itT->first, itT->second));
		}
		if(yRange.size() != 0){
			for(itT = yRange.begin(); itT != yRange.end(); itT++){
				int d = abs(itT->first - x) + abs(itT->second -y);

				if(d < dist){
					xD = itT->first;
					yD = itT->second;
					dist = d;
				}
			}

			if(dist == 1){
				n = true;  
				return false;
			}
			return true;
		}
	}
	
	for(itT = e.begin(); itT != e.end(); itT++){
		int d = abs(itT->first - x) + abs(itT->second -y);
		
		if(d < dist){
			xD = itT->first;
			yD = itT->second;
			dist = d;
		}
	}
	
	if(dist == 1){
		n = true;  
		return false;
	}
	return true;
}

Agent::Action MyAI::goToTarget(int &x, int &y, int &dir, int xL, int yL, int xD, int yD, bool xW, bool yW, multimap<int, int> s, multimap<int, int> &pv)
{					//changed it into an iterative loop, player is on a tile around the destination it goes to it 
	multimap<int, int> t;
	if(yW){
		if((y+1) < yL)
			t.insert(pair<int, int>(x, (y + 1)));
	}
	else{
		if(y != yL)		
			t.insert(pair<int, int>(x, (y + 1)));
	}
	if((y-1) >= 0)
		t.insert(pair<int, int>(x, (y - 1)));
	if(xW){
		if((x+1) < xL)
			t.insert(pair<int, int>((x + 1), y));
	}
	else{
		if(x != xL)
			t.insert(pair<int, int>((x + 1), y));
	}
	if((x-1) >= 0)
		t.insert(pair<int, int>((x - 1), y));
	
	exDelSB(t, pv); //to prevent loops
	pv.insert(pair<int, int>(x, y));
	
	map<int, int>::iterator it;
	map<int, int>::iterator itr;	

	multimap<int, int> tPath;
	for (it = t.begin(); it != t.end(); it++) //take out surrounding tiles that are not in the safe list
	{
		auto const & tPaths = s.equal_range(it->first);
		bool inSafe = false;
		int xPath;
		int yPath;
		
		for(itr = tPaths.first; itr != tPaths.second; itr++){
			if (it->second == itr->second)
			{
				inSafe = true;
				xPath = it->first;
				yPath = it->second;
				break;
			}
		}
		
		if(inSafe){
			tPath.insert(pair<int,int>(xPath, yPath));	
		}
	}
	t.clear();
	t.insert(tPath.begin(), tPath.end());
	tPath.clear();
	
	int nextX = xD;
	int nextY = yD;
				//if current position is far from destination, go to next closest tile
	int dist = abs(xD - x) + abs(yD - y);
	if(dist != 1){
		for(it = t.begin(); it != t.end(); it++){
			int d = abs(xD - it->first) + abs(yD - it->second);
			
			if(d < dist){
				dist = d;
				nextX = it->first;
				nextY = it->second;
			}
		}
	}

	
	if(x == nextX){		//changing player position and direction to next tile
		if((nextY- y) == 1){
			if(dir == 0){
				turnLeft(dir);
				return TURN_LEFT;
			}
			else if(dir == 1){
				goForward(x, y, dir);
				return FORWARD;
			}
			else if(dir == 2){
				turnRight(dir);
				return TURN_RIGHT;
			}
			else if(dir == 3){
				turnRight(dir);
				return TURN_RIGHT;
			}
		}
		else if((nextY - y) == -1){
			if(dir == 0){
				turnRight(dir);
				return TURN_RIGHT;
			}
			else if(dir == 1){
				turnRight(dir);
				return TURN_RIGHT;	
			}
			else if(dir == 2){
				turnLeft(dir);
				return TURN_LEFT;
			}
			else if(dir == 3){
				goForward(x, y, dir);
				return FORWARD;
			}	
		}
	}
	else if(y == nextY){
		if((nextX -x) == 1){
			if(dir == 0){
				goForward(x, y, dir);
				return FORWARD;
			}
			else if(dir == 1){
				turnRight(dir);
				return TURN_RIGHT;
			}
			else if(dir == 2){
				turnRight(dir);
				return TURN_RIGHT;
			}
			else if(dir == 3){
				turnLeft(dir);
				return TURN_LEFT;
			}
		}
		else if((nextX-x) == -1){
			if(dir == 0){
				turnRight(dir);
				return TURN_RIGHT;
			}
			else if(dir == 1){
				turnLeft(dir);
				return TURN_LEFT;
			}
			else if(dir == 2){
				goForward(x, y, dir);
				return FORWARD;
			}
			else if(dir == 3){
				turnRight(dir);
				return TURN_RIGHT;
			}
		}
	}
}
void MyAI::wumpExp(int x, int y, int dir, int xL, int yL, int &xD, int &yD, bool xW, bool yW, multimap<int, int> s, multimap<int, int> &e, int xWump, int yWump){ //sets a target destination from safe to explore
	map<int, int>::iterator it;
	map<int, int>::iterator i;
	
	multimap<int, int> w; //get surrounding tiles of wumpus
	if(yW){
		if((yWump+1) < yL)
			w.insert(pair<int, int>(xWump, (yWump + 1)));
	}
	else{
		if(yWump != yL)		
			w.insert(pair<int, int>(xWump, (yWump + 1)));
	}
	if((yWump-1) >= 0)
		w.insert(pair<int, int>(xWump, (yWump - 1)));
	if(xW){
		if((xWump+1) < xL)
			w.insert(pair<int, int>((xWump + 1), yWump));
	}
	else{
		if(xWump != xL)
			w.insert(pair<int, int>((xWump + 1), yWump));
	}
	if((xWump-1) >= 0)
		w.insert(pair<int, int>((xWump - 1), yWump));
	
	
	multimap<int, int> wPath;
	for (it = w.begin(); it != w.end(); it++) //take out surrounding tiles that are not in the safe list
	{
		auto const & tPaths = s.equal_range(it->first);
		bool inSafe = false;
		int xPath;
		int yPath;
		
		for(itr = wPaths.first; itr != wPaths.second; itr++){
			if (it->second == itr->second)
			{
				inSafe = true;
				xPath = it->first;
				yPath = it->second;
				break;
			}
		}
		
		if(inSafe){
			wPath.insert(pair<int,int>(xPath, yPath));	
		}
	}
	w.clear();
	w.insert(tPath.begin(), tPath.end());
	wPath.clear();
	
				
	int dist = 1000;
	
	for(it = w.begin(); it != w.end(); it++){ //set the safe tiles into explore and set closest tile to destination
		e.insert(pair<int, int> (it->first, it->second);
		int d = abs(it->first - x) + abs(it->second - y);
		if(d < dist){
			dist = d;
			xD = it->first;
			xY = it->second;
		}
	}
	return;	
}
Agent::Action MyAI::wumpHunt(int &x, int &y, int &dir, int xL, int yL, bool xW, bool yW, multimap<int, int> s, int xWump, int yWump, bool &a){ 
	//goes to closest location to kill wumpus
	
	if(x == xWump){		//changing player direction to shoot wumpus
		if((yWump- y) >= 1){
			if(dir == 0){
				turnLeft(dir);
				return TURN_LEFT;
			}
			else if(dir == 1){
				a = true;
				return SHOOT;
			}
			else if(dir == 2){
				turnRight(dir);
				return TURN_RIGHT;
			}
			else if(dir == 3){
				turnLeft(dir);
				return TURN_LEFT;
			}
		}
		else if((yWump - y) <= -1){
			if(dir == 0){
				turnRight(dir);
				return TURN_RIGHT;
			}
			else if(dir == 1){
				turnLeft(dir);
				return TURN_LEFT;	
			}
			else if(dir == 2){
				turnLeft(dir);
				return TURN_LEFT;
			}
			else if(dir == 3){
				a = true;
				return SHOOT;
			}	
		}
	}
	else if(y == nextY){
		if((nextX -x) >= 1){
			if(dir == 0){
				a = true;
				return SHOOT;
			}
			else if(dir == 1){
				turnRight(dir);
				return TURN_RIGHT;
			}
			else if(dir == 2){
				turnLeft(dir);
				return TURN_LEFT;
			}
			else if(dir == 3){
				turnLeft(dir);
				return TURN_LEFT;
			}
		}
		else if((nextX-x) <= -1){
			if(dir == 0){
				turnLeft(dir);
				return TURN_LEFT;
			}
			else if(dir == 1){
				turnLeft(dir);
				return TURN_LEFT;
			}
			else if(dir == 2){
				a = true;
				return SHOOT;
			}
			else if(dir == 3){
				turnRight(dir);
				return TURN_RIGHT;
			}
		}
	}
	
	multimap<int, int> t; //get surrounding positions
	if(yW){
		if((y+1) < yL)
			t.insert(pair<int, int>(x, (y + 1)));
	}
	else{
		if(y != yL)		
			t.insert(pair<int, int>(x, (y + 1)));
	}
	if((y-1) >= 0)
		t.insert(pair<int, int>(x, (y - 1)));
	if(xW){
		if((x+1) < xL)
			t.insert(pair<int, int>((x + 1), y));
	}
	else{
		if(x != xL)
			t.insert(pair<int, int>((x + 1), y));
	}
	if((x-1) >= 0)
		t.insert(pair<int, int>((x - 1), y));
	
	
	map<int, int>::iterator it;
	map<int, int>::iterator itr;	

	multimap<int, int> tPath;
	for (it = t.begin(); it != t.end(); it++) //take out surrounding tiles that are not in the safe list
	{
		auto const & tPaths = s.equal_range(it->first);
		bool inSafe = false;
		int xPath;
		int yPath;
		
		for(itr = tPaths.first; itr != tPaths.second; itr++){
			if (it->second == itr->second)
			{
				inSafe = true;
				xPath = it->first;
				yPath = it->second;
				break;
			}
		}
		
		if(inSafe){
			tPath.insert(pair<int,int>(xPath, yPath));	
		}
	}
	t.clear();
	t.insert(tPath.begin(), tPath.end());
	tPath.clear();
	
	int dist = 1000;
	int nextX;
	int nextY;
	
	auto xLine = s.find(xWump); //find the closest safe location in line to wump at x key
	if(xLine != s.end()){
		for(it = t.begin(); it != t.end(); it++){
			int d = abs(it->first -xWump);
			
			if(d < dist){
				dist = d;
				nextX = it->first;
				nextY = it->second;
				break;
			}
		}
	}
	
	multimap<int, int> yRange; //if no x points available, or if there is a y point closer to current position
	for(it = s.begin(); it != s.end(); it++){
		if(it->second == yWump)
			yRange.insert(pair<int, int>(it->first, it->second));
	}
	if(yRange.size() != 0){
		for(it = t.begin(); it != t.end(); it++){
			auto const &yLine = yRange.equal_range(it->first);
			
			for(i = yLine.first; i != yLine.second; i++){
				int d = abs(it->second- yWump);
				if(d < dist){
					dist = d;
					nextX = it->first;
					nextY = it->second;
				}
			}
		}
	}
	
	
	if(x == nextX){		//changing player position and direction to next, closest tile
		if((nextY- y) == 1){
			if(dir == 0){
				turnLeft(dir);
				return TURN_LEFT;
			}
			else if(dir == 1){
				goForward(x, y, dir);
				return FORWARD;
			}
			else if(dir == 2){
				turnRight(dir);
				return TURN_RIGHT;
			}
			else if(dir == 3){
				turnRight(dir);
				return TURN_RIGHT;
			}
		}
		else if((nextY - y) == -1){
			if(dir == 0){
				turnRight(dir);
				return TURN_RIGHT;
			}
			else if(dir == 1){
				turnRight(dir);
				return TURN_RIGHT;	
			}
			else if(dir == 2){
				turnLeft(dir);
				return TURN_LEFT;
			}
			else if(dir == 3){
				goForward(x, y, dir);
				return FORWARD;
			}	
		}
	}
	else if(y == nextY){
		if((nextX -x) == 1){
			if(dir == 0){
				goForward(x, y, dir);
				return FORWARD;
			}
			else if(dir == 1){
				turnRight(dir);
				return TURN_RIGHT;
			}
			else if(dir == 2){
				turnRight(dir);
				return TURN_RIGHT;
			}
			else if(dir == 3){
				turnLeft(dir);
				return TURN_LEFT;
			}
		}
		else if((nextX-x) == -1){
			if(dir == 0){
				turnRight(dir);
				return TURN_RIGHT;
			}
			else if(dir == 1){
				turnLeft(dir);
				return TURN_LEFT;
			}
			else if(dir == 2){
				goForward(x, y, dir);
				return FORWARD;
			}
			else if(dir == 3){
				turnRight(dir);
				return TURN_RIGHT;
			}
		}
	}
}
// ======================================================================
// YOUR CODE ENDS
// ======================================================================
