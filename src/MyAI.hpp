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
	stack <Action> retrace;
	int moves;
	bool goBack;
	bool arrowShot;
	
	int xPos; //player position
	int yPos;
	int dir;
	
	bool xWall; //max boundaries
	bool yWall;
	int xLim;
	int yLim;
	
	int xWump; //wumpus position if confirmed
	int yWump;
	bool wumpus;
	
	multimap<int, int> safe; //basic maps
	multimap<int, int> explore;
	multimap<int, int> testPos;
	multimap<int, int> pits;
	multimap<int, int> unknownPit;
	multimap<int, int> unknownWump;
	multimap<int, int> wump;
	
	//functions to shorten code
	void checkSafe(int x, int y, multimap<int, int> &s); //checks and adds new tile to safe list
	void exShorten(int x, int y, multimap<int, int> &e); //takes out current tile from explore list
	void surTiles(int x, int y, int xL, int yL, multimap<int, int> &s, multimap<int, int> &t); //gets list of surrounding unknown tiles
	void addOnly(multimap<int, int> &m, multimap<int, int> &t); //adds new tiles to an existing list
	void compSelf(multimap<int, int> &m, multimap<int, int> &m2, multimap<int, int> &t); //compares for overlays and adds to known list
	void exDelSB(multimap<int, int> &e, multimap<int, int> &t); //takes out surrounding tiles from explore if there is breadth or stench
	void wCheckP(multimap<int, int> &e, multimap<int, int> &w, multimap<int, int> &p, multimap<int, int> &p2, multimap<int, int> &t); //check if testPos of wump conflicts with any pit lists, take out from wump and pit and add to explore accordingly
	void pCheckW(multimap<int, int> &e, multimap<int, int> &p, multimap<int, int> &w, multimap<int, int> &t); //check if unknown pits conflict with wump list, take out from wump and pit and add to explore accordingly
	
	// ======================================================================
	// YOUR CODE ENDS
	// ======================================================================
};

#endif
