// ======================================================================
// FILE:        MyAI.hpp
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

#ifndef MYAI_LOCK
#define MYAI_LOCK

#include "Agent.hpp"
#include <iostream>
#include <stack>
#include <map>
#include <queue>

using namespace std;

class MyAI : public Agent
{
public:
	MyAI ( void );
	
	Action getAction
	(
		bool stench,
		bool breeze,
		bool glitter,
		bool bump,
		bool scream
	);
	
	// ======================================================================
	// YOUR CODE BEGINS
	// ======================================================================
	
	int moves;
	bool goBack;
	bool arrowShot;
	int turnCount;
	bool turnHlf;
	bool travel;
	bool goldFound;
	
	int xPos; //player position
	int yPos;
	int dir;
	
	bool xWall; //max boundaries
	bool yWall;
	int xLim;
	int yLim;
	
	bool hunt; //for hunting wumpus
	bool wHunt;
	int xWump; //wumpus position if confirmed
	int yWump;
	bool wumpus; //if wumpus has been killed by arrow
	bool wFound; 
	
	bool target; //variables to determine how to get from point A to B
	bool oneMv;
	bool hlfTurn;
	int xDest;
	int yDest;
	
	multimap<int, int> safe; //basic maps
	multimap<int, int> explore;
	multimap<int, int> testPos;
	multimap<int, int> pits;
	multimap<int, int> unknownPit;
	multimap<int, int> unknownWump;
	multimap<int, int> wump;
	multimap<int, int> prev;
	
	//functions to shorten code
	void turnLeft(int &d);		//directions to keep track of agent
	void turnRight(int &d);
	void goForward(int &x, int &y, int d);
	void turn180(int &d);
	
	void checkSafe(int x, int y, multimap<int, int> &s); //checks and adds new tile to safe list
	void exShorten(int x, int y, multimap<int, int> &e); //takes out current tile from explore list
	void surTiles(int x, int y, int xL, int yL, bool xW, bool yW, multimap<int, int> &s, multimap<int, int> &t); //gets list of surrounding unknown tiles
	void addOnly(multimap<int, int> &m, multimap<int, int> &t); //adds new tiles to an existing list
	void bumpClear(int x, int y, int &xD, int &yD, int xL, int yL, bool xW, bool yW, mulitmap<int, int> &e); //clears out tiles from explore that exceed boundaries 
	
	void compSelf(multimap<int, int> &m, multimap<int, int> &m2, multimap<int, int> &t); //compares for overlays and adds to known list
	void exDelSB(multimap<int, int> &e, multimap<int, int> &t); //takes out surrounding tiles from explore if there is breadth or stench
	void wCheckP(multimap<int, int> &e, multimap<int, int> &w, multimap<int, int> &p, multimap<int, int> &p2, multimap<int, int> t); //check if testPos of wump conflicts with any pit lists, take out from wump and pit and add to explore accordingly
	void pCheckW(multimap<int, int> &e, multimap<int, int> &p, multimap<int, int> &w, multimap<int, int> t); //check if unknown pits conflict with wump list, take out from wump and pit and add to explore accordingly
	
	bool getTarget(int x, int y, int dir, multimap<int, int> e, int &xD, int &yD, bool &n); //bool to get to target destination
	Action goToTarget(int &x, int &y, int &dir, int xL, int yL, int xD, int yD, bool xW, bool yW, multimap<int, int> s, multimap<int, int> &pv); //goes to set target destination
	
	void wumpExp(int x, int y, int dir, int xL, int yL, int &xD, int &yD, bool xW, bool yW, multimap<int, int> s, multimap<int, int> &e, int xWump, int yWump); //sets a target destination from safe to explore
	Action wumpHunt(int &x, int &y, int &dir, int xL, int yL, bool xW, bool yW, multimap<int, int> s, int xWump, int yWump, bool &a); //goes to a location to kill wumpus
	// ======================================================================
	// YOUR CODE ENDS
	// ======================================================================
};

#endif
