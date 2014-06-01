//
//  Player.cpp
//  GameWorld
//
//  Created by Adam James Leuer on 5/31/14.
//  Copyright (c) 2014 Adam James Leuer. All rights reserved.
//

#include "Player.h"

using namespace std ;

Player::Player(const string & name, const string & imageFile) :
	name(name),
	playerCharacter(AssetType::block, imageFile, 0.5,
	Pos2<float>(50, 650, 0), name, Reaction::friendly, DoA::alive, CharacterState::normal,
		500, 100)
{
	registerForCallbacks() ;
}

void Player::registerForCallbacks() {
	KeyInputRegister * moveUpRegister = new KeyInputRegister(this, (&GameInterface::moveUp), {"W", "8"}) ;
	KeyInputRegister * moveDownRegister = new KeyInputRegister(this, (&GameInterface::moveDown), {"S", "2"}) ;
	KeyInputRegister * moveLeftRegister = new KeyInputRegister(this, (&GameInterface::moveLeft), {"A", "4"}) ;
	KeyInputRegister * moveRightRegister = new KeyInputRegister(this, (&GameInterface::moveRight), {"D", "6"}) ;

	InputController::registerForKeypress(moveUpRegister) ;
	InputController::registerForKeypress(moveDownRegister) ;
	InputController::registerForKeypress(moveLeftRegister) ;
	InputController::registerForKeypress(moveRightRegister) ;
}