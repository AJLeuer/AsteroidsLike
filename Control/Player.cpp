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
	KeyInputRegister * moveUpRegister = new KeyInputRegister({"W", "8"}, this, (&GameInterface::moveUp)) ;
	KeyInputRegister * moveDownRegister = new KeyInputRegister({"S", "2"}, this, (&GameInterface::moveDown)) ;
	KeyInputRegister * moveLeftRegister = new KeyInputRegister({"A", "4"}, this, (&GameInterface::moveLeft)) ;
	KeyInputRegister * moveRightRegister = new KeyInputRegister({"D", "6"}, this, (&GameInterface::moveRight)) ;

	InputController::registerForKeypress(moveUpRegister) ;
	InputController::registerForKeypress(moveDownRegister) ;
	InputController::registerForKeypress(moveLeftRegister) ;
	InputController::registerForKeypress(moveRightRegister) ;
}