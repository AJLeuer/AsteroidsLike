//
//  Player.h
//  World
//
//  Created by Adam James Leuer on 5/31/14.
//  Copyright (c) 2014 Adam James Leuer. All rights reserved.
//

#ifndef __GameWorld__Player__
#define __GameWorld__Player__

#include <iostream>
#include <sstream>
#include <chrono>

#include "../Util/Util2.h"

#include "../World/GameInterface.h"
#include "../World/PlayerCharacter.h"
#include "../World/GameState.h"

#include "../Control/DefaultConfig.h"
#include "../Control/Input.hpp"

using namespace std ;

enum class PlayerDefaults {
	playerDefaults0 = 0,
	playerDefaults1 = 1
	//etc
};

class Player /*: public GameInterface*/ {

protected:

	string name ;
	PlayerCharacter playerCharacter ;
	
	/* just for initializing playerCharacter */
	AssetType pcAssetType = AssetType::playerShip ; /* change if needed */
	
	float size = 1.00 ;
	
	PlayerDefaults playerDefaults ;
	

	void registerForCallbacks() ;

public:
	
	Player() ;
	
	Player(const string & name, const string & imageFile, PlayerDefaults playerDefaults) ;

	/* implementing these just so we're not considered an abstract class */
	void operator()() {}
	
	void update() ;

	
	
	
	
	
	
} ;

#endif /* defined(__GameWorld__Player__) */
