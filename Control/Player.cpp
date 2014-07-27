//
//  Player.cpp
//  World
//
//  Created by Adam James Leuer on 5/31/14.
//  Copyright (c) 2014 Adam James Leuer. All rights reserved.
//

#include "Player.h"

using namespace std ;

unsigned Player::IDs = 0 ;


AssetType Player::defaultPCAssetType = AssetType::playerShip ; /* change if needed */
float Player::defaultSize = 1.00 ;

Player * Player::defaultPlayer0 = nullptr ;
Player * Player::defaultPlayer1 = nullptr ;

Pos2<float> Player::position_in_defaultStartingArea() {
	Pos2<float> ret = Pos2<float>((globalMaxX() / 2) + FastRand<float>::defaultRandom(-300, 300), (globalMaxY() - (globalMaxY() * 0.2))) ;
	return ret ;
}

void Player::initDefaultPlayers() {
	defaultPlayer0 = new Player("Player 0", "Ship1_Green.png", defaultSize, position_in_defaultStartingArea(), TWO_SEVENTY_DEGREES, "Green",
                                Reaction::friendly, DoA::nodoa, CharacterState::idle, 500, 100, AssetFile::projectileImageFilenames->at(0)) ;
	
	defaultPlayer1 = new Player("Player 1", "Ship0_Red.png", defaultSize, position_in_defaultStartingArea(), NINETY_DEGREES, "Red",
                                Reaction::friendly, DoA::nodoa, CharacterState::idle, 500, 100, AssetFile::projectileImageFilenames->at(1)) ;
}

Player::Player() :
	ID(IDs),
	name(""),
	playerCharacter()
{
	IDs++ ;
	setNames() ;
	registerForCallbacks() ;
}

Player::Player(const string & name, const string & playerCharacter_imageFilename,
	float playerCharacter_size, const Pos2<float> & playerCharacter_loc, const Angle<float> & playerCharacter_rotation, const string & playerCharacter_name,
	Reaction playerCharacter_reaction, DoA playerCharacter_alive, CharacterState playerCharacter_state,
	unsigned playerCharacter_health, unsigned playerCharacter_damage, const AssetFile & projectileImageFile) :

	ID(IDs),
	name(name),
	playerCharacter(playerCharacter_imageFilename, playerCharacter_size,
		playerCharacter_loc, playerCharacter_rotation, playerCharacter_name, playerCharacter_reaction, playerCharacter_alive, playerCharacter_state,
		playerCharacter_health, playerCharacter_damage, true, projectileImageFile)
{
	IDs++ ;
	setNames() ;
	registerForCallbacks() ;
}

void Player::update() {
	//todo
}

void Player::setNames() {
	if (name == "") {
		name = "Player " + to_string(ID) ;
	}
	if (playerCharacter.getName() == "") {
		playerCharacter.setName(name + "'s Character") ;
	}
}

void Player::registerForCallbacks() {
	
	//todo make more generic
	
	KeyInputRegister * onKeyJump ;
	KeyInputRegister * onKeyMoveUp ;
	KeyInputRegister * onKeyMoveDown ;
	KeyInputRegister * onKeyMoveLeft ;
	KeyInputRegister * onKeyMoveRight ;
    EventRegister * onKeyFire ;
	
	if (playerCharacter.getColor() == Colors::green) {
        
		onKeyMoveUp = new KeyInputRegister(&playerCharacter, (&GameInterface::moveUp),
															{MOVE_UP_KEY}, {SDLK_UP}, KeypressEvaluationMethod::any) ;
		onKeyMoveDown = new KeyInputRegister(&playerCharacter, (&GameInterface::moveDown),
															{MOVE_DOWN_KEY}, {SDLK_DOWN}, KeypressEvaluationMethod::any) ;
		onKeyMoveLeft = new KeyInputRegister(&playerCharacter, (&GameInterface::moveLeft),
															  {MOVE_LEFT_KEY}, {SDLK_LEFT}, KeypressEvaluationMethod::any) ;
		onKeyMoveRight = new KeyInputRegister(&playerCharacter, (&GameInterface::moveRight),
															   {MOVE_RIGHT_KEY}, {SDLK_RIGHT}, KeypressEvaluationMethod::any) ;
        
        onKeyJump = new KeyInputRegister(&playerCharacter, (&GameInterface::jump),
                                                            {JUMP_KEY}, KeypressEvaluationMethod::exactlyOne) ; /* ' ' i.e. underlying value of SDL keycode for space */
		
		onKeyFire = new EventRegister(&playerCharacter, &GameInterface::fire, SDL_MOUSEBUTTONDOWN) ;

	}
	else if (playerCharacter.getColor() == Colors::red) {
		
		onKeyMoveUp = new KeyInputRegister(&playerCharacter, (&GameInterface::moveUp),
															{SDLK_KP_8}, KeypressEvaluationMethod::exactlyOne) ;
		onKeyMoveDown = new KeyInputRegister(&playerCharacter, (&GameInterface::moveDown),
															  {SDLK_KP_2}, KeypressEvaluationMethod::exactlyOne) ;
		onKeyMoveLeft = new KeyInputRegister(&playerCharacter, (&GameInterface::moveLeft),
															  {SDLK_KP_4}, KeypressEvaluationMethod::exactlyOne) ;
		onKeyMoveRight = new KeyInputRegister(&playerCharacter, (&GameInterface::moveRight),
															   {SDLK_KP_6}, KeypressEvaluationMethod::exactlyOne) ;
        
        onKeyJump = new KeyInputRegister(&playerCharacter, (&GameInterface::jump),
                                                            {SDLK_KP_0}, KeypressEvaluationMethod::exactlyOne) ;
        
        onKeyFire = new EventRegister(&playerCharacter, &GameInterface::fire, SDL_MOUSEWHEEL) ;
		
	}
	
	InputController::registerForKeypress(onKeyJump) ;
	InputController::registerForKeypress(onKeyMoveUp) ;
	InputController::registerForKeypress(onKeyMoveDown) ;
	InputController::registerForKeypress(onKeyMoveLeft) ;
	InputController::registerForKeypress(onKeyMoveRight) ;
	InputController::registerForEvent(onKeyFire) ;

}

void Player::displayVelocity(Position<float> pos, GameColor foreground, GameColor background) {
	
	stringstream stream0 ;
	
	auto lastVelocity = playerCharacter.getVector()->getVelocity()->getValue() ;
	
	stream0 << "Player " << this->ID << "'s " << *playerCharacter.getVector()->getVelocity() ;
	
	string str(stream0.str()) ;
	
	
	auto velocityTextDisplayUpdater = [=] () mutable -> const string {
		
		stringstream stream1 ;
		
		auto currentVelocity = playerCharacter.getVector()->getVelocity()->getValue() ;
		

		stream1 << "Player " << this->ID << "'s " << *playerCharacter.getVector()->getVelocity() ;
			
		str = stream1.str() ;
			
		lastVelocity = currentVelocity ;
			
		return str ;

	} ;
	
	TextOutput<float, int>::displayContinuousText(velocityTextDisplayUpdater, pos, ZERO_DEGREES, foreground, background) ;
}







