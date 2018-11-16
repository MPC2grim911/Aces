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
	
	int xPos; //player position
	int yPos;
	int dir;
	
	bool xWall; //max boundaries
	bool yWall;
	int xLim;
	int yLim;
	
	int xWump; //wumpus position if confirmed
	int yWump;
	
	multimap<int, int> safe; //basic maps
	multimap<int, int> explore;
	multimap<int, int> testPos;
	multimap<int, int> pits;
	multimap<int, int> unknownPit;
	multimap<int, int> unknownWump;
	
	//functions to shorten code
	void surTiles(int x, int y, int xL, int yL, multimap<int, int> &s, multimap<int, int> &t); //gets list of surrounding unknown tiles
	
	// ======================================================================
	// YOUR CODE ENDS
	// ======================================================================
};

#endif
