//
//  Adapter.h
//  GameWorld
//
//  Created by Adam James Leuer on 3/18/14.
//  Copyright (c) 2014 Adam James Leuer. All rights reserved.
//

#ifndef __GameWorld__Adapter__
#define __GameWorld__Adapter__

#include <ncurses.h>
#include <unistd.h>

#include <iostream>
#include <thread>

#include "AdapterUtil.h"
#include "AdapterInterface.hpp"

#include "../Util/Util.hpp"
#include "../Util/Location.hpp"
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
	std::thread * aiThread ;
	std::ostream * file ;
	const vector<T*> * container ;
	
	void show_threaded(bool * contin) ;

	
public:
	
	/**
	 * This class should be instantiated only once, and not with this constructor. Call Adapter(int).
	 * Trying to create more than one instance of Adapter or any other
	 * class inheriting from AdapterInterface will result in an exception being thrown.
	 */
	Adapter() : AdapterInterface<T>() {} ;

	/**
	 * Move constructor. Will change AdapterInterface<T>'s pointer to new object
	 */
	Adapter(Adapter && other) : AdapterInterface<T>(other) {}
	
	/**
	 * Assignment operator overload (move)
	 */
	Adapter & operator=(Adapter && rhs){ this->AdapterInterface<T>::operator=(rhs) ; }
	
	~Adapter() { } //should automatically call ~AdapterInterface()
	
	void init(const vector<T*> * container_) ;
	
	void show(bool * contin) ;
	
	void drawRepresentation(vector< vector<const T *> *> *, ostream * = Debug::debugOutput) ;
	
	
	/**
	 * See show()
	 */
	void operator()(bool * contin) ;
	
	void close() ;
	
};


template<class T>
void Adapter<T>::init(const vector<T *> *container_) {
	this->AdapterInterface<T>::init(container_) ;
	
	//local initializations:
	setlocale(LC_ALL, ""); //allows printing more types of characters (?)
	
	initscr();  //this inits and returns a reference to a WINDOW*, which we don't need
	
	cbreak() ; //character-at-a-time  input  without buffering
	noecho() ; //wont print input back out
	
	curs_set(0) ; //sets cursor to invisible
}


template<class T>
void Adapter<T>::show(bool * contin) {
	aiThread = new std::thread((&Adapter<T>::show_threaded), std::move(this), contin) ;
}

template<class T>
void Adapter<T>::show_threaded(bool * contin) {
	while (*contin) {
		stringstream ss ;
		Locking::sharedMutex.lock() ;
		for (auto i = 0 ; i < container->size() ; i++) {
			auto temp = container->at(i) ;
			ss << "Current GameObject: " << endl << temp << endl ;
			Location<long> trans = AdapterUtil::transLocation(*(temp->getLocation())) ;
			mvwaddstr(stdscr, trans.getY(), trans.getX(), temp->getIcon().c_str()) ;
			wnoutrefresh(stdscr) ;
			temp = nullptr ;
		}
		Locking::sharedMutex.unlock() ;
		*(Debug::debugOutput) << ss.rdbuf() ;
		doupdate() ;
		usleep(90000) ;
		wclear(stdscr) ;
	}
}

template<class T>
void Adapter<T>::drawRepresentation(vector< vector<const T *> *> * map, ostream * out) {
	this->AdapterInterface<T>::drawRepresentation(map, out) ;
}

template<class T>
void Adapter<T>::operator()(bool * contin) {
	this->show(contin) ;
}

template<class T>
void Adapter<T>::close() {
	this->AdapterInterface<T>::close() ;
	
	//local cleanup:
	endwin() ;
}

#endif /* defined(__GameWorld__Adapter__) */
