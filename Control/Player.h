//
//  Player.h
//  GameWorld
//
//  Created by Adam James Leuer on 5/31/14.
//  Copyright (c) 2014 Adam James Leuer. All rights reserved.
//

#ifndef __GameWorld__Player__
#define __GameWorld__Player__

#include <iostream>

#include "../GameWorld/GameInterface.h"
#include "../GameWorld/Character.h"

#include "../Control/Input.hpp"

using namespace std ;

class Player : public GameInterface {

protected:

	string name ;
	Character playerCharacter ;

	void registerForCallbacks() ;

public:

	Player(const string & name = "Player 1", const string & imageFile = "/Assets/Blocks/Blocks_01_256x256_Alt_02_006.png") ;

	/* implementing these just so we're not considered an abstract class */
	void operator()() {}

	void moveRight() { playerCharacter.moveRight() ; }
	void moveLeft() { playerCharacter.moveLeft() ; }
	void moveUp() { playerCharacter.moveUp() ; }
	void moveDown() { playerCharacter.moveDown() ; }



} ;

#endif /* defined(__GameWorld__Player__) */
