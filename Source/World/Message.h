//
//  Message.h
//  World
//
//  Created by Adam James Leuer on 3/11/14.
//  Copyright (c) 2014 Adam James Leuer. All rights reserved.
//

#ifndef __SpriteFight__Message__
#define __SpriteFight__Message__

#include <iostream>

#include "CharacterData.h"


using namespace std ;


struct Message {
	
	
	Alert alert ;
	CharacterState state ;  //maybe the sending character's state...
	

	string messageText ;
	double numericalData ;
	
	
	Message() : alert(Alert(-1)), state(CharacterState(-1)),
		messageText(""),
		numericalData(0) {}
	
	
	Message(Alert a, CharacterState s, string mT, double nD) : alert(a), state(s),
		messageText(mT),
		numericalData(nD) {}
	
	
} ;

#endif /* defined(__SpriteFight__Message__) */