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
	playerCharacter = Character(pcAssetType, "/Assets/Blocks/Blocks_01_256x256_Alt_02_002.png" /* AssetFileIO::getRandomImageFilename(pcAssetType) */, 1.0, Pos2<float>(randPosSetter<float>, defaultCheck<float>), name, Reaction::friendly, DoA::alive, CharacterState::normal,
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
	
	KeyInputRegister * moveUpKey = new KeyInputRegister(this, (&GameInterface::moveUp), MOVE_UP, SDLK_UP) ;
	KeyInputRegister * moveDownKey = new KeyInputRegister(this, (&GameInterface::moveDown), MOVE_DOWN, SDLK_DOWN) ;
	KeyInputRegister * moveLeftKey = new KeyInputRegister(this, (&GameInterface::moveLeft), MOVE_LEFT, SDLK_LEFT) ;
	KeyInputRegister * moveRightKey = new KeyInputRegister(this, (&GameInterface::moveRight), MOVE_RIGHT, SDLK_RIGHT) ;
	
	KeyInputRegister * jumpKey = new KeyInputRegister(this, (&GameInterface::jump), SDLK_SPACE) ;

	InputController::registerForKeypress(moveUpKey) ;
	InputController::registerForKeypress(moveDownKey) ;
	InputController::registerForKeypress(moveLeftKey) ;
	InputController::registerForKeypress(moveRightKey) ;
	
	InputController::registerForKeypress(jumpKey) ;
}