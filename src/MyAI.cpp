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
#include <queue>

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
	hunt = false;
	wHunt = false;

	travel = false;
	goldFound = false;
	
	xPos = 0; //player initial position
	yPos = 0;
	dir = 0; //0 is E, 1 is N, 2 is W, 3 is S

	xWall = false; //true when bump into wall
	yWall = false;
	xLim = -1; //changed when bump into wall
	yLim = -1;	

	xWump = -1; //changed when find wumpus
	yWump = -1;
	wumpus = false;
	wFound = false;
	
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

/**/	cout << endl << "Player: " << xPos << ", " << yPos << "\t" << dir << endl;
	
	if(oneMv){
		if(hlfTurn){
			hlfTurn = false;
			turnRight(dir);
			return TURN_RIGHT;
		}
		
		goForward(xPos, yPos, dir);
	
		oneMv = false;
					
		return FORWARD;
	}
	 
	if(glitter)
	{
/**/		cout << "Found gold" << endl;
		goBack = true;
		target = true;
		travel = true;
		xDest = 0;
		yDest = 0;

		int dist = abs(xDest - xPos) + abs(yDest = yPos);
		if(dist > 1)
			goldFound = turn180(xPos, yPos, dir, xLim, yLim, xWall, yWall, safe);;	
		
		return (GRAB);
	}
	
	if(scream){
/**/		cout << "wumpus is dead" << endl;
		wumpus = true;
		unknownWump.clear();
	}

	if(bump)
	{
/**/		cout << "Hit wall" << endl;		
		if(dir == 0){
			if(!xWall){
				xLim = xPos;
				xWall = true;
			}
			xPos = xLim -1;
			bumpClear(xPos, yPos, xDest, yDest, xLim, yLim, xWall, yWall, explore);	
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
			bumpClear(xPos, yPos, xDest, yDest, xLim, yLim, xWall, yWall, explore);	
			if(xPos == 0){
				turnRight(dir);
				return TURN_RIGHT;
			}
			
		}
		else if(dir == 2){
			xPos = 0;
			bumpClear(xPos, yPos, xDest, yDest, xLim, yLim, xWall, yWall, explore);	
			if(yPos == 0){
				turnRight(dir);
				return TURN_RIGHT;
			}
		}
		else if(dir == 3){
			yPos = 0;
			bumpClear(xPos, yPos, xDest, yDest, xLim, yLim, xWall, yWall, explore);	
			if(xPos == xLim){
				turnRight(dir);
				return TURN_RIGHT;
			}
			
		}

		turnLeft(dir);
		return TURN_LEFT;
	}

	if(hunt){ //fill this in with hunt function
		if(arrowShot){
			hunt = false;
			wHunt = false;
			target = true;
			travel = true;	
			if(wumpus){
				wumpExp(xPos, yPos, dir, xLim, yLim, xDest, yDest, xWall, yWall, safe, explore, xWump, yWump);	
				cout << "Next Destination: " << xDest << ", " << yDest << endl;
			}
			else if(!wumpus){
				goBack = true;
				xDest = 0;
				yDest = 0;
			}
			
		}
		
		if(wHunt){
//			cout << "entered hunt" << endl;		
			return wumpHunt(xPos, yPos, dir, xLim, yLim, xWall, yWall, safe, xWump, yWump, arrowShot);
		}
	}
	
	if(target){
//		cout << "entered targeting system " << endl;
		if(goldFound){
			if(turnCount == 2){
				turnCount = 0;
				goldFound = false;
				goForward(xPos, yPos, dir);
				return FORWARD;
			}
			turnCount++;
			turnLeft(dir);
			return TURN_LEFT;
		
		}

		if(turnHlf){
			if(turnCount == 1){
				turnCount = 0;
				turnHlf = false;
				goForward(xPos, yPos, dir);
				return FORWARD;
			}
		
			turnCount++;
			turnLeft(dir);
			return TURN_LEFT;
		}

		if(xPos == xDest && yPos == yDest){
			target = false;
			travel = false;
			prev.clear();
			if((xPos == 0 && yPos == 0) && goBack){
				return CLIMB;
			}
		}

		if(travel){
//			cout << "going to travel" << endl;
			return naviPath(xPos, yPos, dir, xLim, yLim, xDest, yDest, xWall, yWall, safe, prev);
		}
	}
		
	if(moves == 0 ) //1st turn
	{

/**/		cout << "1st turn" << endl;

		if(breeze){
/**/			cout << "felt breeze" << endl;
			return CLIMB;
		}
		if(stench){
/**/			cout << "smelled STENCH" << endl;
			if(!arrowShot){
/**/				cout << "arrow shot" << endl;
				arrowShot = true;
				return SHOOT;
			}
			if(!wumpus){						
				xWump = 0;
				yWump = 1;
				wFound = true;
			}
			else{
				explore.insert(pair<int, int> (0, 1));
			}
			checkSafe(xPos, yPos, safe);
			goForward(xPos, yPos, dir);
			moves++;
			return FORWARD;

		}
		else{
			checkSafe(xPos, yPos, safe);

			if(moves != 0)
				exShorten(xPos, yPos, explore);
		
			surTiles(xPos, yPos, xLim, yLim, xWall, yWall, safe, testPos);

			addOnly(explore, testPos);
		
			testPos.clear();

			moves++;

			goForward(xPos, yPos, dir);
	
			return FORWARD;
		}
	}
	else{
		if(stench || breeze){
			checkSafe(xPos, yPos, safe);
			exShorten(xPos, yPos, explore);

			if(unknownPit.size() != 0){
				pitShorten(xPos, yPos, unknownPit, pit);
			}
			surTiles(xPos, yPos, xLim, yLim, xWall, yWall, safe, testPos);

			if(stench && breeze){
/**/				cout << "there's a Stench and Breeze" << endl;
				if(!wumpus){
					compSelf(unknownWump, wump, testPos);
				}
				
				compSelf(unknownPit, pit, testPos);
				
				//exDelSB(explore, testPos, saveSafe);
			}
			else if(stench){
/**/				cout << "something smells" << endl;
				if(!wumpus){
					compSelf(unknownWump, wump, testPos);
				//	exDelSB(explore, testPos, saveSafe);
					
					if(wump.size() == 0 && !wFound)
						wCheckP(explore, unknownWump, unknownPit, pit, testPos);//compare with pits
				}
				if(wumpus){
					cout << "if stench only and wumpus is dead:" << endl;
					map<int, int>::iterator i;
					cout << "   explore:" << endl;
					for(i = explore.begin(); i != explore.end(); i++){
						cout << "\t" << i->first << " " << i->second << endl;
					}
					cout << "  testPos:" << endl;
					for(i = testPos.begin(); i != testPos.end(); i++){
						cout << "\t" << i->first << " " << i->second << endl;
					}
					addOnly(explore, testPos);
				}	
			}
			else if(breeze){
/**/				cout << "felt breeze" << endl;
				compSelf(unknownPit, pit, testPos);
				//exDelSB(explore, testPos, saveSafe);
	
				if(!wumpus && !wFound)//compare with wumpus	
					pCheckW(explore, unknownPit, unknownWump, testPos);		
			}

			if(pit.size() != 0)
				exDelSB(explore, pit);
			
			testPos.clear();

			map<int, int>::iterator it;
			
			cout << "Safe Points: " << endl;
			for(it = safe.begin(); it != safe.end(); it++){
				cout << "\t" << it->first << " " << it->second << endl;
			}
			cout << "Explore Points: " << endl;
			for(it = explore.begin(); it != explore.end(); it++){
				cout << "\t" << it->first << " " << it->second << endl;
			}

			cout << "Wumpus?: " << endl;
			for(it = unknownWump.begin(); it != unknownWump.end(); it++){
				cout << "\t" << it->first << " " << it->second << endl;
			}
			cout << "Wumpus Location: " << endl;
			for(it = wump.begin(); it != wump.end(); it++){
				cout << "\t" << it->first << " " << it->second << endl;
			}
			cout << "Wumpus Location(x,y):\t " << xWump << ", " << yWump << endl; 
				
			cout << "Pit????: " << endl;
			for(it = unknownPit.begin(); it != unknownPit.end(); it++){
				cout << "\t" << it->first << " " << it->second << endl;
			}
			cout << "Pits: " << endl;
			for(it = pit.begin(); it != pit.end(); it++){
				cout << "\t" << it->first << " " << it->second << endl;
			}

			if(!wumpus && !wFound){
				if(unknownWump.size() == 1 || wump.size() == 1){
					map<int, int>::iterator i;

					if(wump.size() == 0){
						i = unknownWump.begin();
						xWump = i->first;
						yWump = i->second;
					}
					else{
						i = wump.begin();
						xWump = i->first;
						yWump = i->second;
					}
					unknownWump.clear();
					wFound = true;
/**/					cout << "Wumpus Found" << endl;
				}
			}
			if(wFound){
				//take out wumpus point from explore list
				//if explore size == 0, then hunt wumpus
				if(!wumpus)		
					exShorten(xWump, yWump, explore);	
				if(explore.size() == 0 && !arrowShot){
/**/					cout << "Hunting time" << endl;
					hunt = true;
					wHunt = true;
					turnRight(dir);
					return TURN_RIGHT;
				}
			}
			
			if(explore.size() == 0 || explore.empty()){
				cout << "going back" << endl; 
				goBack = true;
				target = true;
				travel = true;
				turnHlf = true;
				xDest = 0;
				yDest = 0;
				turnLeft(dir);
				return TURN_LEFT;
			}
			else{
		
				//need to get hunt function working
				bool next = false;
				target = getTarget(xPos, yPos, dir, explore, xDest, yDest, next);
					
/**/				cout << "Next Destination: " << xDest << ", " << yDest << endl;

				if(next){
/**/					cout << "one move only function" << endl;
					oneMv = true;
					if(xPos == xDest){
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
								hlfTurn = true; //180 degree turn
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
					if(yPos == yDest){
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
								hlfTurn = true; //180 degree turn
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
								hlfTurn = true; //180 degree turn
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
/**/				cout << "take a step back and go to target" << endl;		
				turnHlf = turn180(xPos, yPos, dir, xLim, yLim, xWall, yWall, safe);
				travel = true;
				turnLeft(dir);	
				return TURN_LEFT;
					
			}
		}
		else{
			checkSafe(xPos, yPos, safe);

			if(moves != 0)
				exShorten(xPos, yPos, explore);
		
			if(unknownPit.size() != 0){
				pitShorten(xPos, yPos, unknownPit, pit);
			}

			if(pit.size() != 0)
				exDelSB(explore, pit);
				
			surTiles(xPos, yPos, xLim, yLim, xWall, yWall, safe, testPos);

			addOnly(explore, testPos);
			
			//addOnly(saveSafe, testPos);

			testPos.clear();

/**/			cout << "Safe Points: " << endl;
			map<int, int>::iterator it;
			for(it = safe.begin(); it != safe.end(); it++){
				cout << "\t" << it->first << " " << it->second << endl;
			}
			cout << "Explore Points: " << endl;
			for(it = explore.begin(); it != explore.end(); it++){
				cout << "\t" << it->first << " " << it->second << endl;
			}

			if(!wumpus && !wFound){
				if(unknownWump.size() == 1 || wump.size() == 1){
					map<int, int>::iterator i;

					if(wump.size() == 0){
						i = unknownWump.begin();
						xWump = i->first;
						yWump = i->second;
					}
					else{
						i = wump.begin();
						xWump = i->first;
						yWump = i->second;
					}
					unknownWump.clear();
					wFound = true;
/**/					cout << "Wumpus Found" << endl;
				}
			}
			if(wFound){
				//take out wumpus point from explore list
				//if explore size == 0, then hunt wumpus
				if(!wumpus)		
					exShorten(xWump, yWump, explore);	
				if(explore.size() == 0 && !arrowShot){
/**/					cout << "Hunting time" << endl;
					hunt = true;
					wHunt = true;
					turnRight(dir);
					return TURN_RIGHT;
				}
			}
				
			if(explore.size() == 0 || explore.empty()){
				cout << "going back" << endl; 
				goBack = true;
				target = true;
				travel = true;
				turnHlf = true;
				xDest = 0;
				yDest = 0;
				turnLeft(dir);
				return TURN_LEFT;
			}

			bool next = false;
			target = getTarget(xPos, yPos, dir, explore, xDest, yDest, next);
					
/**/			cout << "Next Destination: " << xDest << ", " << yDest << endl;

			if(next){
/**/				oneMv = true;
				if(xPos == xDest){
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
							hlfTurn = true; //180 degree turn
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
				if(yPos == yDest){
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
							hlfTurn = true; //180 degree turn
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
							hlfTurn = true; //180 degree turn
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
			/*cout << "take a step back and go to target" << endl;		
			turnHlf = true;
			travel = true;
			turnLeft(dir);	
			return TURN_LEFT;*/


			if(dir == 0){
				if(xWall){
					if(yWall){
						if(yPos == (yLim - 1)){
							cout << "entered upper right corner" << endl;
							turnRight(dir);
							return TURN_RIGHT;
						}
					}
					if(xPos == (xLim - 1)){
						turnLeft(dir);
						return TURN_LEFT;
					}
				}
			}
			else if(dir == 1){
				if(yWall){
					if(xWall){
						if(xPos == (xLim - 1)){
							cout << "entered upper left corner" << endl;
							turnLeft(dir);
							return TURN_LEFT;
						}
					}
					if(yPos == (yLim - 1)){
						turnRight(dir);
						return TURN_RIGHT;
					}
				}
			}
			else if(dir == 2){
				if(xPos == 0){
					if(yWall){
						if(yPos = (yLim -1)){
							turnLeft(dir);
							return TURN_LEFT;
						}
					}
					turnRight(dir);
					return TURN_RIGHT;
				}
			}
			else if(dir == 3){
				if(yPos == 0){
					if(xWall){
						if(xPos = (xLim -1)){
							turnRight(dir);
							return TURN_RIGHT;
						}
					}
					turnLeft(dir);
					return TURN_LEFT;
				}
			}
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

	}	
	
//	cout << " climbed out" << endl;
	// ======================================================================
	// YOUR CODE ENDS
	// ======================================================================
}

// ======================================================================
// YOUR CODE BEGINS
// ======================================================================
void MyAI::turnLeft(int &d){
	if(d == 0){
		d = 1;
	}
	else if(d == 1){
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
void MyAI::turnRight(int &d){
	if(d == 0){
		d = 3;
	}
	else if(d == 1){
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
void MyAI:: goForward(int &x, int &y, int d){
	if(d == 0){
		x++;
	}
	else if(d == 1){
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

bool MyAI::turn180(int x, int y, int dir, int xL, int yL, bool xWall, bool yWall, multimap<int, int> s){
	int xCheck = x;
	int yCheck = y;

	map<int, int>::iterator i;
	if(dir == 0){
		if((x-1) >= 0){
			xCheck = x - 1;

			for(i = s.begin(); i != s.end(); i++){
				if(i->first == xCheck && i->second == yCheck)
					return true;
			}
		}
	}
	else if(dir == 1){
		if((y-1) >= 0){
			yCheck = y - 1;

			for(i = s.begin(); i != s.end(); i++){
				if(i->first == xCheck && i->second == yCheck)
					return true;
			}
		}

	}
	else if(dir == 2){
		if(xWall){
			if((x+1) <= xL){
				xCheck = x + 1;

				for(i = s.begin(); i != s.end(); i++){
					if(i->first == xCheck && i->second == yCheck)
						return true;
				}
			}
		}		
	}
	else if(dir == 3){
		if(yWall){
			if((y+1) <= yL){
				yCheck = y + 1;

				for(i = s.begin(); i != s.end(); i++){
					if(i->first == xCheck && i->second == yCheck)
						return true;
				}
			}
		}		
	}
	return false;
}


void MyAI::checkSafe(int x, int y, multimap<int,int> &s){
	/*cout << "safe Check: " << endl;
	map<int, int>::iterator i;
	cout << "    before:" << endl;
	for(i = s.begin(); i != s.end(); i++){
		cout << "\t" << i->first << " " << i->second << endl;
	}*/

	if(s.size() == 0)
		s.insert(pair<int,int>(x, y));
	else{
//		cout << "entered insertion" << endl;
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
			s.insert(pair<int,int>(x, y));
		}
	}

/*	cout << "    after:" << endl;
	for(i = s.begin(); i != s.end(); i++){
		cout << "\t" << i->first << " " << i->second << endl;
	}
*/
	return;
}

void MyAI::exShorten(int x, int y, multimap<int, int> &e){
/*	cout << "exShorten:" << endl;
	map<int, int>::iterator i;
	cout << "    before:" << endl;
	for(i = e.begin(); i != e.end(); i++){
		cout << "\t" << i->first << " " << i->second << endl;
	}
*/	
	if(e.count(x) == 1){
		auto exDel = e.find(x);
		if(exDel != e.end()){
			if(exDel->second == y)
				e.erase(x);
		}
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
	
/*	cout << "    after:" << endl;
	for(i = e.begin(); i != e.end(); i++){
		cout << "\t" << i->first << " " << i->second << endl;
	}
*/
	return;
}

void MyAI::pitShorten(int x, int y, multimap<int, int> &p, multimap<int, int> p2){
	map<int, int>::iterator it;
	map<int, int>::iterator i;
	if(p.count(x) == 1){
		auto pDel = p.find(x);
		if(pDel != p.end()){
			if(pDel->second == y)
				p.erase(x);
		}
	}
	else{
		auto const& pDel = p.equal_range(x);
		for(it = pDel.first; it != pDel.second; it++){
			if(it->second == y){
				p.erase(it);
				break;
			}
		}
	}

	if(p2.size() != 0){
		for(it = p2.begin(); it != p2.end(); it++){
			for(i = p.begin(); i != p.end(); i++){
				if(i->first == it->first && i->second == it->second){
					p.erase(i);
					break;
				}
			}
		}
	}
	
	return;
}
void MyAI::surTiles(int x, int y, int xL, int yL, bool xW, bool yW, multimap<int, int> &s, multimap<int, int> &t){
	
	if(yW){
		if((y+1) < yL)
			t.insert(pair<int, int> (x, (y + 1)));
	}
	else{
		if(y != yL)
			t.insert(pair<int, int> (x, (y + 1)));
	}
	if((y-1) >= 0)
		t.insert(pair<int, int> (x, (y - 1)));
	
	if(xW){
		if((x+1) < xL)
			t.insert(pair<int, int> ((x + 1), y));
	}
	else{
		if(x != xL)
			t.insert(pair<int, int> ((x + 1), y));
	}
	if((x-1) >= 0)
		t.insert(pair<int, int> ((x - 1), y));

	map<int, int>::iterator surIt;
	map<int, int>::iterator it;
	for(surIt = s.begin(); surIt != s.end(); surIt++){
		auto const& itrS = t.equal_range(surIt->first);
		for(it = itrS.first; it != itrS.second; it++){
			if(surIt->second == it->second){
				t.erase(it);
				break;
			}
		}
	}	

	return;
}

void MyAI::addOnly(multimap<int, int> &m, multimap<int, int> &t){
/*	cout << "AddOnly: " << endl;
	map<int, int>::iterator i;
	cout << "    testPos:" << endl;
	for(i = t.begin(); i != t.end(); i++){
		cout << "\t" << i->first << " " << i->second << endl;
	}
	cout << "    mapPoints:" << endl;
	for(i = m.begin(); i != m.end(); i++){
		cout << "\t" << i->first << " " << i->second << endl;
	}
*/

	if(m.size() == 0){
		m.insert(t.begin(), t.end());
	}
	else{
		map<int, int>::iterator itA;
		map<int, int>::iterator i;	

		for(itA = t.begin(); itA != t.end(); itA++){
			auto itr = m.find(itA->first);
			if(itr == m.end()){
				m.insert(pair<int, int>(itA->first, itA->second));
			}
			else{
				if(itr->second != itA->second){
					auto const& itM = m.equal_range(itA->first);
					bool exists = false;
					for( i = itM.first; i != itM.second; i++){
						if(i->second == itA->second){
							exists = true;
							break;	
						}		
					}
					if(!exists)		
						m.insert(pair<int, int>(itA->first, itA->second));
										
				}
			}
		}
	}

/*	cout << "  After:" << endl;
	for(i = m.begin(); i != m.end(); i++){
		cout << "\t" << i->first << " " << i->second << endl;
	}
*/

	return;
}

void MyAI::bumpClear(int x, int y, int &xD, int &yD, int xL, int yL, bool xW, bool yW, multimap<int, int> &e){
	xD = x;
	yD = y;
/*	cout << "bumpClear: " << endl;
	map<int, int>::iterator i;
	cout << "  Before:" << endl;
	cout << "    explore:" << endl;
	for(i = e.begin(); i != e.end(); i++){
		cout << "\t" << i->first << " " << i->second << endl;
	}
*/
	map<int, int>::iterator it;
	map<int, int>::iterator i;
	if(xW){
		auto xDel = e.find(xL);
		if(xDel != e.end()){
			e.erase(xDel);
			/*for(it = xDel.first; it != xDel.second; it++){
				for(i = e.begin(); i != e.end(); i++){
					if((i->first == it->first) && (i->second == it->second)){
						e.erase(i);
						break;
					}
				}
			}*/
		}
	}
	if(yW){
		multimap<int, int> yRange;
		for(it = e.begin(); it != e.end(); it++){
			if(it->second == yL){
				yRange.insert(pair<int, int> (it->first, it->second));
			}
		}

		for(it = yRange.begin(); it != yRange.end(); it++){
			for(i = e.begin(); i != e.end(); i++){
				if((i->first == it->first) && (i->second == it->second)){
					e.erase(i);
					break;
				}
			}	
		}
	}
/*	cout << "  After:" << endl;
	cout << "    explore:" << endl;
	for(i = e.begin(); i != e.end(); i++){
		cout << "\t" << i->first << " " << i->second << endl;
	}
*/
	return;
}	
	

void MyAI::compSelf(multimap<int, int> &m, multimap<int, int> &m2, multimap<int, int> &t){
/*	cout << "Compare Points: " << endl;
	map<int, int>::iterator i;
	cout << "    TestPos:" << endl;
	for(i = t.begin(); i != t.end(); i++){
		cout << "\t" << i->first << " " << i->second << endl;
	}

	cout << "    mapPoints:" << endl;
	for(i = m.begin(); i != m.end(); i++){
		cout << "\t" << i->first << " " << i->second << endl;
	}
*/

	if(m.size() == 0){
		m.insert(t.begin(), t.end());
	}
	else{
		map<int, int>::iterator it;
		map<int, int>::iterator i;
		
		for(it = t.begin(); it != t.end(); it++){
			auto itr = m.find(it->first);
			if(itr == m.end()){
				m.insert(pair<int, int>(it->first, it->second));
			}
			else{
				if(itr->second == it->second){
					auto const& itM = m2.equal_range(it->first);
					bool exists = false;
					for( i = itM.first; i != itM.second; i++){
						if(i->second == it->second){
							exists = true;
							break;	
						}		
					}
					if(!exists)		
						m2.insert(pair<int, int>(it->first, it->second));
										
					m.erase(itr);
				}
			}
		}
	}
/*	cout << "  After:" << endl;
	cout << "    mapPoints:" << endl;
	for(i = m.begin(); i != m.end(); i++){
		cout << "\t" << i->first << " " << i->second << endl;
	}
	cout << "    overlap: " << endl;;
	for(i = m2.begin(); i != m2.end(); i++){
		cout << "\t" << i->first << " " << i->second << endl;
	}

*/
	return;

}

void MyAI::exDelSB(multimap<int, int> &e, multimap<int, int> &t){
/*	cout << "exDelSB: " << endl;
	map<int, int>::iterator i;
	cout << "    TestPos:" << endl;
	for(i = t.begin(); i != t.end(); i++){
		cout << "\t" << i->first << " " << i->second << endl;
	}

	cout << "    explore before:" << endl;
	for(i = e.begin(); i != e.end(); i++){
		cout << "\t" << i->first << " " << i->second << endl;
	}
*/

	map<int, int>::iterator it;

	for(it = t.begin(); it != t.end(); it++){
		if(e.count(it->first) == 1){
			auto exDel = e.find(it->first);
			if(exDel != e.end())
				if(exDel->second == it->second)
					e.erase(exDel);
		}
		else{
			auto const& exDel = e.equal_range(it->first);
			map<int, int>::iterator itr;
			for(itr = exDel.first; itr != exDel.second; itr++){
				if(itr->second == it->second){
					e.erase(itr);
					break;
				}
			}
		}
	
	}
	
/*	cout << "  After:" << endl;
	for(i = e.begin(); i != e.end(); i++){
		cout << "\t" << i->first << " " << i->second << endl;
	}
*/
	return;
}

void MyAI::wCheckP(multimap<int, int> &e, multimap<int, int> &w, multimap<int, int> &p, multimap<int, int> &p2, multimap<int, int> t){
/*	cout << "wCheckP: " << endl;
	map<int, int>::iterator i;
	cout << "  Before: " << endl;
	cout << "    TestPos:" << endl;
	for(i = t.begin(); i != t.end(); i++){
		cout << "\t" << i->first << " " << i->second << endl;
	}
	cout << "    explore:" << endl;
	for(i = e.begin(); i != e.end(); i++){
		cout << "\t" << i->first << " " << i->second << endl;
	}
	cout << "    unknownWump:" << endl;
	for(i = w.begin(); i != w.end(); i++){
		cout << "\t" << i->first << " " << i->second << endl;
	}
	cout << "    unknownPit:" << endl;
	for(i = p.begin(); i != p.end(); i++){
		cout << "\t" << i->first << " " << i->second << endl;
	}
	cout << "    pits:" << endl;
	for(i = p2.begin(); i != p2.end(); i++){
		cout << "\t" << i->first << " " << i->second << endl;
	}
*/

	map<int, int>::iterator it;
	map<int, int>::iterator itr;
	map<int, int>::iterator i;

//	cout << "checking against known pits" << endl;
	
	if(p2.size() != 0){
		for(it = t.begin(); it != t.end(); it++){	
			auto const& wDel = p2.equal_range(it->first);
			bool found = false; 
			for(itr = wDel.first; itr != wDel.second; itr++){
				if(itr->second == it->second){
					auto const& wump = w.equal_range(it->first);
					for(i = wump.first; i!= wump.second; i++){
						if(i->second == it->second){
							w.erase(i);
							found = true;
							break;
						}
					}
				}
				if(found)
					break;
			}
		}
			
	}
/*	cout << "  After pits: " << endl;
	cout << "    TestPos:" << endl;
	for(i = t.begin(); i != t.end(); i++){
		cout << "\t" << i->first << " " << i->second << endl;
	}
	cout << "    explore:" << endl;
	for(i = e.begin(); i != e.end(); i++){
		cout << "\t" << i->first << " " << i->second << endl;
	}
	cout << "    unknownWump:" << endl;
	for(i = w.begin(); i != w.end(); i++){
		cout << "\t" << i->first << " " << i->second << endl;
	}
	cout << "    unknownPit:" << endl;
	for(i = p.begin(); i != p.end(); i++){
		cout << "\t" << i->first << " " << i->second << endl;
	}
	cout << "    pits:" << endl;
	for(i = p2.begin(); i != p2.end(); i++){
		cout << "\t" << i->first << " " << i->second << endl;
	}
//	cout << "checking against unknown pits" << endl;	
*/

	int xDel;
	int yDel;
	bool found = false;
	for(it = t.begin(); it != t.end(); it++){	
		auto const& wComp = p.equal_range(it->first);
//		cout << "compariing with unknown pit value" << endl;
		for(itr = wComp.first; itr != wComp.second; itr++){
			if(itr->second == it->second){
//				cout << "found overlap" << endl;
				p.erase(itr);
				e.insert(pair<int, int>(it->first, it->second));
				found = true;
				xDel = it->first;
				yDel = it->second;
				break;
			/*	cout << "erasing unknownWump Coord" << endl;
				auto const& wump = w.equal_range(it->first);
				for(i = wump.first; i!= wump.second; i++){
					if(i->second == it->second){
						w.erase(i);
			//			found = true;
						break;
					}
				}*/
			}
		}

		if(found)
			break;
	}

	if(found){
//		cout << "erasing unknownWump Coord" << endl;
	
		auto const& wump = w.equal_range(xDel);
		for(i = wump.first; i!= wump.second; i++){
			if(i->second == yDel){
				w.erase(i);
				break;
			}
		}
	}
/*	cout << "  After: " << endl;
	cout << "    TestPos:" << endl;
	for(i = t.begin(); i != t.end(); i++){
		cout << "\t" << i->first << " " << i->second << endl;
	}
	cout << "    explore:" << endl;
	for(i = e.begin(); i != e.end(); i++){
		cout << "\t" << i->first << " " << i->second << endl;
	}
	cout << "    unknownWump:" << endl;
	for(i = w.begin(); i != w.end(); i++){
		cout << "\t" << i->first << " " << i->second << endl;
	}
	cout << "    unknownPit:" << endl;
	for(i = p.begin(); i != p.end(); i++){
		cout << "\t" << i->first << " " << i->second << endl;
	}
	cout << "    pits:" << endl;
	for(i = p2.begin(); i != p2.end(); i++){
		cout << "\t" << i->first << " " << i->second << endl;
	}
*/

	return;
}

void MyAI::pCheckW(multimap<int, int> &e, multimap<int, int> &p, multimap<int, int> &w, multimap<int, int> t){
/*	cout << "pCheckW: " << endl;
	map<int, int>::iterator i;
	cout << "  Before: " << endl;
	cout << "    TestPos:" << endl;
	for(i = t.begin(); i != t.end(); i++){
		cout << "\t" << i->first << " " << i->second << endl;
	}
	cout << "    explore:" << endl;
	for(i = e.begin(); i != e.end(); i++){
		cout << "\t" << i->first << " " << i->second << endl;
	}
	cout << "    unknownPit:" << endl;
	for(i = p.begin(); i != p.end(); i++){
		cout << "\t" << i->first << " " << i->second << endl;
	}
	cout << "    unknownWump:" << endl;
	for(i = w.begin(); i != w.end(); i++){
		cout << "\t" << i->first << " " << i->second << endl;
	}
*/	
	map<int, int>::iterator it;
	map<int, int>::iterator itr;
	map<int, int>::iterator i;
	
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
			/*	auto const& pit = p.equal_range(it->first);
				for(i = pit.first; i!= pit.second; i++){
					if(i->second == it->second){
						p.erase(i);
						break;
					}
				}
			*/
			}
		}
		if(found)
			break;
	}
	if(found){
//		cout << "erasing unknownPit Coord" << endl;
	
		auto const& wump = p.equal_range(xDel);
		for(i = wump.first; i!= wump.second; i++){
			if(i->second == yDel){
				p.erase(i);
				break;
			}
		}
	}
/*	cout << "  After: " << endl;
	cout << "    TestPos:" << endl;
	for(i = t.begin(); i != t.end(); i++){
		cout << "\t" << i->first << " " << i->second << endl;
	}
	cout << "    explore:" << endl;
	for(i = e.begin(); i != e.end(); i++){
		cout << "\t" << i->first << " " << i->second << endl;
	}
	cout << "    unknownPit:" << endl;
	for(i = p.begin(); i != p.end(); i++){
		cout << "\t" << i->first << " " << i->second << endl;
	}
	cout << "    unknownWump:" << endl;
	for(i = w.begin(); i != w.end(); i++){
		cout << "\t" << i->first << " " << i->second << endl;
	}
*/	
	return;
}

bool MyAI::getTarget(int x, int y, int dir, multimap<int, int> e, int& xD, int& yD, bool& n){
/*	cout << "getTarget: " << endl;
	map<int, int>:: iterator i;
	cout << "    explore:" << endl;
	for(i = e.begin(); i != e.end(); i++){
		cout << "\t" << i->first << " " << i->second << endl;
	}
*/	

	map<int, int>::iterator itT;
	int dist = 1000;
	
	if(dir == 1 || dir == 3){
		auto xFind = e.find(x);
		if(xFind != e.end()){
			auto const& xRange = e.equal_range(x);
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


Agent::Action MyAI::naviPath(int &x, int &y, int &dir, int xL, int yL, int xD, int yD,bool xW, bool yW, multimap<int, int> s, multimap<int, int> &pv){
	cout << "Navigate to path: " << endl;
	map<int, int>::iterator c;
	cout << "Safe: " << endl;
	for(c = s.begin(); c != s.end(); c++){
		cout << "\t" << c->first << " " << c->second << endl;
	}

	cout << "  Before: " << endl;
		

	multimap<int, int> t;
	if(yW){
		if((y+1) < yL)
			t.insert(pair<int, int> (x, (y + 1)));
	}
	else{
		if(y != yL)
			t.insert(pair<int, int> (x, (y + 1)));
	}
	if((y-1) >= 0)
		t.insert(pair<int, int> (x, (y - 1)));
	
	if(xW){
		if((x+1) < xL)
			t.insert(pair<int, int> ((x + 1), y));
	}
	else{
		if(x != xL)
			t.insert(pair<int, int> ((x + 1), y));
	}

	if((x-1) >= 0)
		t.insert(pair<int, int> ((x - 1), y));


	cout << "    Navi TestPos:" << endl;
	for(c = t.begin(); c != t.end(); c++){
		cout << "\t" << c->first << " " << c->second << endl;
	}

	
	map<int, int>::iterator it;
	map<int, int>::iterator itr;

	multimap<int, int> tSave;
	tSave.insert(t.begin(), t.end());
	exDelSB(t, pv);
	pv.insert(pair<int, int> (x, y));
	multimap<int, int> tPath;
	for(it = t.begin(); it != t.end(); it++){
		auto const& tPaths = s.equal_range(it->first);
		bool inSafe = false;
		int xPath;
		int yPath;
		for(itr = tPaths.first; itr != tPaths.second; itr++){
			if(itr->second == it->second){
				inSafe = true;
				xPath = it->first;
				yPath = it->second;
				break;
			}

		}

		if(inSafe){
			tPath.insert(pair<int, int> (xPath, yPath));
		}
	}
	
	if(tPath.size() == 0){
		pv.clear();
		pv.insert(pair<int, int>(x, y));
		
		for(it = tSave.begin(); it != tSave.end(); it++){
			auto const& tPaths = s.equal_range(it->first);
			bool inSafe = false;
			int xPath;
			int yPath;
			for(itr = tPaths.first; itr != tPaths.second; itr++){
				if(itr->second == it->second){
					inSafe = true;
					xPath = it->first;
					yPath = it->second;
					break;
				}

			}

			if(inSafe){
				tPath.insert(pair<int, int> (xPath, yPath));
			}
		}
		
	}

	t.clear();
	t.insert(tPath.begin(), tPath.end());
	tPath.clear();
	
	cout << "  After Safe: " << endl;
	//map<int, int>::iterator c;
	cout << "    Navi TestPos:" << endl;
	for(c = t.begin(); c != t.end(); c++){
		cout << "\t" << c->first << " " << c->second << endl;
	}
	
	
	int nextX = xD;
	int nextY = yD;
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
		if(dist != 1){
			int d = 10;
			for(it = t.begin(); it != t.end(); it++){
				int d2 = abs(xD - it->first) + abs(yD - it->second);

				if(d2 < d){
					d2 = d;
					nextX = it->first;
					nextY = it->second;
				}
			}
		}
	}
/**/	cout << "next step " << nextX << ", " << nextY << endl;

	if(x == nextX){
		if((nextY-y) == 1){
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
		else if((nextY-y) == -1){
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
		if((nextX-x) == 1){
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

void MyAI::wumpExp(int x, int y, int dir, int xL, int yL, int &xD, int &yD, bool xW, bool yW, multimap<int, int> s, multimap<int, int> &e, int xWump, int yWump){
	map<int, int>::iterator it;
	map<int, int>::iterator i;

	multimap<int, int> w;
	if(yW){
		if((yWump+1) < yL)
			w.insert(pair<int, int> (xWump, (yWump + 1)));
	}
	else{
		if(yWump != yL)
			w.insert(pair<int, int> (xWump, (yWump + 1)));
	}
	if((yWump-1) >= 0)
		w.insert(pair<int, int> (xWump, (yWump - 1)));
	
	if(xW){
		if((xWump+1) < xL)
			w.insert(pair<int, int> ((xWump + 1), yWump));
	}
	else{
		if(xWump != xL)
			w.insert(pair<int, int> ((xWump + 1), yWump));
	}

	if((xWump-1) >= 0)
		w.insert(pair<int, int> ((xWump - 1), yWump));

/**/	cout << "wumpExp: " << endl;
	cout << "  surrounding points" << endl; 
	for(i = w.begin(); i != w.end(); i++){
		cout << "\t" << i->first << " " << i->second << endl;
	}


	multimap<int, int> wPath;
	for(it = w.begin(); it != w.end(); it++){
		auto const& wPaths = s.equal_range(it->first);
		bool inSafe = false;
		int xPath;
		int yPath;
		for(i = wPaths.first; i != wPaths.second; i++){
			if(i->second == it->second){
				inSafe = true;
				xPath = it->first;
				yPath = it->second;
				break;
			}
		}

		if(inSafe){
			wPath.insert(pair<int, int> (xPath, yPath));
		}
	}
	w.clear();
	w.insert(wPath.begin(), wPath.end());

	cout<< " After safe:" << endl;
 	for(i = w.begin(); i != w.end(); i++){
		cout << "\t" << i->first << " " << i->second << endl;
	}


	addOnly(e, w);

	int dist = 1000;
	if(dir == 1 || dir == 3){
		auto xFind = w.find(x);
		if(xFind != w.end()){
			auto const& xRange = w.equal_range(x);
			for(it = xRange.first; it != xRange.second; it++){
		
				int d = abs(it->first - x) + abs(it->second -y);

				if(d < dist){
					xD = it->first;
					yD = it->second;
					dist = d;
				}

			}
			
			return;	
		}
	}
	if(dir == 0 || dir == 2){

		multimap<int, int> yRange;

		for(it = w.begin(); it != w.end(); it++){
			if(it->second == y)
				yRange.insert(pair<int, int>(it->first, it->second));
		}
		if(yRange.size() != 0){
			for(it = yRange.begin(); it != yRange.end(); it++){
				int d = abs(it->first - x) + abs(it->second -y);

				if(d < dist){
					xD = it->first;
					yD = it->second;
					dist = d;
				}
			}
			return;	
		}
	}
	
	for(it = w.begin(); it != w.end(); it++){
		
		int d = abs(it->first - x) + abs(it->second -y);
		if(d < dist){
			dist = d;
			xD = it->first;
			yD = it->second;
		}
	}
	
	return;
}


Agent::Action MyAI::wumpHunt(int &x, int &y, int &dir, int xL, int yL, bool xW, bool yW, multimap<int, int> s, int xWump, int yWump, bool &a){
	
	map<int, int>::iterator it;
	map<int, int>::iterator i;
	

	if(x == xWump){	//change directions to shoot
		if((yWump - y) >= 1){
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
	else if(y == yWump){
		if((xWump - x) >= 1){
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
		else if((xWump - x) <= -1){
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

	multimap<int, int> t;
	if(yW){
		if((y+1) < yL)
			t.insert(pair<int, int> (x, (y + 1)));
	}
	else{
		if(y != yL)
			t.insert(pair<int, int> (x, (y + 1)));
	}
	if((y-1) >= 0)
		t.insert(pair<int, int> (x, (y - 1)));
	
	if(xW){
		if((x+1) < xL)
			t.insert(pair<int, int> ((x + 1), y));
	}
	else{
		if(x != xL)
			t.insert(pair<int, int> ((x + 1), y));
	}

	if((x-1) >= 0)
		t.insert(pair<int, int> ((x-1), y));

	multimap<int, int> tPath;
	for(it = t.begin(); it != t.end(); it++){
		auto const& tPaths = s.equal_range(it->first);
		bool inSafe = false;
		int xPath;
		int yPath;
		for(i = tPaths.first; i != tPaths.second; i++){
			if(i->second == it->second){
				inSafe = true;
				xPath = it->first;
				yPath = it->second;
				break;
			}

		}

		if(inSafe){
			tPath.insert(pair<int, int> (xPath, yPath));
		}
	}
	t.clear();
	t.insert(tPath.begin(), tPath.end());

	//Get to closest tile in line with wumpus
	int dist = 1000;
	int nextX;
	int nextY;

	auto xLine = s.find(xWump); //find a key with x value in safe list
	if(xLine != s.end()){
		for(it = t.begin(); it != t.end(); it++){
			int d = abs(it->first - xWump);
			if(d < dist){
				dist = d;
				nextX = it->first;
				nextY = it->second;
				break;	
			}
		}
	}
	
	multimap<int, int> yRange;
	for(it = s.begin(); it != s.end(); it++){//find a key with y value in safe list
		if(it->second == yWump){
			yRange.insert(pair<int, int> (it->first, it->second));
		}
	}


	if(yRange.size() != 0){
		for(it = t.begin(); it != t.end(); it++){
			auto const& yLine = yRange.equal_range(it->first);
			
			for(i = yLine.first; i != yLine.second; i++){
				int d = abs(it->second - yWump);

				if(d < dist){
					dist = d;
					nextX = it->first;
					nextY = it->second;
					break;
				}
			}
		}
	}

	//go to position
	if(x == nextX){
		if((nextY-y) == 1){
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
		else if((nextY-y) == -1){
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
		if((nextX-x) == 1){
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
