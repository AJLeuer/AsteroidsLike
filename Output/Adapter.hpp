//
//  Adapter.h
//  GameWorld
//
//  Created by Adam James Leuer on 3/18/14.
//  Copyright (c) 2014 Adam James Leuer. All rights reserved.
//

#ifndef __GameWorld__Adapter__
#define __GameWorld__Adapter__

#define eight_milliseconds 8333 //expressed in microseconds

#include <ncurses.h>
#include <unistd.h>

#include <iostream>
#include <thread>

#include "AdapterUtil.h"
#include "AdapterInterface.hpp"
#include "ConsoleOutput.hpp"

#include "../Util/Util.hpp"
#include "../Util/Position.hpp"
#include "../GameWorld/GameData.h"


using namespace std ;


/**
 * Much like WorldController, Adapter is a singleton, unlike WorldController though its class variables and functions run in an instance
 * For the client however, the only difference in calling WorldController's functions and calling Adapter's is that they will 
 * need to create a single object before calling Adapter's methods. However the naming conventions and usage should be
 * very similar to that found in the WorldController class. It will have access to WorldController's data
 * members, but while WorldController is in charge of manipulating objects in the GameWorld and mediating their interactions,
 * Adapter only uses WorldController's data to update the view
 */
template<class T>
class Adapter : public AdapterInterface<T> {


private:

	void show_threaded() ;
	
public:
	
	/**
	 * This class should be instantiated only once, and not with this constructor. Call Adapter(int).
	 * Trying to create more than one instance of Adapter or any other
	 * class inheriting from AdapterInterface will result in an exception being thrown.
	 */
	Adapter() : AdapterInterface<T>() { Debug::init(false) ; }

	/**
	 * Move constructor. Will change AdapterInterface<T>'s pointer to new object
	 */
	Adapter(Adapter && other) : AdapterInterface<T>(other) {}
	
	/**
	 * Assignment operator overload (move)
	 */
	Adapter & operator=(Adapter && rhs){ this->AdapterInterface<T>::operator=(rhs) ; }
	
	~Adapter() {} //should automatically call ~AdapterInterface()
	
	void init(const vector<T*> * container_) ;
	
	void show() ;
	
	
	/**
	 * See show()
	 */
	void operator()() ;
	
	void close() ;
	
};


template<class T>
void Adapter<T>::init(const vector<T *> *container_) {
	ConsoleOutput::init() ;
	this->AdapterInterface<T>::container = container_ ;
}

template<class T>
void Adapter<T>::show() {
	this->AdapterInterface<T>::aiThread = new std::thread((&Adapter<T>::show_threaded), std::move(this)) ;
}

template<class T>
void Adapter<T>::show_threaded() {
	while (*GLOBAL_CONTINUE_SIGNAL) {
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
	ConsoleOutput::close() ;
}

template<class T>
void Adapter<T>::operator()() {
	this->show() ;
}

template<class T>
void Adapter<T>::close() {
	ConsoleOutput::close() ;
	AdapterInterface<T>::close() ;
}

#endif /* defined(__GameWorld__Adapter__) */
