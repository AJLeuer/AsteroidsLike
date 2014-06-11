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
#include <sstream>
#include <chrono>

#include "../GameWorld/GameInterface.h"
#include "../GameWorld/Character.h"
#include "../GameWorld/GameState.h"

#include "../Control/Input.hpp"

using namespace std ;

class Player : public GameInterface {

protected:

	string name ;
	Character playerCharacter ;
	
	/* just for initializing playerCharacter */
	AssetType pcAssetType = AssetType::character ; /* change if needed */
	

	void registerForCallbacks() ;

public:
	
	Player() ;
	
	Player(const string & name, const string & imageFile) ;

	/* implementing these just so we're not considered an abstract class */
	void operator()() {}
	
	void moveUp() { playerCharacter.moveUp(defaultOffset<float>) ; /* Debug */ printPositition() ; /* end debug */ }
	void moveDown() { playerCharacter.moveDown(defaultOffset<float>) ; /* Debug */ printPositition() ; /* end debug */ }
	void moveRight() { playerCharacter.moveRight(defaultOffset<float>) ; /* Debug */ printPositition() ; /* end debug */ }
	void moveLeft() { playerCharacter.moveLeft(defaultOffset<float>) ; /* Debug */ printPositition() ; /* end debug */ }
	
	void printPositition() ;
	
	void jump() ;
	
	
} ;

#endif /* defined(__GameWorld__Player__) */
