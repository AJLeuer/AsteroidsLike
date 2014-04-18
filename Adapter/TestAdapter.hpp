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
#include "../Util/Location.hpp"
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
	std::thread * aiThread ;
	std::ostream * file = new ofstream("AdapterOutput.txt") ;
	const vector<T*> * container ;
	
	void show_threaded(bool * contin) ;

	
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
	
	void show(bool * contin) ;
	
	void drawRepresentation(vector< vector<const T *> *> *, ostream * = Debug::debugOutput) ;
	
	/**
	 * See show()
	 */
	void operator()(bool * contin) ;
	
	void close() ;
	
} ;


template<class T>
void TestAdapter<T>::init(const vector<T *> *container_) {
	this->AdapterInterface<T>::init(container_) ;
	
	//local initializations:
	//
	///
	//
	///
	//
	//
	//
	//
	//
	//
	//
	//
	
}



template<class T>
void TestAdapter<T>::show(bool * contin) {
	aiThread = new std::thread(&TestAdapter<T>::show_threaded, this, contin) ;
}

template<class T>
void TestAdapter<T>::show_threaded(bool * contin) {
	while (*contin) {
		stringstream ss ;
		Locking::sharedMutex.lock() ;
		for (auto i = 0 ; i < container->size() ; i++) {
			auto temp = container->at(i) ;
			ss << "Current GameObject: " ;
			ss << *temp ;
			ss << temp->getIcon().c_str() << endl << endl ;
			temp = nullptr ;
		}
		Locking::sharedMutex.unlock() ;
		*(Debug::debugOutput) << ss.rdbuf() ;
		//
		//
		//
	}
}

template<class T>
void TestAdapter<T>::drawRepresentation(vector< vector<const T *> *> * map, ostream * out) {
	this->AdapterInterface<T>::drawRepresentation(map, out) ;
}

template<class T>
void TestAdapter<T>::operator()(bool * contin) {
	this->show(contin) ;
}

template<class T>
void TestAdapter<T>::close() {
	this->AdapterInterface<T>::close() ;
	
	//local cleanup:
}

#endif /* defined(__GameWorld__TestAdapter__) */
