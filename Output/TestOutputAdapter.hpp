//
//  TestOutputAdapter.h
//  GameWorld
//
//  Created by Adam James Leuer on 3/19/14.
//  Copyright (c) 2014 Adam James Leuer. All rights reserved.
//

#ifndef __GameWorld__TestOutputAdapter__
#define __GameWorld__TestOutputAdapter__

#define eight_milliseconds 8333 //expressed in microseconds

#include <ncurses.h>
#include <unistd.h>

#include <iostream>
#include <ostream>
#include <thread>

#include "AdapterUtil.h"
#include "AdapterInterface.hpp"

#include "../Util/Util.hpp"
#include "../Util/Position.hpp"
#include "../GameWorld/GameData.h"



using namespace std ;

/**
 * A stand-in for Adapter, mainly used for debug purposes
 *
 *
 *
 *
 *
 */
template<class T>
class TestOutputAdapter : public AdapterInterface<T> {
	

private:

	
	 
public:
	
	/**
	 * This class should be instantiated only once, and not with this constructor. Call TestOutputAdapter(int).
	 * Trying to create more than one instance of TestOutputAdapter or any other
	 * class inheriting from AdapterInterface will result in an exception being thrown.
	 */
	TestOutputAdapter() : AdapterInterface<T>() {}
	
	/**
	 * Copy constructor.
	 */
	TestOutputAdapter(const TestOutputAdapter & other) : AdapterInterface<T>(other) {}
	
	/**
	 * Move constructor. Will change AdapterInterface<T>'s pointer to new object
	 */
	TestOutputAdapter(TestOutputAdapter && other) : AdapterInterface<T>(other) {}
	
	/**
	 * Assignment operator overload (copy)
	 */
	TestOutputAdapter & operator=(const TestOutputAdapter & rhs){ this->AdapterInterface<T>::operator=(rhs) ; }
	
	/**
	 * Assignment operator overload (move)
	 */
	TestOutputAdapter & operator=(TestOutputAdapter && rhs){ this->AdapterInterface<T>::operator=(rhs) ; }
	
	~TestOutputAdapter() {} //should automatically call ~AdapterInterface<T>()
	
	void init(const vector<T*> * container_) ;
	
	void show() ;
	
	
	/**
	 * See show()
	 */
	void operator()() ;
	
	void exit() ;
	
} ;


template<class T>
void TestOutputAdapter<T>::init(const vector<T*> * container_) {
	
	this->AdapterInterface<T>::container = container_ ;
}

template<class T>
void TestOutputAdapter<T>::show() {
	while (*GLOBAL_CONTINUE_SIGNAL) {
		stringstream ss ;
		Locking::sharedMutex.lock() ;
		for (auto i = 0 ; i < TestOutputAdapter<T>::container->size() ; i++) {
			auto temp = TestOutputAdapter<T>::container->at(i) ;
			ss << "Current GameObject: " ;
			ss << *temp ;
			ss << temp->getIcon().c_str() << endl << endl ;
			temp = nullptr ;
		}
		Locking::sharedMutex.unlock() ;
		*(Debug::debugOutput) << ss.rdbuf() ;
	}
	/*   */
}

template<class T>
void TestOutputAdapter<T>::operator()() {
	this->show() ;
}

template<class T>
void TestOutputAdapter<T>::exit() {
	this->AdapterInterface<T>::exit() ;
	//local cleanup:
}

#endif /* defined(__GameWorld__TestOutputAdapter__) */
