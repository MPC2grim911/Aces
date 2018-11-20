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
#include <queue>
#include <vector>
#include <iterator>
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
	queue <Action> stuff;
	vector< pair <int, int>> avoidMap;
	vector< pair <int, int>> goMap;
	pair<int, int> test;
	pair<int, int> temp;
	bool doStuff;
	int moves;
	bool goBack;
	bool arrowShot;
	bool wumpusDead;
	bool wumpusFound;
	bool foundGold;
	int x, y, dir, xLim, yLim;
	bool spin;
	void addGoMap();
	bool checkAvoid(pair<int, int> test);
	void updatePos();
	void updateLeft();
	void updateRight();
	void updateBump();
	Action explore();
	bool canExplore();
	Action moveUp();
	Action moveDown();
	Action moveLeft();
	Action moveRight();
	
	// ======================================================================
	// YOUR CODE ENDS
	// ======================================================================
};

#endif