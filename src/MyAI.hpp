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
	bool turnHlf;
	int turnCount;

	bool travel;	
	bool goldFound;

	int xPos;
	int yPos;
	int dir;
	
	bool xWall;
	bool yWall;
	int xLim;
	int yLim;

	bool hunt;
	bool wHunt;
	int xWump;
	int yWump;
	bool wumpus;
	bool wFound;
	
	bool target;
	bool oneMv;
	bool hlfTurn;
	int xDest;
	int yDest;

	multimap<int, int> safe;
	multimap<int, int> saveSafe;
	multimap<int, int> testPos;
	multimap<int, int> explore;
	multimap<int, int> pit;
	multimap<int, int> unknownPit;
	multimap<int, int> unknownWump;
	multimap<int, int> wump;
	multimap<int, int> prev;

	void turnLeft(int &d);
	void turnRight(int &d);
	void goForward(int &x, int &y, int d);
	bool turn180(int x, int y, int dir, int xL, int yL, bool xWall, bool yWall, multimap<int, int> s);

	void checkSafe(int x, int y, multimap<int, int> &s);
	void exShorten(int x, int y, multimap<int, int> &e);
	void pitShorten(int x, int y, multimap<int, int> &p, multimap<int, int> p2);
	void surTiles(int x, int y, int xL, int yL, bool xW, bool yW,  multimap<int, int> &s, multimap <int, int> &t);
	void addOnly(multimap<int, int> &m, multimap <int, int> &t);
	void bumpClear(int x, int y, int &xD, int &yD, int xL, int yL, bool xW, bool yW, multimap<int, int> &e);	
			
	void compSelf(multimap<int, int> &m, multimap<int, int> &m2, multimap <int, int> &t);
	void exDelSB(multimap<int, int> &e, multimap<int, int> &t);
	void wCheckP(multimap<int, int> &e, multimap<int, int> &w, multimap<int, int> &p, multimap <int, int> &p2, multimap<int, int> t);
	void pCheckW(multimap<int, int> &e, multimap<int, int> &p, multimap <int, int> &w, multimap<int, int> t);
			
	bool getTarget(int x, int y, int dir, multimap<int, int> e, int &xD, int &yD, bool &n);
	Action naviPath(int &x, int &y, int &dir, int xL, int yL, int xD, int yD, bool xW, bool yW,  multimap<int, int> s, multimap<int, int> &pv);
	
	void wumpExp(int x, int y, int dir, int xL, int yL, int &xD, int &yD, bool xW, bool yW, multimap<int, int> s, multimap<int, int> &e, int xWump, int yWump);
	
	Action wumpHunt(int &x, int &y, int &dir, int xL, int yL, bool xW, bool yW, multimap<int, int> s, int xWump, int yWump, bool &a);
		// ======================================================================
	// YOUR CODE ENDS
	// ======================================================================
};

#endif
