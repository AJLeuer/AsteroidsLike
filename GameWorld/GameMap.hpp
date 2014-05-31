//
//  GameMap.h
//  GameWorld
//
//  Created by Adam James Leuer on 4/7/14.
//  Copyright (c) 2014 Adam James Leuer. All rights reserved.
//

#ifndef __GameWorld__GameMap__
#define __GameWorld__GameMap__

#include <iostream>
#include <array>
#include <list>
#include <climits>

#include "../Util/Debug.h"
#include "../Util/Util.hpp"
#include "../Util/Position.hpp"
#include "../Util/Navigator.h"
#include "GameData.h"



template <class T>
class GameMap {
	
private:
	int mapMembers = 0 ;
	array< array< T *, GLOBAL_MAX_Y_+1>*, GLOBAL_MAX_X_+1> * intern_map ;
	
	void findAllNearby_helper(vector<T*> * store, Navigator & nav, const int x_lim, const int y_lim) ;
	
	GameMap<string> * gmDebug ; //debug

public:
	bool searchSuccess = false ;
	
	GameMap(int maxX, int maxY) ;
	
	//GameMap(T &) ; todo
	
	~GameMap() ;

	//GameMap<T> & operator=(const GameMap<T> & rhs) ; //todo
	
	array< array< T *, GLOBAL_MAX_Y_+1>*, GLOBAL_MAX_X_+1> * getMapVect() { return this->intern_map ; } ;
	
	unsigned long getXBound() { return intern_map->size() -1 ; } ;
	unsigned long getYBound() { return intern_map->at(0)->size() -1 ; } ;
	
	void place(Position<int> * where, T * mapObj, const BoundsCheck<int> check, bool allowMove) ;
	

	void placeAtNearestFree(Position<int> * where, T * mapObj, BoundsCheck<int> check) ;
	
	void erase(const Position<int> * currentLoc) ;
	
	void eraseAll() ;
	
	/**
	 * Moves the object to a new Position<int> on the map, and erases
	 * (calls erase()) on its old Position<int>
	 */
	void move(Position<int> & currentLoc, Position<int> & toNewLoc) ;
	
	/**
	 * Returns the first object at this Position<int>
	 */
	T * at(const Position<int> * where) ;
	
	Position<int> currentLoc(T* obj) ;
	
	/**
	 * Similar to erase(), but returns a pointer before erasing
	 */
	T * remove(Position<int> & currentLoc) ;
	
	/**
	 * Searches within the specified limits for an object of the specified type.
	 * Returns a nullptr if none found
	 * 
	 * @param start The Position<int> of the object that wants to search for nearby objects
	 * @param maxDistX The maximum distance to search longtitudinally
	 * @param maxDistY The maximum distance to search latitudinally
	 */
	vector<T*> * findNearby(const Position<int> * start, const int x_lim, const int y_lim) ;
	
} ;

template<class T>
GameMap<T>::GameMap(int maxX, int maxY) :
	intern_map(new array< array< T *, GLOBAL_MAX_Y_+1>*, GLOBAL_MAX_X_+1>()),
	gmDebug(nullptr)
{
	for (auto i = 0 ; i < maxX ; i++) {
		intern_map->at(i) = new array< T *, GLOBAL_MAX_Y_+1>() ;
		for (auto j = 0 ; j < maxY; j++) {
			intern_map->at(i)->at(j) = nullptr ;
		}
	}
}

template<class T>
GameMap<T>::~GameMap() {
	
	mapMembers = 0 ; 
	
	//delete all the vectors inside intern_map
	for (auto i = 0 ; i < intern_map->size() ; i++) {
		delete intern_map->at(i) ;
	}
	
	if (gmDebug != nullptr) {
		delete gmDebug ;
	}
}

/**
 * Places mapObj at Position where. If this Position is taken, places at the nearest Position that is free, and updates the
 * Position object pointed by where to reflect the new Position
 */
template<class T>
void GameMap<T>::place(Position<int> * where, T * mapObj, const BoundsCheck<int> check, bool allowMove) {
	if (mapObj == nullptr) {
		*(Debug::debugOutput) << "place() and placeAtNearestFree() cannot be used to place nullptrs. Use erase and eraseAll() \n" ;
		throw new exception() ;
	}
	
	if (at(where) == nullptr) {
		intern_map->at(where->getX())->at(where->getY()) = mapObj ;
		mapMembers++ ;
		return ;
	}
	else if (allowMove) {
		stringstream ss ;
		ss << "Warning: Call to GameMap::place() unsucessful. That Position was already taken." << '\n' ;
		ss << "Call rerouted to placeAtNearestFree(). Some objects may be in the wrong spot." << '\n' ;
		*(Debug::debugOutput) << ss.rdbuf() ;
		placeAtNearestFree(where, mapObj, check) ;
	}
	else {
		stringstream ss ;
		ss << "Warning: Call to GameMap::place() unsucessful. That Position was already taken." << '\n' ;
		ss << "The given position was not changed, nor was it moved on the GameMap" << '\n' ;
		*(Debug::debugOutput) << ss.rdbuf() ;
	}
	
}


template<class T>
void GameMap<T>::placeAtNearestFree(Position<int> * where, T * mapObj, const BoundsCheck<int> check) {
	if (mapObj == nullptr) {
		*(Debug::debugOutput) << "place() and placeAtNearestFree() cannot be used to place nullptrs. Use erase and eraseAll() \n" ;
		throw new exception() ;
	}
	if (at(where) == nullptr) {
		intern_map->at(where->getX())->at(where->getY()) = mapObj ;
		mapMembers++ ;
		return ;
	}
	else {
		
		FastRand<unsigned> rand(0, 3) ;
		unsigned swt = rand.nextValue() ;
		
		switch (swt) {
			case 0:
			{
				Position<int> * here = new Position<int>(where->getX(), where->getY() + 1, where->getZ(), check) ;
				return placeAtNearestFree(here, mapObj, defaultCheck<int>) ;
			}

			case 1:
			{
				Position<int> * here = new Position<int>(where->getX(), where->getY() - 1, where->getZ(), check) ;
				return placeAtNearestFree(here, mapObj, defaultCheck<int>) ;
			}
				
			case 2:
			{
				Position<int> * here = new Position<int>(where->getX() + 1, where->getY(), where->getZ(), check) ;
				return placeAtNearestFree(here, mapObj, defaultCheck<int>) ;
			}
				
			case 3:
			{
				Position<int> * here = new Position<int>(where->getX() - 1, where->getY(), where->getZ(), check);
				return placeAtNearestFree(here, mapObj, defaultCheck<int>) ;
			}
			default:
			{
				DebugOutput << "Problem with placeAtNearestFree() \n" ;
				throw new exception() ;
			}
		}
		//mapMembers++ ;
	}
	*Debug::debugOutput << "No empty positions found on map. placeAtNearestFree() threw exception \n" ;
	throw new exception() ;
}

template<class T>
void GameMap<T>::erase(const Position<int> * currentLoc) {
	if (at(currentLoc) != nullptr) {
		mapMembers-- ;
	}
	intern_map->at(currentLoc->getX())->at(currentLoc->getY()) = nullptr ;
}

template<class T>
void GameMap<T>::eraseAll() {
	for (auto i = 0 ; i < intern_map->size() ; i++) {
		for (auto j = 0 ; j < intern_map->at(i)->size() ; j++) {
			intern_map->at(i)->at(j) = nullptr ;
		}
	}
	mapMembers = 0 ;
}

template<class T>
void GameMap<T>::move(Position<int> & currentLoc, Position<int> & toNewLoc) {
	T * temp = at(currentLoc) ;
	erase(currentLoc) ;
	place(toNewLoc, temp) ;
}

template<class T>
T* GameMap<T>::at(const Position<int> * where) {
	return intern_map->at(where->getX())->at(where->getY()) ;
}

template<class T>
Position<int> GameMap<T>::currentLoc(T *obj) {
	for (auto i = 0 ; i < intern_map->size() ; i++) {
		for (auto j = 0 ; j < intern_map->at(i)->size() ; j++) {
			if ((intern_map->at(i)->at(j) != nullptr) && ((*(intern_map->at(i)->at(j)) == *obj))) {
				Position<int> l = (Position<int>(i, j, 0)) ;
				return l ;
			}
		}
	}
	/* debug code */
	DebugOutput << "GameMap::currentLoc() throwing exception. No object found at that Position. \n" ;
	throw exception() ;
	/* end debug code */
}

template<class T>
T* GameMap<T>::remove(Position<int> & currentLoc) {
	T * temp = at(currentLoc) ;
	if (temp != nullptr) {
		mapMembers-- ;
	}
	this->erase(currentLoc) ;
	return temp ;
}

template<class T>
vector<T*> * GameMap<T>::findNearby(const Position<int> * start, int x_lim, int y_lim) {
	
	searchSuccess = false ;
	vector<T*> * store = new vector<T*>() ;
	const Position<int> * strt = start ;
	Position<int> init = Position<int>(*start) ;
	Navigator nav(Direction::noDirection, strt, init) ;
	findAllNearby_helper(store, nav, x_lim, y_lim) ;
	return store ;
}

template<class T>
void GameMap<T>::findAllNearby_helper(vector<T*> * store, Navigator & nav, const int x_lim, const int y_lim) {
	
	//Debug::debugCounter++ ;
	
	if ((at(&(nav.current)) != nullptr) && (nav.current != *(nav.start))) {
		searchSuccess = true ;
		store->push_back(at(&(nav.current))) ;
	}

	switch (nav.dir) {
		case north : {
			if ((nav.y_travelled() <= y_lim) && (nav.current.getY() < getYBound())) {
				nav.current.y_plus_one() ;
				findAllNearby_helper(store, nav, x_lim, y_lim) ;
			}
			
		}
		break ;
			
				
		case south : {
			if ((nav.y_travelled() <= y_lim) && (nav.current.getY() > 0)) {
				nav.current.y_minus_one() ;
				findAllNearby_helper(store, nav, x_lim, y_lim) ;
			}
		}
		break ;
			
			
		case east : {
			if ((nav.x_travelled() <= x_lim) && (nav.current.getX() < getXBound())) {
				nav.current.x_plus_one() ;
				findAllNearby_helper(store, nav, x_lim, y_lim) ;
			}
		}
		break ;
			
				
		case west : {
			if ((nav.x_travelled() <= x_lim) && (nav.current.getX() > 0)) {
				nav.current.x_minus_one() ;
				findAllNearby_helper(store, nav, x_lim, y_lim) ;
			}
		}
		break ;
			
				
		case ne : {
			bool continue_e = ((nav.x_travelled() <= x_lim) && (nav.current.getX() < getXBound())) ;
			bool continue_n = ((nav.y_travelled() <= y_lim) && (nav.current.getY() < getYBound())) ;
			
			if (continue_e || continue_n)
			//if (((nav.x_travelled() < x_lim) && (nav.current.x < getXBound())) && //old code
			//((nav.y_travelled() < y_lim) && (nav.current.y < getYBound())))
			{
				if (continue_n) {
					Position<int> n_loc((nav.current)) ;
					n_loc.y_plus_one() ;
					Navigator n_nav = Navigator(Direction::north, nav.start, n_loc) ;
					findAllNearby_helper(store, n_nav, x_lim, y_lim) ;
				}
				
				if (continue_e) {
					Position<int> e_loc((nav.current)) ;
					e_loc.x_plus_one() ;
					Navigator e_nav = Navigator(Direction::east, nav.start, e_loc) ;
					findAllNearby_helper(store, e_nav, x_lim, y_lim) ;
				}
				
				if (continue_e && continue_n) {
					nav.current.x_plus_one() ;
					nav.current.y_plus_one() ;
					//nav.dir = ne ;
					findAllNearby_helper(store, nav, x_lim, y_lim) ;
				}
			}
		}
		break ;
			
				
		case se : {
			bool continue_e = ((nav.x_travelled() <= x_lim) && (nav.current.getX() < getXBound())) ;
			bool continue_s = ((nav.y_travelled() <= y_lim) && (nav.current.getY() > 0)) ;
			
			if (continue_e || continue_s)
				//(((nav.x_travelled() < x_lim) && (nav.current.x < getXBound())) &&
			   //((nav.y_travelled() < y_lim) && (nav.current.y > 0)))
			{
				if (continue_s) {
					Position<int> s_loc((nav.current)) ;
					s_loc.y_minus_one() ;
					Navigator s_nav = Navigator(Direction::south, nav.start, s_loc) ;
					findAllNearby_helper(store, s_nav, x_lim, y_lim) ;
				}
				
				if (continue_e) {
					Position<int> e_loc((nav.current)) ;
					e_loc.x_plus_one() ;
					Navigator e_nav = Navigator(Direction::east, nav.start, e_loc) ;
					findAllNearby_helper(store, e_nav, x_lim, y_lim) ;
				}
			
				if (continue_e && continue_s) {
					nav.current.x_plus_one() ;
					nav.current.y_minus_one() ;
					//nav.dir = ne ;
					findAllNearby_helper(store, nav, x_lim, y_lim) ;
				}
			}
		}
		break ;
			
				
		case sw : {
			bool continue_w = ((nav.x_travelled() <= x_lim) && (nav.current.getX() > 0)) ;
			bool continue_s = ((nav.y_travelled() <= y_lim) && (nav.current.getY() > 0)) ;
			
			if (continue_w || continue_s)
				//(((nav.x_travelled() < x_lim) && (nav.current.x > 0)) &&
				//((nav.y_travelled() < y_lim) && (nav.current.y > 0)))
			{
				if (continue_s) {
					Position<int> s_loc((nav.current)) ;
					s_loc.y_minus_one() ;
					Navigator s_nav = Navigator(Direction::south, nav.start, s_loc) ;
					findAllNearby_helper(store, s_nav, x_lim, y_lim) ;
				}
				
				if (continue_w) {
					Position<int> w_loc((nav.current)) ;
					w_loc.x_minus_one() ;
					Navigator w_nav = Navigator(Direction::west, nav.start, w_loc) ;
					findAllNearby_helper(store, w_nav, x_lim, y_lim) ;
				}
				
				if (continue_s && continue_w) {
					nav.current.x_minus_one() ;
					nav.current.y_minus_one() ;
					//nav.dir = sw ;
					findAllNearby_helper(store, nav, x_lim, y_lim) ;
				}
			}
		}
		break ;
			
				
		case nw : {
			bool continue_w = ((nav.x_travelled() <= x_lim) && (nav.current.getX() > 0)) ;
			bool continue_n = ((nav.y_travelled() <= y_lim) && (nav.current.getY() < getYBound())) ;
			
	
			if (continue_w || continue_w)
				//(((nav.x_travelled() < x_lim) && (nav.current.x > 0)) &&
			   //((nav.y_travelled() < y_lim) && (nav.current.y < getYBound())))
			{
				if (continue_n) {
					Position<int> n_loc((nav.current)) ;
					n_loc.y_plus_one() ;
					Navigator n_nav = Navigator(Direction::north, nav.start, n_loc) ;
					findAllNearby_helper(store, n_nav, x_lim, y_lim) ;
				}
				
				if (continue_w) {
					Position<int> w_loc((nav.current)) ;
					w_loc.x_minus_one() ;
					Navigator w_nav = Navigator(Direction::west, nav.start, w_loc) ;
					findAllNearby_helper(store, w_nav, x_lim, y_lim) ;
				}
				
				if (continue_w && continue_n) {
					nav.current.x_minus_one() ;
					nav.current.y_plus_one() ;
					findAllNearby_helper(store, nav, x_lim, y_lim) ;
				}
			}
		}
		break ;
			
		case noDirection : { //the base case
			if ((nav.current.getY() < getYBound())) {
				Position<int> n_loc = Position<int>(*nav.start) ;
				n_loc.y_plus_one() ;
				Navigator n_nav(north, nav.start, n_loc) ;
				findAllNearby_helper(store, n_nav, x_lim, y_lim) ;
			}
			
			if ((nav.current.getY() > 0)) {
				Position<int> s_loc = Position<int>(*nav.start) ;
				s_loc.y_minus_one() ;
				Navigator s_nav(south, nav.start, s_loc) ;
				findAllNearby_helper(store, s_nav, x_lim, y_lim) ;
			}
			
			if (nav.current.getX() < getXBound()) {
				Position<int> e_loc = Position<int>(*nav.start) ;
				e_loc.x_plus_one() ;
				Navigator e_nav(east, nav.start, e_loc) ;
				findAllNearby_helper(store, e_nav, x_lim, y_lim) ;
			}
			
			if ((nav.current.getX() > 0)) {
				Position<int> w_loc = Position<int>(*nav.start) ;
				w_loc.x_minus_one() ;
				Navigator w_nav(west, nav.start, w_loc) ;
				findAllNearby_helper(store, w_nav, x_lim, y_lim) ;
			}
			if ((nav.current.getX() < getXBound()) && ((nav.current.getY() < getYBound()))) {
				Position<int> ne_loc = Position<int>(*nav.start) ;
				ne_loc.x_plus_one() ;
				ne_loc.y_plus_one() ;
				Navigator ne_nav(ne, nav.start, ne_loc) ;
				findAllNearby_helper(store, ne_nav, x_lim, y_lim) ;
			}
			if ((nav.current.getX() < getXBound()) && ((nav.current.getY() > 0))) {
				Position<int> se_loc = Position<int>(*nav.start) ;
				se_loc.x_plus_one() ;
				se_loc.y_minus_one() ;
				Navigator se_nav(se, nav.start, se_loc) ;
				findAllNearby_helper(store, se_nav, x_lim, y_lim) ;
			}
			if ((nav.current.getX() > 0) && ((nav.current.getY() > 0))) {
				Position<int> sw_loc = Position<int>(*nav.start) ;
				sw_loc.x_minus_one() ;
				sw_loc.y_minus_one() ;
				Navigator sw_nav(sw, nav.start, sw_loc) ;
				findAllNearby_helper(store, sw_nav, x_lim, y_lim) ;
			}
			if ((nav.current.getX() > 0) && ((nav.current.getY() < getYBound()))) {
				Position<int> nw_loc = Position<int>(*nav.start) ;
				nw_loc.x_minus_one() ;
				nw_loc.y_plus_one() ;
				Navigator nw_nav(nw, nav.start, nw_loc) ;
				findAllNearby_helper(store, nw_nav, x_lim, y_lim) ;
			}
			
		}
			
	}

}



#endif /* defined(__GameWorld__GameMap__) */
