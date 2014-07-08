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
Pos2<float> Player::defaultStartingPosition = Pos2<float>((globalMaxX() / 2) + FastRand<int>::defaultRandom(-20, 20), (globalMaxY() - (globalMaxY() * 0.25)), 0) ;
AssetType Player::defaultPCAssetType = AssetType::playerShip ; /* change if needed */
float Player::defaultSize = 1.00 ;

Player * Player::defaultPlayer0 = nullptr ;
Player * Player::defaultPlayer1 = nullptr ;

void Player::initDefaultPlayers() {
	defaultPlayer0 = new Player("Player 0", "Ship1_Green.png", Colors::green, defaultSize, defaultStartingPosition, "Green",
									Reaction::friendly, DoA::nodoa, CharacterState::idle, 500, 100) ;
	
	defaultPlayer1 = new Player("Player 1", "Ship0_Red.png", Colors::blue, defaultSize, defaultStartingPosition, "Blue",
									Reaction::friendly, DoA::nodoa, CharacterState::idle, 500, 100) ;
}

Player::Player() :
	ID(IDs),
	name(""),
	playerCharacter(Colors::blue, defaultPCAssetType, AssetFileIO::getRandomImageFile(defaultPCAssetType).fileName, defaultSize,
					defaultStartingPosition,"", Reaction::friendly, DoA::nodoa, CharacterState::idle, 500, 100)
{
	IDs++ ;
	setNames() ;
	registerForCallbacks() ;
}

Player::Player(const string & name, const string & playerCharacter_imageFilename,
	Colors playerCharacter_color, float playerCharacter_size, const Pos2<float> & playerCharacter_loc, const string & playerCharacter_name,
	Reaction playerCharacter_reaction, DoA playerCharacter_alive, CharacterState playerCharacter_state,
	unsigned playerCharacter_health, unsigned playerCharacter_damage) :

	ID(IDs),
	name(name),
	playerCharacter(playerCharacter_color, defaultPCAssetType, playerCharacter_imageFilename, playerCharacter_size,
		playerCharacter_loc, playerCharacter_name, playerCharacter_reaction, playerCharacter_alive, playerCharacter_state,
		playerCharacter_health, playerCharacter_damage)
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
	if (playerCharacter.getColor() != Colors::green) {
		KeyInputRegister * onKeyMoveUp = new KeyInputRegister(&playerCharacter, (&GameInterface::moveUp),
															{MOVE_UP}, {SDLK_UP}, KeypressEvaluationMethod::any) ;
		KeyInputRegister * onKeyMoveDown = new KeyInputRegister(&playerCharacter, (&GameInterface::moveDown),
															{MOVE_DOWN}, {SDLK_DOWN}, KeypressEvaluationMethod::any) ;
		KeyInputRegister * onKeyMoveLeft = new KeyInputRegister(&playerCharacter, (&GameInterface::moveLeft),
															  {MOVE_LEFT}, {SDLK_LEFT}, KeypressEvaluationMethod::any) ;
		KeyInputRegister * onKeyMoveRight = new KeyInputRegister(&playerCharacter, (&GameInterface::moveRight),
															   {MOVE_RIGHT}, {SDLK_RIGHT}, KeypressEvaluationMethod::any) ;
		InputController::registerForKeypress(onKeyMoveUp) ;
		InputController::registerForKeypress(onKeyMoveDown) ;
		InputController::registerForKeypress(onKeyMoveLeft) ;
		InputController::registerForKeypress(onKeyMoveRight) ;
	}
	else if (playerCharacter.getColor() != Colors::blue) {
		
		KeyInputRegister * onKeyMoveUp = new KeyInputRegister(&playerCharacter, (&GameInterface::moveUp),
															{SDLK_KP_8}, KeypressEvaluationMethod::exactlyOne) ;
		KeyInputRegister * onKeyMoveDown = new KeyInputRegister(&playerCharacter, (&GameInterface::moveDown),
															  {SDLK_KP_2}, KeypressEvaluationMethod::exactlyOne) ;
		KeyInputRegister * onKeyMoveLeft = new KeyInputRegister(&playerCharacter, (&GameInterface::moveLeft),
															  {SDLK_KP_4}, KeypressEvaluationMethod::exactlyOne) ;
		KeyInputRegister * onKeyMoveRight = new KeyInputRegister(&playerCharacter, (&GameInterface::moveRight),
															   {SDLK_KP_6}, KeypressEvaluationMethod::exactlyOne) ;
		
		InputController::registerForKeypress(onKeyMoveUp) ;
		InputController::registerForKeypress(onKeyMoveDown) ;
		InputController::registerForKeypress(onKeyMoveLeft) ;
		InputController::registerForKeypress(onKeyMoveRight) ;
	}
}

void Player::displayVelocity(Position<float> pos, GameColor foreground, GameColor background) {
	
	auto velocityDisplay = [&, this] () -> void {
		
		stringstream stream ;
		stream << "Player " << this->ID << "'s " << setprecision(2) << std::fixed << *playerCharacter.getVector()->getVelocity() ;
	
		string str(stream.str()) ;
		
		//Size<int> size(TextOutput::getSizeOfText(str)) ;
		
		TextOutput out(str, pos, foreground, background) ;
		
		while (GLOBAL_CONTINUE_SIGNAL) {
			/* out will keep updating as long as it exists */
			stream = stringstream() ; //reset the sstream
			stream << "Player " << this->ID << "'s " << setprecision(2) << std::fixed << *playerCharacter.getVector()->getVelocity() ;
			str = stream.str() ;
			out.updateText(str) ;
			this_thread::sleep_for(chrono::milliseconds(32)) ;
		}
		
	} ;
	
	thread thr(velocityDisplay) ;
	thr.detach() ;
}







