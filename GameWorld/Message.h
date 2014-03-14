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
#include "GameData.h"

using namespace std ;


struct Message {
	
	
	Alert alert ;
	CharacterState senderState ;  //or maybe a different character's state...
	Misc misc ;
	

	string messageText ;
	string messageData ;
	double numericalData ;
	
	
	Message() : alert(Alert(-1)), senderState(CharacterState(-1)),
		misc(Misc(-1)), messageText(NULL),
		messageData(NULL), numericalData(NULL) {}
	
	
	Message(Alert a, CharacterState s, Misc c, string mT, string mD, double nD) : alert(a), senderState(s),
		misc(c), messageText(mT),
		messageData(mD), numericalData(nD) {}
	
	
} ;

#endif /* defined(__GameWorld__Message__) */
