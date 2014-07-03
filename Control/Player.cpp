//
//  Player.cpp
//  World
//
//  Created by Adam James Leuer on 5/31/14.
//  Copyright (c) 2014 Adam James Leuer. All rights reserved.
//

#include "Player.h"

using namespace std ;

Player::Player() :
	name("Player 1")
{
    auto x = (globalMaxX() / 2) ;
    auto y = (globalMaxY() - (globalMaxY() * 0.25)) ;
	playerCharacter = PlayerCharacter(pcAssetType, AssetFileIO::getRandomImageFilename(pcAssetType), size, Pos2<float>(x, y, 0), /* Pos2<float>(randPositionSetter<float>(), defaultCheck<float>()) */ name, Reaction::friendly, DoA::alive, CharacterState::normal,
        500, 100) ;
	
	registerForCallbacks() ;
}

Player::Player(const string & name, const string & imageFile) :
	name(name)
{
    auto x = (globalMaxX() / 2) ;
    auto y = (globalMaxY() - (globalMaxY() * 0.25)) ;
	playerCharacter = PlayerCharacter(pcAssetType, imageFile, size,
								Pos2<float>(x, y, 0), name, Reaction::friendly, DoA::alive, CharacterState::normal, 500, 100) ;
	registerForCallbacks() ;
}

void Player::update() {
	//todo
}



void Player::registerForCallbacks() {
	
	KeyInputRegister * moveUpKey = new KeyInputRegister(&playerCharacter, (&GameInterface::moveUp), {MOVE_UP}, {SDLK_UP}, KeypressEvaluationMethod::any) ;
	KeyInputRegister * moveDownKey = new KeyInputRegister(&playerCharacter, (&GameInterface::moveDown), {MOVE_DOWN}, {SDLK_DOWN}, KeypressEvaluationMethod::any) ;
	KeyInputRegister * moveLeftKey = new KeyInputRegister(&playerCharacter, (&GameInterface::moveLeft), {MOVE_LEFT}, {SDLK_LEFT}, KeypressEvaluationMethod::any) ;
	KeyInputRegister * moveRightKey = new KeyInputRegister(&playerCharacter, (&GameInterface::moveRight), {MOVE_RIGHT}, {SDLK_RIGHT}, KeypressEvaluationMethod::any) ;
	KeyInputRegister * moveUpLeftKey = new KeyInputRegister(&playerCharacter, (&GameInterface::moveUpLeft), {"Q"}, KeypressEvaluationMethod::any) ;
	KeyInputRegister * moveUpRightKey = new KeyInputRegister(&playerCharacter, (&GameInterface::moveUpRight), {"E"}, KeypressEvaluationMethod::any) ;
	
	KeyInputRegister * jumpKey = new KeyInputRegister(&playerCharacter, (&GameInterface::jump), {SDLK_SPACE}, KeypressEvaluationMethod::any) ;
	
	
	InputController::registerForKeypress(moveUpKey) ;
	InputController::registerForKeypress(moveDownKey) ;
	InputController::registerForKeypress(moveLeftKey) ;
	InputController::registerForKeypress(moveRightKey) ;
	InputController::registerForKeypress(moveUpLeftKey) ;
	InputController::registerForKeypress(moveUpRightKey) ;
	
	InputController::registerForKeypress(jumpKey) ;
}

