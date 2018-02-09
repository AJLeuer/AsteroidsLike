//
//  Player.cpp
//  World
//
//  Created by Adam James Leuer on 5/31/14.
//  Copyright (c) 2014 Adam James Leuer. All rights reserved.
//

#include "Player.h"

using namespace std ;


/* Initialized to -1, but each player will increment it before copying its
 * value as their own id, so the first player will still have id 0
 */
long long Player::IDs = -1 ;

Player * Player::defaultPlayer0 = nullptr ;
Player * Player::defaultPlayer1 = nullptr ;

const unsigned long Player::getNumberOfPlayers() {
	return Player::IDs + 1 ;
}

Vect<float> Player::position_in_defaultStartingArea() {
	Vect<float> ret = Vect<float>((globalMaxX() / 2) + Randm<float>::defaultRandom(-300, 300), (globalMaxY() - (globalMaxY() * 0.2))) ;
	return ret ;
}


Player::Player() :
	ID(assignID()),
	name(""),
	playerCharacter()
{
	setNames() ;
	registerForCallbacks() ;
}

Player::Player(const string & name, const string & playerCharacter_imageFilename,
	float playerCharacter_size, const Vect<float> & playerCharacter_loc, const Angle playerCharacter_rotation, const string & playerCharacter_name,
	unsigned playerCharacter_health, unsigned playerCharacter_damage, const AssetFile & projectileImageFile) :

	ID(assignID()),
	name(name),
	playerCharacter(playerCharacter_imageFilename, playerCharacter_size,
	playerCharacter_loc, playerCharacter_rotation, playerCharacter_name,
	playerCharacter_health, playerCharacter_damage, Util::SafeBoolean::t, true, projectileImageFile)
{
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
		playerCharacter.setName(name + "'s Pawn") ;
	}
}

void Player::registerForCallbacks() {
	
	//todo make more generic
	
	
    KeyInputRegister * onKeyMoveUp ;
	KeyInputRegister * onKeyMoveDown ;
	KeyInputRegister * onKeyMoveLeft ;
	KeyInputRegister * onKeyMoveRight ;
    KeyInputRegister * onKeyRotateCounterClockwise ;
    KeyInputRegister * onKeyRotateClockwise ;
    KeyInputRegister * onKeyJump ;
    EventRegister * onActionFire ;
	
	if (getNumberOfPlayers() == 1) {
        
        onKeyMoveUp = new KeyInputRegister(&playerCharacter, (&GameInterface::moveUp),
                                                            {MOVE_UP_KEY}, {SDLK_UP}, KeypressEvaluationMethod::any) ;
        onKeyMoveDown = new KeyInputRegister(&playerCharacter, (&GameInterface::moveDown),
															{MOVE_DOWN_KEY}, {SDLK_DOWN}, KeypressEvaluationMethod::any) ;
		onKeyMoveLeft = new KeyInputRegister(& playerCharacter, (& GameInterface::moveLeft),
															  {MOVE_LEFT_KEY}, {SDLK_LEFT}, KeypressEvaluationMethod::any) ;
		onKeyMoveRight = new KeyInputRegister(& playerCharacter, (& GameInterface::moveRight),
															   {MOVE_RIGHT_KEY}, {SDLK_RIGHT}, KeypressEvaluationMethod::any) ;
        
        onKeyRotateCounterClockwise = new KeyInputRegister(& playerCharacter, (& GameInterface::rotateCounterClockwise),
                                         {"Q"}, KeypressEvaluationMethod::any) ;
        
        onKeyRotateClockwise = new KeyInputRegister(& playerCharacter, (& GameInterface::rotateClockwise),
                                                    {"E"}, KeypressEvaluationMethod::any) ;
        
        onKeyJump = new KeyInputRegister(& playerCharacter, (& GameInterface::jump),
                                                            {JUMP_KEY}, KeypressEvaluationMethod::exactlyOne) ; /* ' ' i.e. underlying value of SDL keycode for space */
		
		onActionFire = new EventRegister(& playerCharacter, &GameInterface::fire, SDL_MOUSEBUTTONDOWN) ;

	}
	else /* if (getNumberOfPlayers() > 2) */ {
		cerr << "The maximum supported number of players is 1." << endl ;
		throw exception() ;
	}
	
	InputControl::registerForKeypress(onKeyJump) ;
    InputControl::registerForKeypress(onKeyMoveUp) ;
	InputControl::registerForKeypress(onKeyMoveDown) ;
	InputControl::registerForKeypress(onKeyMoveLeft) ;
	InputControl::registerForKeypress(onKeyMoveRight) ;
	InputControl::registerForKeypress(onKeyRotateCounterClockwise) ;
	InputControl::registerForKeypress(onKeyRotateClockwise) ;
	InputControl::registerForEvent(onActionFire) ;
}

void Player::displayVelocity(Vect<float> pos, GameColor foreground, GameColor background) {
	
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

	
	ContinuousTextOutput<float, int> * displayVelocityContinuously =
		new ContinuousTextOutput<float, int>(velocityTextDisplayUpdater, pos, 0.0, foreground, background) ;
	
	displayVelocityContinuously->display() ;
}







