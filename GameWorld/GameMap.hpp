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
	
	template<typename N>
	void findAllNearby_helper(vector<T*> * store, Navigator & nav, const N x_lim, const N y_lim) ;
	
	GameMap<string> * gmDebug ; //debug

public:
	bool searchSuccess = false ;
	
	template<typename N>
	GameMap(N maxX, N maxY) ;
	
	//GameMap(T &) ; todo
	
	~GameMap() ;

	//GameMap<T> & operator=(const GameMap<T> & rhs) ; //todo
	
	array< array< T *, GLOBAL_MAX_Y_+1>*, GLOBAL_MAX_X_+1> * getMapVect() { return this->intern_map ; } ;
	
	unsigned long getXBound() { return intern_map->size() -1 ; } ;
	unsigned long getYBound() { return intern_map->at(0)->size() -1 ; } ;
	
	template<typename N>
	void place(Position<N> * where, T * mapObj, const BoundsCheck<N> check, bool allowMove) ;
	
	template<typename N>
	void placeAtNearestFree(Position<N> * where, T * mapObj, BoundsCheck<N> check) ;
	
	template<typename N>
	void erase (const Position<N> & currentLoc) ;
	
	void eraseAll() ;
	
	/**
	 * Moves the object to a new Position<N> on the map, and erases
	 * (calls erase()) on its old Position<N>
	 */
	template<typename N>
	void move(Position<N> & currentLoc, Position<N> & toNewLoc) ;
	
	/**
	 * Returns the first object at this Position<N>
	 */
	template<typename N>
	T * at(const Position<N> & where) ;
	
	template<typename N>
	Position<N> currentLoc(T* obj) ;
	
	/**
	 * Similar to erase(), but returns a pointer before erasing
	 */
	template<typename N>
	T * remove(Position<N> & currentLoc) ;
	
	/**
	 * Searches within the specified limits for an object of the specified type.
	 * Returns a nullptr if none found
	 * 
	 * @param start The Position<N> of the object that wants to search for nearby objects
	 * @param maxDistX The maximum distance to search longtitudinally
	 * @param maxDistY The maximum distance to search latitudinally
	 */
	template<typename N>
	vector<T*> * findNearby(const Position<N> * start, const N x_lim, const N y_lim) ;
	
} ;

template<class T>
template<typename N>
GameMap<T>::GameMap(N maxX, N maxY) :
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
template<typename N>
void GameMap<T>::place(Position<N> * where, T * mapObj, const BoundsCheck<N> check, bool allowMove) {
	if (mapObj == nullptr) {
		*(Debug::debugOutput) << "place() and placeAtNearestFree() cannot be used to place nullptrs. Use erase and eraseAll() \n" ;
		throw new exception() ;
	}
	
	if (at(*where) == nullptr) {
		intern_map->at(where->getX())->at(where->getY()) = mapObj ;
		mapMembers++ ;
		return ;
	}
	else if (allowMove) {
		stringstream ss ;
		ss << "Warning: Call to GameMap::place() unsucessful. That Position was already taken." << endl ;
		ss << "Call rerouted to placeAtNearestFree(). Some objects may be in the wrong spot." << endl ;
		*(Debug::debugOutput) << ss.rdbuf() ;
		placeAtNearestFree(where, mapObj, check) ;
	}
	else {
		stringstream ss ;
		ss << "Warning: Call to GameMap::place() unsucessful. That Position was already taken." << endl ;
		ss << "The given position was not changed, nor was it moved on the GameMap" << endl ;
		*(Debug::debugOutput) << ss.rdbuf() ;
	}
	
}


template<class T>
template<typename N>
void GameMap<T>::placeAtNearestFree(Position<N> * where, T * mapObj, const BoundsCheck<N> check) {
	if (mapObj == nullptr) {
		*(Debug::debugOutput) << "place() and placeAtNearestFree() cannot be used to place nullptrs. Use erase and eraseAll() \n" ;
		throw new exception() ;
	}
	if (at(*where) == nullptr) {
		intern_map->at(where->getX())->at(where->getY()) = mapObj ;
		mapMembers++ ;
		return ;
	}
	else {
		
		fastRand<unsigned> rand(0, 3) ;
		unsigned swt = rand.nextValue() ;
		
		switch (swt) {
			case 0:
			{
				Position<N> * temp = new Position<N>(where->x, where->y + 1, where->z, check) ;
				//delete where ;
				*where = std::move(*temp) ;
				return placeAtNearestFree(where, mapObj, defaultCheck) ;
			}

			case 1:
			{
				Position<N> * temp = new Position<N>(where->x, where->y - 1, where->z, check) ;
				//delete where ;
				*where = std::move(*temp) ;
				return placeAtNearestFree(where, mapObj, defaultCheck) ;
			}
				
			case 2:
			{
				Position<N> * temp = new Position<N>(where->x + 1, where->y, where->z, check) ;
				//delete where ;
				*where = std::move(*temp) ;
				return placeAtNearestFree(where, mapObj, defaultCheck) ;
			}
				
			case 3:
			{
				Position<N> * temp = new Position<N>(where->x - 1, where->y, where->z, check);
				//delete where ;
				*where =  std::move(*temp) ;
				return placeAtNearestFree(where, mapObj, defaultCheck) ;
			}
			default:
			{
				cerr << "Problem with placeAtNearestFree() \n" ;
				throw new exception() ;
			}
		}
		//mapMembers++ ;
	}
	*Debug::debugOutput << "No empty positions found on map. placeAtNearestFree() threw exception \n" ;
	throw new exception() ;
}

template<class T>
template<typename N>
void GameMap<T>::erase(const Position<N> & currentLoc) {
	if (at(currentLoc) != nullptr) {
		mapMembers-- ;
	}
	intern_map->at(currentLoc.getX())->at(currentLoc.getY()) = nullptr ;
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
template<typename N>
void GameMap<T>::move(Position<N> & currentLoc, Position<N> & toNewLoc) {
	T * temp = at(currentLoc) ;
	erase(currentLoc) ;
	place(toNewLoc, temp) ;
}

template<class T>
template<typename N>
T* GameMap<T>::at(const Position<N> & where) {
	return intern_map->at(where.getX())->at(where.getY()) ;
}

template<class T>
template<typename N>
Position<N> GameMap<T>::currentLoc(T *obj) {
	for (auto i = 0 ; i < intern_map->size() ; i++) {
		for (auto j = 0 ; j < intern_map->at(i)->size() ; j++) {
			if ((intern_map->at(i)->at(j) != nullptr) && ((*(intern_map->at(i)->at(j)) == *obj))) {
				Position<N> l = (Position<N>(i, j, 0)) ;
				return l ;
			}
		}
	}
	/* debug code */
	*(Debug::debugOutput) << "GameMap::currentLoc() throwing exception. No object found at that Position. \n" ;
	throw exception() ;
	/* end debug code */
}

template<class T>
template<typename N>
T* GameMap<T>::remove(Position<N> & currentLoc) {
	T * temp = at(currentLoc) ;
	if (temp != nullptr) {
		mapMembers-- ;
	}
	this->erase(currentLoc) ;
	return temp ;
}

template<class T>
template<typename N>
vector<T*> * GameMap<T>::findNearby(const Position<N> * start, N x_lim, N y_lim) {
	
	searchSuccess = false ;
	vector<T*> * store = new vector<T*>() ;
	const Position<N> * strt = start ;
	Position<N> init = Position<N>(*start) ;
	Navigator nav(Direction::oneDirection, strt, init) ;
	findAllNearby_helper(store, nav, x_lim, y_lim) ;
	return store ;
}

template<class T>
template<typename N>
void GameMap<T>::findAllNearby_helper(vector<T*> * store, Navigator & nav, const N x_lim, const N y_lim) {
	
	//Debug::debugCounter++ ;
	
	if ((at(nav.current) != nullptr) && (nav.current != *(nav.start))) {
		searchSuccess = true ;
		store->push_back(at(nav.current)) ;
	}

	switch (nav.dir) {
		case north : {
			if ((nav.y_travelled() <= y_lim) && (nav.current.y < getYBound())) {
				nav.current.y++ ;
				findAllNearby_helper(store, nav, x_lim, y_lim) ;
			}
			
		}
			break ;
			
				
		case south : {
			if ((nav.y_travelled() <= y_lim) && (nav.current.y > 0)) {
				nav.current.y-- ;
				findAllNearby_helper(store, nav, x_lim, y_lim) ;
			}
		}
			break ;
			
			
		case east : {
			if ((nav.x_travelled() <= x_lim) && (nav.current.x < getXBound())) {
				nav.current.x++ ;
				findAllNearby_helper(store, nav, x_lim, y_lim) ;
			}
		}
			break ;
			
				
		case west : {
			if ((nav.x_travelled() <= x_lim) && (nav.current.x > 0)) {
				nav.current.x-- ;
				findAllNearby_helper(store, nav, x_lim, y_lim) ;
			}
		}
			break ;
			
				
		case ne : {
			bool continue_e = ((nav.x_travelled() <= x_lim) && (nav.current.x < getXBound())) ;
			bool continue_n = ((nav.y_travelled() <= y_lim) && (nav.current.y < getYBound())) ;
			
			if (continue_e || continue_n)
			//if (((nav.x_travelled() < x_lim) && (nav.current.x < getXBound())) && //old code
			//((nav.y_travelled() < y_lim) && (nav.current.y < getYBound())))
			{
				if (continue_n) {
					Position<N> n_loc((nav.current)) ;
					n_loc.y++ ;
					Navigator n_nav = Navigator(Direction::north, nav.start, n_loc) ;
					findAllNearby_helper(store, n_nav, x_lim, y_lim) ;
				}
				
				if (continue_e) {
					Position<N> e_loc((nav.current)) ;
					e_loc.x++ ;
					Navigator e_nav = Navigator(Direction::east, nav.start, e_loc) ;
					findAllNearby_helper(store, e_nav, x_lim, y_lim) ;
				}
				
				if (continue_e && continue_n) {
					nav.current.x++ ;
					nav.current.y++ ;
					//nav.dir = ne ;
					findAllNearby_helper(store, nav, x_lim, y_lim) ;
				}
			}
		}
			break ;
			
				
		case se : {
			bool continue_e = ((nav.x_travelled() <= x_lim) && (nav.current.x < getXBound())) ;
			bool continue_s = ((nav.y_travelled() <= y_lim) && (nav.current.y > 0)) ;
			
			if (continue_e || continue_s)
				//(((nav.x_travelled() < x_lim) && (nav.current.x < getXBound())) &&
			   //((nav.y_travelled() < y_lim) && (nav.current.y > 0)))
			{
				if (continue_s) {
					Position<N> s_loc((nav.current)) ;
					s_loc.y-- ;
					Navigator s_nav = Navigator(Direction::south, nav.start, s_loc) ;
					findAllNearby_helper(store, s_nav, x_lim, y_lim) ;
				}
				
				if (continue_e) {
					Position<N> e_loc((nav.current)) ;
					e_loc.x++ ;
					Navigator e_nav = Navigator(Direction::east, nav.start, e_loc) ;
					findAllNearby_helper(store, e_nav, x_lim, y_lim) ;
				}
			
				if (continue_e && continue_s) {
					nav.current.x++ ;
					nav.current.y-- ;
					//nav.dir = ne ;
					findAllNearby_helper(store, nav, x_lim, y_lim) ;
				}
			}
		}
			break ;
			
				
		case sw : {
			bool continue_w = ((nav.x_travelled() <= x_lim) && (nav.current.x > 0)) ;
			bool continue_s = ((nav.y_travelled() <= y_lim) && (nav.current.y > 0)) ;
			
			if (continue_w || continue_s)
				//(((nav.x_travelled() < x_lim) && (nav.current.x > 0)) &&
				//((nav.y_travelled() < y_lim) && (nav.current.y > 0)))
			{
				if (continue_s) {
					Position<N> s_loc((nav.current)) ;
					s_loc.y-- ;
					Navigator s_nav = Navigator(Direction::south, nav.start, s_loc) ;
					findAllNearby_helper(store, s_nav, x_lim, y_lim) ;
				}
				
				if (continue_w) {
					Position<N> w_loc((nav.current)) ;
					w_loc.x-- ;
					Navigator w_nav = Navigator(Direction::west, nav.start, w_loc) ;
					findAllNearby_helper(store, w_nav, x_lim, y_lim) ;
				}
				
				if (continue_s && continue_w) {
					nav.current.x-- ;
					nav.current.y-- ;
					//nav.dir = sw ;
					findAllNearby_helper(store, nav, x_lim, y_lim) ;
				}
			}
		}
			break ;
			
				
		case nw : {
			bool continue_w = ((nav.x_travelled() <= x_lim) && (nav.current.x > 0)) ;
			bool continue_n = ((nav.y_travelled() <= y_lim) && (nav.current.y < getYBound())) ;
			
	
			if (continue_w || continue_w)
				//(((nav.x_travelled() < x_lim) && (nav.current.x > 0)) &&
			   //((nav.y_travelled() < y_lim) && (nav.current.y < getYBound())))
			{
				if (continue_n) {
					Position<N> n_loc((nav.current)) ;
					n_loc.y++ ;
					Navigator n_nav = Navigator(Direction::north, nav.start, n_loc) ;
					findAllNearby_helper(store, n_nav, x_lim, y_lim) ;
				}
				
				if (continue_w) {
					Position<N> w_loc((nav.current)) ;
					w_loc.x-- ;
					Navigator w_nav = Navigator(Direction::west, nav.start, w_loc) ;
					findAllNearby_helper(store, w_nav, x_lim, y_lim) ;
				}
				
				if (continue_w && continue_n) {
					nav.current.x-- ;
					nav.current.y++ ;
					findAllNearby_helper(store, nav, x_lim, y_lim) ;
				}
			}
		}
			break ;
			
		case oneDirection : { //the best case! (also base case)
			if ((nav.current.y < getYBound())) {
				Position<N> n_loc = Position<N>(*nav.start) ;
				n_loc.y++ ;
				Navigator n_nav(north, nav.start, n_loc) ;
				findAllNearby_helper(store, n_nav, x_lim, y_lim) ;
			}
			
			if ((nav.current.y > 0)) {
				Position<N> s_loc = Position<N>(*nav.start) ;
				s_loc.y-- ;
				Navigator s_nav(south, nav.start, s_loc) ;
				findAllNearby_helper(store, s_nav, x_lim, y_lim) ;
			}
			
			if (nav.current.x < getXBound()) {
				Position<N> e_loc = Position<N>(*nav.start) ;
				e_loc.x++ ;
				Navigator e_nav(east, nav.start, e_loc) ;
				findAllNearby_helper(store, e_nav, x_lim, y_lim) ;
			}
			
			if ((nav.current.x > 0)) {
				Position<N> w_loc = Position<N>(*nav.start) ;
				w_loc.x-- ;
				Navigator w_nav(west, nav.start, w_loc) ;
				findAllNearby_helper(store, w_nav, x_lim, y_lim) ;
			}
			if ((nav.current.x < getXBound()) && ((nav.current.y < getYBound()))) {
				Position<N> ne_loc = Position<N>(*nav.start) ;
				ne_loc.x++ ;
				ne_loc.y++ ;
				Navigator ne_nav(ne, nav.start, ne_loc) ;
				findAllNearby_helper(store, ne_nav, x_lim, y_lim) ;
			}
			if ((nav.current.x < getXBound()) && ((nav.current.y > 0))) {
				Position<N> se_loc = Position<N>(*nav.start) ;
				se_loc.x++ ;
				se_loc.y-- ;
				Navigator se_nav(se, nav.start, se_loc) ;
				findAllNearby_helper(store, se_nav, x_lim, y_lim) ;
			}
			if ((nav.current.x > 0) && ((nav.current.y > 0))) {
				Position<N> sw_loc = Position<N>(*nav.start) ;
				sw_loc.x-- ;
				sw_loc.y-- ;
				Navigator sw_nav(sw, nav.start, sw_loc) ;
				findAllNearby_helper(store, sw_nav, x_lim, y_lim) ;
			}
			if ((nav.current.x > 0) && ((nav.current.y < getYBound()))) {
				Position<N> nw_loc = Position<N>(*nav.start) ;
				nw_loc.x-- ;
				nw_loc.y++ ;
				Navigator nw_nav(nw, nav.start, nw_loc) ;
				findAllNearby_helper(store, nw_nav, x_lim, y_lim) ;
			}
			
		}
			
	}

}



#endif /* defined(__GameWorld__GameMap__) */