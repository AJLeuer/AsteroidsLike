//
//  AdapterInterface.h
//  GameWorld
//
//  Created by Adam James Leuer on 3/19/14.
//  Copyright (c) 2014 Adam James Leuer. All rights reserved.
//

#ifndef __GameWorld__AdapterInterface__
#define __GameWorld__AdapterInterface__

#include <ncurses.h>
#include <unistd.h>

#include <iostream>
#include <thread>
#include <vector>

#include "AdapterUtil.h"
#include "ConsoleOutput.hpp"

#include "../Util/Debug.h"
#include "../Util/Position.hpp"
#include "../GameWorld/GameData.h"

using namespace std ;

/**
 * An abstract class that mainly serves as the base class for Adapter, but will also allow us to create
 * other Adapter-like classes to handle output
 *
 */
template<class T>
class AdapterInterface {
	
protected:
	std::ostream * outputDebug ;
	const vector<T*> * container ;
	bool * currentlyThreaded ;
	
	virtual void show_threaded() ;
	
public:
	
	AdapterInterface() ;
	
	/**
	 * Move constructor
	 */
	AdapterInterface(AdapterInterface && other) ;
				
	virtual ~AdapterInterface() ;
	
	/**
	 * Assignment operator overload (move)
	 */
	AdapterInterface & operator=(AdapterInterface &&) ;
	
	virtual void init(const vector<T*> * container_ ) ;
	
	virtual void show() ;
	
	
	void drawRepresentation(array< array<const T *, GLOBAL_MAX_Y_> *, GLOBAL_MAX_X_> * map, ostream * out) ;
	
	
	/**
	 * See show()
	 */
	virtual void operator()() ;
	
	virtual void exit() ;
	
	
};

template<class T>
AdapterInterface<T>::AdapterInterface() :
	outputDebug(nullptr/*new ofstream("Output Debug.txt")*/),
	container(nullptr),
	currentlyThreaded(new bool{false})
	/* container initialized via init() */ {}


template<class T>
AdapterInterface<T>::AdapterInterface(AdapterInterface<T> && other) :
	outputDebug(other.outputDebug),
	container(other.container),
	currentlyThreaded(new bool(true))
{
	other.aiThread = nullptr ;
	other.outputDebug = nullptr ;
	other.container = nullptr ;
}


template<class T>
AdapterInterface<T>::~AdapterInterface() {
	if (!(*currentlyThreaded)) {
		container = nullptr ;
		delete currentlyThreaded ;
		delete outputDebug ;
	}
}

template<class T>
AdapterInterface<T> & AdapterInterface<T>::operator=(AdapterInterface && rhs) {
	if (this != &rhs) {
		if (this != &rhs) {
			this->aiThread = rhs.aiThread ;
			container = rhs.container ;
			currentlyThreaded = rhs.currentlyThreaded ;
			outputDebug = rhs.outputDebug ;
		}
		if (!currentlyThreaded) {
			rhs.aiThread = nullptr ;
			rhs.container = nullptr ;
			rhs.currentlyThreaded = nullptr ;
			rhs.outputDebug = nullptr ;
		}
	}
	return *this ;
}

template<class T>
void AdapterInterface<T>::init(const vector<T *> *container_) {}

template<class T>
void AdapterInterface<T>::show() {}
 

template<class T>
void AdapterInterface<T>::show_threaded() {}

template<class T>
void AdapterInterface<T>::drawRepresentation(array< array<const T *, GLOBAL_MAX_Y_> *, GLOBAL_MAX_X_> * map, ostream * out) {
	Drawing drawing ;
	stringstream ss ;
	drawing.draw2DRepresentation(ss, map, ' ') ;
	*outputDebug << ss.rdbuf() << endl ;
}

template<class T>
void AdapterInterface<T>::operator()() {
	this->show() ;
}

template<class T>
void AdapterInterface<T>::exit() {
	*currentlyThreaded = false ;
}



#endif /* defined(__GameWorld__AdapterInterface__) */
