//
//  Message.h
//  GameWorld
//
//  Created by Adam James Leuer on 3/11/14.
//  Copyright (c) 2014 Adam James Leuer. All rights reserved.
//

#ifndef __GameWorld__Message__
#define __GameWorld__Message__

#include <iostream>
#include "Location.h"

using namespace std ;

/* Note for all enums here: we will use -1
 as our equivelent to NULL */
enum Alert {
	danger = 0,
	nearbyEnemy = 1,
	lowHealth = 2,
	lowEnergy = 3, /*i.e. for abilities*/
	lowResource = 4,
	nearbyMoney = 5,
	nearbyBoost = 6,
	//add more here
} ;

enum Status {
	/*typically for player and NPCs*/
	normal = 0,
	inCombat = 1,
	inDialogue = 2,
	idle = 3,
} ;

enum Attitude {
	/*typically npc*/
	friendly = 0,
	neutral = 1,
	hostile = 2
	//add more here
} ;


enum Misc {
	//anything else?
} ;

struct Message {
	Alert alert ;
	Status status ;
	Misc misc ;
	
	string messageText ;
	string messageData ;
	double numericalData ;
	
	Message() : alert(Alert(-1)), status(Status(-1)),
		misc(Misc(-1)), messageText(NULL),
		messageData(NULL), numericalData(NULL) {}
	
	Message(Alert a, Status s, Misc c, string mT, string mD, double nD) : alert(a), status(s),
		misc(c), messageText(mT),
		messageData(mD), numericalData(nD) {}
	
	
} ;

#endif /* defined(__GameWorld__Message__) */
