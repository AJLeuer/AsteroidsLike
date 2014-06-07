//
//  Player.cpp
//  GameWorld
//
//  Created by Adam James Leuer on 5/31/14.
//  Copyright (c) 2014 Adam James Leuer. All rights reserved.
//

#include "Player.h"

using namespace std ;

Player::Player() :
	name("Player 1")
{
	playerCharacter = Character(pcAssetType, AssetFileIO::getRandomImageFilename(pcAssetType), 1.0, Pos2<float>(randPosSetter<float>, defaultCheck<float>), name, Reaction::friendly, DoA::alive, CharacterState::normal,
								500, 100) ;
	
	registerForCallbacks() ;
}

Player::Player(const string & name, const string & imageFile) :
	name(name)
{
	playerCharacter = Character(pcAssetType, imageFile, 1.0,
								Pos2<float>(50, 650, 0), name, Reaction::friendly, DoA::alive, CharacterState::normal, 500, 100) ;
	registerForCallbacks() ;
}

void Player::registerForCallbacks() {
	KeyInputRegister * moveUpRegister = new KeyInputRegister(this, (&GameInterface::moveUp), "W", SDLK_UP) ;
	KeyInputRegister * moveDownRegister = new KeyInputRegister(this, (&GameInterface::moveDown), "S",SDLK_DOWN) ;
	KeyInputRegister * moveLeftRegister = new KeyInputRegister(this, (&GameInterface::moveLeft), "A", SDLK_LEFT) ;
	KeyInputRegister * moveRightRegister = new KeyInputRegister(this, (&GameInterface::moveRight), "D", SDLK_RIGHT) ;

	InputController::registerForKeypress(moveUpRegister) ;
	InputController::registerForKeypress(moveDownRegister) ;
	InputController::registerForKeypress(moveLeftRegister) ;
	InputController::registerForKeypress(moveRightRegister) ;
}