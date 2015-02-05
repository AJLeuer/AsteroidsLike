//
//  Player.h
//  World
//
//  Created by Adam James Leuer on 5/31/14.
//  Copyright (c) 2014 Adam James Leuer. All rights reserved.
//

#ifndef __SpriteFight__Player__
#define __SpriteFight__Player__

#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <chrono>
#include <thread>

#include "../Util/Util.hpp"
#include "../Util/Util2.h"
#include "../Util/BoundsCheck.hpp"
#include "../Util/Random.hpp"

#include "../Output/GraphicsData.hpp"
#include "../Output/TextOutput.hpp"

#include "../World/GameInterface.h"
#include "../World/PlayerCharacter.h"
#include "../World/GameState.hpp"

#include "../Control/Configuration.h"
#include "../Control/Input.hpp"

using namespace std ;


class Player /*: public GameInterface*/ {

protected:
	
	static unsigned IDs ;
	
	static AssetType defaultPCAssetType ; /* change if needed */
	static float defaultSize ;
	
	unsigned ID ;
	string name ;
	PlayerCharacter playerCharacter ;
	
	
	/* just for initializing playerCharacter */
	
	static Pos2<float> position_in_defaultStartingArea() ;
	
	void setNames() ;
	
	void registerForCallbacks() ;
	
	

public:
	
	/* Players with default settings, for convenience */
	static Player * defaultPlayer0 ;
	static Player * defaultPlayer1 ;
	
	static void initDefaultPlayers() ;
	
	Player() ;
	
	Player(const string & name, const string & playerCharacter_imageFilename,
		   float playerCharacter_size, const Pos2<float> & playerCharacter_loc, const Angle playerCharacter_rotation,
		   const string & playerCharacter_name, Reaction playerCharacter_reaction, DoA playerCharacter_alive,
		   CharacterState playerCharacter_state, unsigned playerCharacter_health, unsigned playerCharacter_damage,
		   const AssetFile & projectileImageFile) ;
	
	~Player() { InputController::deregister(&playerCharacter) ; }
	
	/**
	 * @brief Creates a text display of the Player's velocity onscreen.
	 *
	 * @param pos The position (x, y) onscreen where the text will be shown
	 * @param foreground The color of the text
	 * @param background The color of the background
	 */
	void displayVelocity(Position<float> pos, GameColor foreground, GameColor background) ;

	/* implementing these just so we're not considered an abstract class */
	void operator()() {}
	
	void update() ;

	

} ;

#endif /* defined(__SpriteFight__Player__) */
