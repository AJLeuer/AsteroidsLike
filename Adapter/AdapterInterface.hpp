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

#include "../Util/Debug.h"
#include "../Util/Location.hpp"
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
	std::thread * aiThread ;
	std::ostream * file ;
	const vector<T*> * container ;
	bool * currentlyThreading ;
	
	virtual void show_threaded(bool * contin) ;
	
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
	
	virtual void show(bool * contin) ;
	
	void drawRepresentation(vector< vector<const T *> *> *, ostream * out = Debug::debugOutput) ;
	
	
	
	/**
	 * See show()
	 */
	virtual void operator()(bool * contin) ;
	
	virtual void close() ;
	
	
};

template<class T>
AdapterInterface<T>::AdapterInterface() :
	aiThread(nullptr),
	file(new ofstream("AdapterOutput.txt")),
	container(nullptr),
	currentlyThreading(new bool{false})
	/* container initialized via init() */ {}



template<class T>
AdapterInterface<T>::AdapterInterface(AdapterInterface<T> && other) :
	aiThread(other.aiThread),
	file(other.file),
	container(other.container)
{
	//other.aidThread = nullptr ;
	other.file = nullptr ;
	other.container = nullptr ;
}


template<class T>
AdapterInterface<T>::~AdapterInterface() {
	if (!(*currentlyThreading)) {
		delete aiThread ;
		container = nullptr ;
		delete currentlyThreading ;
		delete file ;
	}
}

template<class T>
AdapterInterface<T> & AdapterInterface<T>::operator=(AdapterInterface && rhs) {
	if (this != &rhs) {
		if (this != &rhs) {
			this->aiThread = rhs.aiThread ;
			container = rhs.container ;
			currentlyThreading = rhs.currentlyThreading ;
			file = rhs.file ;
		}
		if (!currentlyThreading) {
			rhs.aiThread = nullptr ;
			rhs.container = nullptr ;
			rhs.currentlyThreading = nullptr ;
			rhs.file = nullptr ;
		}
	}
	return *this ;
}

template<class T>
void AdapterInterface<T>::init(const vector<T *> *container_) {}

template<class T>
void AdapterInterface<T>::show(bool * contin) {}
 

template<class T>
void AdapterInterface<T>::show_threaded(bool * contin) {}

template<class T>
void AdapterInterface<T>::drawRepresentation(vector< vector<const T *> *> * map, ostream * out) {
	Drawing drawing ;
	stringstream ss ;
	drawing.draw2DRepresentation(ss, map, ' ') ;
	*file << ss.rdbuf() << endl ;
}

template<class T>
void AdapterInterface<T>::operator()(bool * contin) {
	this->show(contin) ;
}

template<class T>
void AdapterInterface<T>::close() {
	this->aiThread->join() ;
}



#endif /* defined(__GameWorld__AdapterInterface__) */
