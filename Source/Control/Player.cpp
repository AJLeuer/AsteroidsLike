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

AssetType Player::defaultPCAssetType = AssetType::playerShip ; /* change if needed */
float Player::defaultGeometrySize = 0.50f ;

Player * Player::defaultPlayer0 = nullptr ;
Player * Player::defaultPlayer1 = nullptr ;

const unsigned long Player::getNumberOfPlayers() {
	return Player::IDs + 1 ;
}

Vect<float> Player::position_in_defaultStartingArea() {
	Vect<float> ret = Vect<float>((globalMaxX() / 2) + Randm<float>::defaultRandom(-300, 300), (globalMaxY() - (globalMaxY() * 0.2))) ;
	return ret ;
}


void Player::initDefaultPlayers() {
	defaultPlayer0 = new Player("Player 0", "Ship1_Blue.png", defaultGeometrySize, position_in_defaultStartingArea(), 0.0, "Blue",
                                Reaction::friendly, DoA::nodoa, CharacterState::idle, 500, 100, AssetFile::projectileImageFilenames->at(0)) ;
	
	defaultPlayer1 = new Player("Player 1", "Ship1_Green.png", defaultGeometrySize, position_in_defaultStartingArea(), 0.0, "Green",
                                Reaction::friendly, DoA::nodoa, CharacterState::idle, 500, 100, AssetFile::projectileImageFilenames->at(1)) ;
}

Player::Player() :
	ID(IDs++),
	name(""),
	playerCharacter()
{
	setNames() ;
	registerForCallbacks() ;
}

Player::Player(const string & name, const string & playerCharacter_imageFilename,
	float playerCharacter_size, const Vect<float> & playerCharacter_loc, const Angle playerCharacter_rotation, const string & playerCharacter_name,
	Reaction playerCharacter_reaction, DoA playerCharacter_alive, CharacterState playerCharacter_state,
	unsigned playerCharacter_health, unsigned playerCharacter_damage, const AssetFile & projectileImageFile) :

	ID(IDs++),
	name(name),
	playerCharacter(playerCharacter_imageFilename, playerCharacter_size,
		playerCharacter_loc, playerCharacter_rotation, playerCharacter_name, playerCharacter_reaction, playerCharacter_alive, playerCharacter_state,
		playerCharacter_health, playerCharacter_damage, SafeBoolean::t, true, projectileImageFile)
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
		playerCharacter.setName(name + "'s Character") ;
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
    EventRegister * onKeyFire ;
	
	if (getNumberOfPlayers() == 1) {
        
		onKeyMoveUp = new KeyInputRegister(&playerCharacter, (&GameInterface::moveUp),
															{MOVE_UP_KEY}, {SDLK_UP}, KeypressEvaluationMethod::any) ;
		onKeyMoveDown = new KeyInputRegister(&playerCharacter, (&GameInterface::moveDown),
															{MOVE_DOWN_KEY}, {SDLK_DOWN}, KeypressEvaluationMethod::any) ;
		onKeyMoveLeft = new KeyInputRegister(&playerCharacter, (&GameInterface::moveLeft),
															  {MOVE_LEFT_KEY}, {SDLK_LEFT}, KeypressEvaluationMethod::any) ;
		onKeyMoveRight = new KeyInputRegister(&playerCharacter, (&GameInterface::moveRight),
															   {MOVE_RIGHT_KEY}, {SDLK_RIGHT}, KeypressEvaluationMethod::any) ;
        
        onKeyRotateCounterClockwise = new KeyInputRegister(&playerCharacter, (&GameInterface::rotateCounterClockwise),
                                         {"Q"}, KeypressEvaluationMethod::any) ;
        
        onKeyRotateClockwise = new KeyInputRegister(&playerCharacter, (&GameInterface::rotateClockwise),
                                                    {"E"}, KeypressEvaluationMethod::any) ;
        
        onKeyJump = new KeyInputRegister(&playerCharacter, (&GameInterface::jump),
                                                            {JUMP_KEY}, KeypressEvaluationMethod::exactlyOne) ; /* ' ' i.e. underlying value of SDL keycode for space */
		
		onKeyFire = new EventRegister(&playerCharacter, &GameInterface::fire, SDL_MOUSEBUTTONDOWN) ;

	}
	else if (getNumberOfPlayers() == 2) {
		
		onKeyMoveUp = new KeyInputRegister(&playerCharacter, (&GameInterface::moveUp),
															{SDLK_KP_8}, KeypressEvaluationMethod::exactlyOne) ;
		onKeyMoveDown = new KeyInputRegister(&playerCharacter, (&GameInterface::moveDown),
															  {SDLK_KP_2}, KeypressEvaluationMethod::exactlyOne) ;
		onKeyMoveLeft = new KeyInputRegister(&playerCharacter, (&GameInterface::orientationDependentLeftRightMove),
															  {SDLK_KP_4}, KeypressEvaluationMethod::exactlyOne) ;
		onKeyMoveRight = new KeyInputRegister(&playerCharacter, (&GameInterface::orientationDependentRightLeftMove),
															   {SDLK_KP_6}, KeypressEvaluationMethod::exactlyOne) ;
        
        onKeyRotateCounterClockwise = new KeyInputRegister(&playerCharacter, (&GameInterface::rotateCounterClockwise),
                                                           {SDLK_KP_7}, KeypressEvaluationMethod::any) ;
        
        onKeyRotateClockwise = new KeyInputRegister(&playerCharacter, (&GameInterface::rotateClockwise),
                                                    {SDLK_KP_9}, KeypressEvaluationMethod::any) ;
        
        onKeyJump = new KeyInputRegister(&playerCharacter, (&GameInterface::jump),
                                                            {SDLK_KP_0}, KeypressEvaluationMethod::exactlyOne) ;
        
        onKeyFire = new EventRegister(&playerCharacter, &GameInterface::fire, SDL_MOUSEWHEEL) ;
		
	}
	else if (getNumberOfPlayers() > 2) {
		cerr << "The maximum supported number of players is two." << endl ;
		throw exception() ;
	}
	
	InputControl::registerForKeypress(onKeyJump) ;
	InputControl::registerForKeypress(onKeyMoveUp) ;
	InputControl::registerForKeypress(onKeyMoveDown) ;
	InputControl::registerForKeypress(onKeyMoveLeft) ;
	InputControl::registerForKeypress(onKeyMoveRight) ;
	InputControl::registerForKeypress(onKeyRotateCounterClockwise) ;
	InputControl::registerForKeypress(onKeyRotateClockwise) ;
	InputControl::registerForEvent(onKeyFire) ;
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
	
	TextOutput<float, int>::displayContinuousText(velocityTextDisplayUpdater, chrono::seconds(1), pos, 0.0, foreground, background) ;
}







