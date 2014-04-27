//
//  OutputAdapter.h
//  GameWorld
//
//  Created by Adam James Leuer on 3/18/14.
//  Copyright (c) 2014 Adam James Leuer. All rights reserved.
//

#ifndef __GameWorld__OutputAdapter__
#define __GameWorld__OutputAdapter__

#define eight_milliseconds 8333 //expressed in microseconds

#include <ncurses.h>
#include <unistd.h>

#include <iostream>
#include <thread>

#include "AdapterUtil.h"
#include "ConsoleOutput.hpp"
#include "WindowOutput.h"

#include "../Util/Util.hpp"
#include "../Util/Position.hpp"
#include "../GameWorld/GameData.h"


using namespace std ;


/**
 * Much like WorldController, OutputAdapter is a singleton, unlike WorldController though its class variables and functions run in an instance
 * For the client however, the only difference in calling WorldController's functions and calling OutputAdapter's is that they will 
 * need to create a single object before calling OutputAdapter's methods. However the naming conventions and usage should be
 * very similar to that found in the WorldController class. It will have access to WorldController's data
 * members, but while WorldController is in charge of manipulating objects in the GameWorld and mediating their interactions,
 * OutputAdapter only uses WorldController's data to update the view
 */
template<class T>
class OutputAdapter : public AdapterInterface<T> {


private:


	
public:
	
	/**
	 * This class should be instantiated only once.
	 */
	OutputAdapter() : AdapterInterface<T>() {}
	
	/**
	 * Move constructor. Will change AdapterInterface<T>'s pointer to new object
	 */
	OutputAdapter(OutputAdapter && other) : AdapterInterface<T>(move(other)) {}
		
	/**
	 * Assignment operator overload (move)
	 */
	OutputAdapter & operator=(OutputAdapter && rhs){ this->AdapterInterface<T>::operator=(move(rhs)) ; }
	
	~OutputAdapter() {} //should automatically call ~AdapterInterface()
	
	void init(const vector<T*> * container_) ;
	
	void show() ;
	
	
	/**
	 * See show()
	 */
	void operator()() ;
	
	void exit() ;
	
};


template<class T>
void OutputAdapter<T>::init(const vector<T *> *container_) {
	ConsoleOutput::init() ;
	this->AdapterInterface<T>::container = container_ ;
}

template<class T>
void OutputAdapter<T>::show() {
	while (GLOBAL_CONTINUE_SIGNAL) {
		stringstream ss ;
		Locking::sharedMutex.lock() ;
		for (auto i = 0 ; i < this->AdapterInterface<T>::container->size() ; i++) {
			auto temp = AdapterInterface<T>::container->at(i) ;
			ss << "Current GameObject: " << endl << temp << endl ;
			ConsoleOutput::setOutput(*temp->getPosition(), temp->getIcon().c_str()) ;
			temp = nullptr ;
		}
		Locking::sharedMutex.unlock() ;
		*(Debug::debugOutput) << ss.rdbuf() ;
		ConsoleOutput::update(eight_milliseconds) ; //waits for 8 ms (i.e. 120th of a second) before clearing
	}
	ConsoleOutput::exit() ;
}

template<class T>
void OutputAdapter<T>::operator()() {
	this->show() ;
}

template<class T>
void OutputAdapter<T>::exit() {
	ConsoleOutput::exit() ;
	AdapterInterface<T>::exit() ;
}

#endif /* defined(__GameWorld__OutputAdapter__) */
