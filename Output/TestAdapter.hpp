//
//  TestAdapter.h
//  GameWorld
//
//  Created by Adam James Leuer on 3/19/14.
//  Copyright (c) 2014 Adam James Leuer. All rights reserved.
//

#ifndef __GameWorld__TestAdapter__
#define __GameWorld__TestAdapter__

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
class TestAdapter : public AdapterInterface<T> {
	

private:

	void show_threaded() ;
	 
public:
	
	/**
	 * This class should be instantiated only once, and not with this constructor. Call TestAdapter(int).
	 * Trying to create more than one instance of TestAdapter or any other
	 * class inheriting from AdapterInterface will result in an exception being thrown.
	 */
	TestAdapter() : AdapterInterface<T>() {}
	
	/**
	 * Move constructor. Will change AdapterInterface<T>'s pointer to new object
	 */
	TestAdapter(TestAdapter && other) : AdapterInterface<T>(other) {}
	
	/**
	 * Assignment operator overload (move)
	 */
	TestAdapter & operator=(TestAdapter && rhs){ this->AdapterInterface<T>::operator=(rhs) ; }
	
	~TestAdapter() {} //should automatically call ~AdapterInterface<T>()
	
	void init(const vector<T*> * container_) ;
	
	void show() ;
	
	
	/**
	 * See show()
	 */
	void operator()() ;
	
	void close() ;
	
} ;


template<class T>
void TestAdapter<T>::init(const vector<T*> * container_) {
	
	this->AdapterInterface<T>::container = container_ ;
}

template<class T>
void TestAdapter<T>::show() {
	this->AdapterInterface<T>::aiThread = new std::thread(&TestAdapter<T>::show_threaded, std::move(this)) ;
}

template<class T>
void TestAdapter<T>::show_threaded() {
	while (*GLOBAL_CONTINUE_SIGNAL) {
		stringstream ss ;
		Locking::sharedMutex.lock() ;
		for (auto i = 0 ; i < TestAdapter<T>::container->size() ; i++) {
			auto temp = TestAdapter<T>::container->at(i) ;
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
void TestAdapter<T>::operator()() {
	this->show() ;
}

template<class T>
void TestAdapter<T>::close() {
	this->AdapterInterface<T>::close() ;
	
	//local cleanup:
}

#endif /* defined(__GameWorld__TestAdapter__) */
