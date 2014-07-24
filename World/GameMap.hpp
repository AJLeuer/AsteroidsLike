//
//  GameMap.h
//  World
//
//  Created by Adam James Leuer on 4/7/14.
//  Copyright (c) 2014 Adam James Leuer. All rights reserved.
//

#ifndef __GameWorld__GameMap__
#define __GameWorld__GameMap__

#include <iostream>
#include <queue>
#include <deque>
#include <vector>
#include <list>
#include <climits>

#include "../Util/Debug.h"
#include "../Util/Util.hpp"
#include "../Util/Position.hpp"
#include "../Util/Navigator.hpp"

#include "GameState.hpp"

#include "../Control/Configuration.h"


template <class T>
class GameMap {
	
private:
	int mapMembers = 0 ;
	vector< vector< list<T *> > *> * intern_map ;
	
	template<typename N>
	void findAllNearby_helper(vector<T*> * store, Navigator<N> & nav, const N x_lim, const N y_lim) ;
	
	GameMap<string> * gmDebug ; //debug

public:
	bool searchSuccess = false ;
	
	template<typename N>
	GameMap(N maxX, N maxY) ;
	
	//GameMap(T &) ; todo
	
	~GameMap() ;

	//GameMap<T> & operator=(const GameMap<T> & rhs) ; //todo
	
	vector< vector< deque<T *> > *> * getMapVect() { return this->intern_map ; } ;
	
	unsigned long getXBound() { return intern_map->size() -1 ; } ;
	unsigned long getYBound() { return intern_map->at(0)->size() -1 ; } ;
	
	template<typename N>
	void place(Position<N> * where, T * pointerToOriginalObject, const BoundsCheck<N> check) ;
	
	/*
	template<typename N>
	void placeAtNearestFree(Position<N> * where, T * mapObj, BoundsCheck<N> check) ; */
	
	template<typename N>
	void erase(const Position<N> * currentLoc, T * pointerToOriginalObject) ;
	
	/**
	 * Moves the object to a new Position<N> on the map, and erases
	 * (calls erase()) on its old Position<N>
	 */
	template<typename N>
	void map_move(const Position<N> & currentLoc, const Position<N> & toNewLoc, T * pointerToOriginalObject) ;
	
	/**
	 * Returns the first object at this Position<N>
	 */
	template<typename N>
	list<T *> * at_pos(const Position<N> * where) ;
	
	template<typename N>
	Position<N> currentLoc(T* obj) ;
	
	/**
	 * Searches within the specified limits for an object of the specified type.
	 * Returns a nullptr if none found
	 * 
	 * @param start The Position<N> of the object that wants to search for nearby objects
	 * @param x_lim The maximum distance to search longtitudinally
	 * @param y_lim The maximum distance to search latitudinally
	 */
	template<typename N>
	vector<T*> * findNearby(const Position<N> * start, const N x_lim, const N y_lim) ;
	
} ;

template<class T>
template<typename N>
GameMap<T>::GameMap(N maxX, N maxY) :
	intern_map(new vector< vector< list<T *> > *>()),
	gmDebug(nullptr)
{
	for (auto i = 0 ; i < maxX ; i++) {
		intern_map->push_back(new vector< list<T *> >()) ;
		for (auto j = 0 ; j < maxY; j++) {
			intern_map->at(i)->push_back(list<T *>()) ;
		}
	}
}

template<class T>
GameMap<T>::~GameMap() {
	
	mapMembers = 0 ; 
	
	//delete all the vectors inside intern_map
	for (auto i = 0 ; i < intern_map->size() ; i++) {
		/*for (auto j = 0 ; j < intern_map->at(i)->size() ; j++) {
			delete intern_map->at(i)->at(j) ;
		} */
		delete intern_map->at(i) ;
	}
	delete intern_map ;
	
	if (gmDebug != nullptr) {
		delete gmDebug ;
	}
}

/**
 * Places pointerToOriginalObject at Position where.
 */
template<class T>
template<typename N>
void GameMap<T>::place(Position<N> * where, T * pointerToOriginalObject, const BoundsCheck<N> check) {
	if (pointerToOriginalObject == nullptr) {
		cerr << "place() and placeAtNearestFree() cannot be used to place nullptrs. Use erase and eraseAll() \n" ;
		throw exception() ;
	}
	where->checkBounds(check) ;
	intern_map->at(where->getIntX())->at(where->getIntY()).push_back(pointerToOriginalObject) ;
	mapMembers++ ;
}

/*
template<class T>
template<typename N>
void GameMap<T>::placeAtNearestFree(Position<N> * where, T * mapObj, const BoundsCheck<N> check) {
	if (mapObj == nullptr) {
		cerr << "place() and placeAtNearestFree() cannot be used to place nullptrs. Use erase and eraseAll() \n" ;
		throw exception() ;
	}
	if (at(where) == nullptr) {
		intern_map->at(where->getIntX())->at(where->getIntY()) = {mapObj} ;
		mapMembers++ ;
		return ;
	}
	else {
		
		FastRand<unsigned> rand(0, 3) ;
		unsigned swt = rand.nextValue() ;
		
		switch (swt) {
			case 0:
			{
				Position<N> * here = new Position<N>(where->getIntX(), where->getIntY() + 1, where->getIntZ(), check) ;
				return placeAtNearestFree(here, mapObj, BoundsCheck<N>::defaultCheck) ;
			}

			case 1:
			{
				Position<N> * here = new Position<N>(where->getIntX(), where->getIntY() - 1, where->getIntZ(), check) ;
				return placeAtNearestFree(here, mapObj, BoundsCheck<N>::defaultCheck) ;
			}
				
			case 2:
			{
				Position<N> * here = new Position<N>(where->getIntX() + 1, where->getIntY(), where->getIntZ(), check) ;
				return placeAtNearestFree(here, mapObj, BoundsCheck<N>::defaultCheck) ;
			}
				
			case 3:
			{
				Position<N> * here = new Position<N>(where->getIntX() - 1, where->getIntY(), where->getIntZ(), check);
				return placeAtNearestFree(here, mapObj, BoundsCheck<N>::defaultCheck) ;
			}
			default:
			{
				cerr << "Problem with placeAtNearestFree() \n" ;
				throw exception() ;
			}
		}
		//mapMembers++ ;
	}
	cerr << "No empty positions found on map. placeAtNearestFree() threw exception \n" ;
	throw exception() ;
}
 */


template<class T>
template<typename N>
void GameMap<T>::map_move(const Position<N> & currentLoc, const Position<N> & toNewLoc, T * pointerToOriginalObject) {
	list<T *> * temp = at(currentLoc) ;
	erase(&currentLoc, pointerToOriginalObject) ;
	place(toNewLoc, pointerToOriginalObject, temp) ;
}

template<class T>
template<typename N>
list<T *> * GameMap<T>::at_pos(const Position<N> * where) {
	unsigned x = where->getIntX() ;
	unsigned y = where->getIntY() ;
	return &(intern_map->at(x)->at(y)) ;
}

template<class T>
template<typename N>
Position<N> GameMap<T>::currentLoc(T * obj) {
	
	for (auto i = 0 ; i < intern_map->size() ; i++) {
		
		for (auto j = 0 ; j < intern_map->at(i)->size() ; j++) {
			
			auto containingList = intern_map->at(i)->at(j) ;
				
			for (auto k = containingList->begin(); k < containingList->end() ; k++) {
					
				if (**k == *obj) {
					return Position<N>(i, j, 0) ;
				}
			}
		}
	}
	cerr << "GameMap::currentLoc() throwing exception. No object found at that Position. \n" ;
	throw exception() ;
}

template<class T>
template<typename N>
void GameMap<T>::erase(const Position<N> * currentLoc, T * pointerToOriginalObject) {
	
	list<T *> * containingList = at_pos(currentLoc) ;
	
	int x_ = currentLoc->getIntX() ;
	int y_ = currentLoc->getIntY() ;

	bool badpos = true ;
		
    unsigned ct = 0, erased = 0 ; /* debug vars, remove */
	for (auto i = containingList->begin(); i != containingList->end() ; i++, ct++) {
        
		auto sz = containingList->size() ; /* debug var */

		if (**i == *pointerToOriginalObject) {
			containingList->erase(i) ;
            
            /* Debug code */
            stringstream st ;
            st << "GameMap erased pointer at the following position: " << *currentLoc << '\n' ;
            DebugOutput << st.rdbuf() ;
            /* end debug code */
            
			mapMembers-- ;
            erased++ ;
			badpos = false ;
			break ; //break to avoid errors
		}
	}
	if (badpos) {
		/* Debug code */
		stringstream ss ;
		ss << "GameMap::erase() called with a bad position. Debug." << '\n' ;
		DebugOutput << ss.rdbuf() ;
		/* End debug code */
	}
}


template<class T>
template<typename N>
vector<T*> * GameMap<T>::findNearby(const Position<N> * start, N x_lim, N y_lim) {
	
	searchSuccess = false ;
	vector<T*> * store = new vector<T*>() ;
	const Position<N> * strt = start ;
	Position<N> init = Position<N>(*start) ;
	Navigator<N> nav(Direction::noDirection, strt, init) ;
	findAllNearby_helper(store, nav, x_lim, y_lim) ;
	return store ;
}

template<class T>
template<typename N>
void GameMap<T>::findAllNearby_helper(vector<T*> * store, Navigator<N> & nav, const N x_lim, const N y_lim) {
	
	//Debug::debugCounter++ ;
	
	auto currentList = at_pos(& nav.current) ;
	
	if ((currentList->size() > 0) && (nav.current != *(nav.start))) {
		
		searchSuccess = true ;
		list<T *> * templist = at_pos(&nav.current) ;
		
		for	(auto i = templist->begin() ; i != templist->end() ; i++) {
			store->push_back(*i) ;
		}
		
	}

	switch (nav.dir) {
		case north : {
			if ((nav.y_travelled() <= y_lim) && (nav.current.getIntY() < getYBound())) {
				nav.current.y_plus_one() ;
				findAllNearby_helper(store, nav, x_lim, y_lim) ;
			}
		}
		break ;
			
				
		case south : {
			if ((nav.y_travelled() <= y_lim) && (nav.current.getIntY() > 0)) {
				nav.current.y_minus_one() ;
				findAllNearby_helper(store, nav, x_lim, y_lim) ;
			}
		}
		break ;
			
			
		case east : {
			if ((nav.x_travelled() <= x_lim) && (nav.current.getIntX() < getXBound())) {
				nav.current.x_plus_one() ;
				findAllNearby_helper(store, nav, x_lim, y_lim) ;
			}
		}
		break ;
			
				
		case west : {
			if ((nav.x_travelled() <= x_lim) && (nav.current.getIntX() > 0)) {
				nav.current.x_minus_one() ;
				findAllNearby_helper(store, nav, x_lim, y_lim) ;
			}
		}
		break ;
			
				
		case ne : {
			bool continue_e = ((nav.x_travelled() <= x_lim) && (nav.current.getIntX() < getXBound())) ;
			bool continue_n = ((nav.y_travelled() <= y_lim) && (nav.current.getIntY() < getYBound())) ;
			
			if (continue_e || continue_n)
			//if (((nav.x_travelled() < x_lim) && (nav.current.x < getXBound())) && //old code
			//((nav.y_travelled() < y_lim) && (nav.current.y < getYBound())))
			{
				if (continue_n) {
					Position<N> n_loc((nav.current)) ;
					n_loc.y_plus_one() ;
					Navigator<N> n_nav = Navigator<N>(Direction::north, nav.start, n_loc) ;
					findAllNearby_helper(store, n_nav, x_lim, y_lim) ;
				}
				
				if (continue_e) {
					Position<N> e_loc((nav.current)) ;
					e_loc.x_plus_one() ;
					Navigator<N> e_nav = Navigator<N>(Direction::east, nav.start, e_loc) ;
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
			bool continue_e = ((nav.x_travelled() <= x_lim) && (nav.current.getIntX() < getXBound())) ;
			bool continue_s = ((nav.y_travelled() <= y_lim) && (nav.current.getIntY() > 0)) ;
			
			if (continue_e || continue_s)
				//(((nav.x_travelled() < x_lim) && (nav.current.x < getXBound())) &&
			   //((nav.y_travelled() < y_lim) && (nav.current.y > 0)))
			{
				if (continue_s) {
					Position<N> s_loc((nav.current)) ;
					s_loc.y_minus_one() ;
					Navigator<N> s_nav = Navigator<N>(Direction::south, nav.start, s_loc) ;
					findAllNearby_helper(store, s_nav, x_lim, y_lim) ;
				}
				
				if (continue_e) {
					Position<N> e_loc((nav.current)) ;
					e_loc.x_plus_one() ;
					Navigator<N> e_nav = Navigator<N>(Direction::east, nav.start, e_loc) ;
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
			bool continue_w = ((nav.x_travelled() <= x_lim) && (nav.current.getIntX() > 0)) ;
			bool continue_s = ((nav.y_travelled() <= y_lim) && (nav.current.getIntY() > 0)) ;
			
			if (continue_w || continue_s)
				//(((nav.x_travelled() < x_lim) && (nav.current.x > 0)) &&
				//((nav.y_travelled() < y_lim) && (nav.current.y > 0)))
			{
				if (continue_s) {
					Position<N> s_loc((nav.current)) ;
					s_loc.y_minus_one() ;
					Navigator<N> s_nav = Navigator<N>(Direction::south, nav.start, s_loc) ;
					findAllNearby_helper(store, s_nav, x_lim, y_lim) ;
				}
				
				if (continue_w) {
					Position<N> w_loc((nav.current)) ;
					w_loc.x_minus_one() ;
					Navigator<N> w_nav = Navigator<N>(Direction::west, nav.start, w_loc) ;
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
			bool continue_w = ((nav.x_travelled() <= x_lim) && (nav.current.getIntX() > 0)) ;
			bool continue_n = ((nav.y_travelled() <= y_lim) && (nav.current.getIntY() < getYBound())) ;
			
	
			if (continue_w || continue_w)
				//(((nav.x_travelled() < x_lim) && (nav.current.x > 0)) &&
			   //((nav.y_travelled() < y_lim) && (nav.current.y < getYBound())))
			{
				if (continue_n) {
					Position<N> n_loc((nav.current)) ;
					n_loc.y_plus_one() ;
					Navigator<N> n_nav = Navigator<N>(Direction::north, nav.start, n_loc) ;
					findAllNearby_helper(store, n_nav, x_lim, y_lim) ;
				}
				
				if (continue_w) {
					Position<N> w_loc((nav.current)) ;
					w_loc.x_minus_one() ;
					Navigator<N> w_nav = Navigator<N>(Direction::west, nav.start, w_loc) ;
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
			if ((nav.current.getIntY() < getYBound())) {
				Position<N> n_loc = Position<N>(*nav.start) ;
				n_loc.y_plus_one() ;
				Navigator<N> n_nav(north, nav.start, n_loc) ;
				findAllNearby_helper(store, n_nav, x_lim, y_lim) ;
			}
			
			if ((nav.current.getIntY() > 0)) {
				Position<N> s_loc = Position<N>(*nav.start) ;
				s_loc.y_minus_one() ;
				Navigator<N> s_nav(south, nav.start, s_loc) ;
				findAllNearby_helper(store, s_nav, x_lim, y_lim) ;
			}
			
			if (nav.current.getIntX() < getXBound()) {
				Position<N> e_loc = Position<N>(*nav.start) ;
				e_loc.x_plus_one() ;
				Navigator<N> e_nav(east, nav.start, e_loc) ;
				findAllNearby_helper(store, e_nav, x_lim, y_lim) ;
			}
			
			if ((nav.current.getIntX() > 0)) {
				Position<N> w_loc = Position<N>(*nav.start) ;
				w_loc.x_minus_one() ;
				Navigator<N> w_nav(west, nav.start, w_loc) ;
				findAllNearby_helper(store, w_nav, x_lim, y_lim) ;
			}
			if ((nav.current.getIntX() < getXBound()) && ((nav.current.getIntY() < getYBound()))) {
				Position<N> ne_loc = Position<N>(*nav.start) ;
				ne_loc.x_plus_one() ;
				ne_loc.y_plus_one() ;
				Navigator<N> ne_nav(ne, nav.start, ne_loc) ;
				findAllNearby_helper(store, ne_nav, x_lim, y_lim) ;
			}
			if ((nav.current.getIntX() < getXBound()) && ((nav.current.getIntY() > 0))) {
				Position<N> se_loc = Position<N>(*nav.start) ;
				se_loc.x_plus_one() ;
				se_loc.y_minus_one() ;
				Navigator<N> se_nav(se, nav.start, se_loc) ;
				findAllNearby_helper(store, se_nav, x_lim, y_lim) ;
			}
			if ((nav.current.getIntX() > 0) && ((nav.current.getIntY() > 0))) {
				Position<N> sw_loc = Position<N>(*nav.start) ;
				sw_loc.x_minus_one() ;
				sw_loc.y_minus_one() ;
				Navigator<N> sw_nav(sw, nav.start, sw_loc) ;
				findAllNearby_helper(store, sw_nav, x_lim, y_lim) ;
			}
			if ((nav.current.getIntX() > 0) && ((nav.current.getIntY() < getYBound()))) {
				Position<N> nw_loc = Position<N>(*nav.start) ;
				nw_loc.x_minus_one() ;
				nw_loc.y_plus_one() ;
				Navigator<N> nw_nav(nw, nav.start, nw_loc) ;
				findAllNearby_helper(store, nw_nav, x_lim, y_lim) ;
			}
			
		}
			
	}

}



#endif /* defined(__GameWorld__GameMap__) */
