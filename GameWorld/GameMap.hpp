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
#include <vector>
#include <list>
#include <climits>

#include "../Util/Util.h"
#include "../Util/Location.h"
#include "../Util/Navigator.h"



template <class T>
class GameMap {
	
private:
	vector< vector<T*>*> * map ;
	
	template<typename N>
	void findAllNearby_helper(vector<T*> * store, Navigator nav, const N x_lim, const N y_lim) ;
	
	template<typename N>
	bool boundsCheck(Location<N> & current) ;

public:
	bool searchSuccess = false ;
	
	template<typename N>
	GameMap(N maxX, N maxY) ;
	
	//GameMap(T &) ; todo
	
	~GameMap() ;

	//GameMap<T> & operator=(const GameMap<T> & rhs) ; todo
	
	unsigned long getXBound() { return map->size() -1 ; } ;
	unsigned long getYBound() { return map->at(0)->size() -1 ; } ;
	
	template<typename N>
	void place(Location<N> & where, T * mapObj) ;
	
	template<typename N>
	void erase(Location<N> & currentLoc) ;
	
	void eraseAll() ;
	
	/**
	 * Moves the object to a new Location<N> on the map, and erases
	 * (calls erase()) on its old Location<N>
	 */
	template<typename N>
	void move(Location<N> & currentLoc, Location<N> & toNewLoc) ;
	
	/**
	 * Returns the first object at this Location<N>
	 */
	template<typename N>
	T * at(Location<N> & where) ;
	
	template<typename N>
	Location<N> currentLoc(T* obj) ;
	
	/**
	 * Similar to erase(), but returns a pointer before erasing
	 */
	template<typename N>
	T * remove(Location<N> & currentLoc) ;
	
	/**
	 * Searches within the specified limits for an object of the specified type.
	 * Returns a nullptr if none found
	 * 
	 * @param start The Location<N> of the object that wants to search for nearby objects
	 * @param maxDistX The maximum distance to search Nitudinally
	 * @param maxDistY The maximum distance to search latitudinally
	 */
	template<typename N>
	vector<T*> * findNearby(const Location<N> & start, const N x_lim, const N y_lim) ;
	
	/*
	 * Draws a grid representing every object on the map. Draws the specified icon at every
	 * index an object is found, though if the object implements getIcon() returning a std::string, and
	 * the parameter icon is left as the default (i.e. ' '), drawSimpleGraphic will call getIcon
	 * and print that at each index. It is recommended that icon consist of only a single char (though
	 * stored inside a string) in order to maintain proportions.
	 *
	 * @param whiteSpace The character to be printed in spaces where there are no objects
	 * @param writeTo The outputstream (std::ostream) that will receive output
	 * @param icon The string representing whatever is stored on this GameMap
	 */
	void drawSimpleGraphic(char whiteSpace = ' ', ostream & writeTo = cout, string icon = " ") ;
} ;

template<class T>
template<typename N>
GameMap<T>::GameMap(N maxX, N maxY) :
	map(new vector< vector<T*>*>())
{
	for (auto i = 0 ; i < maxX ; i++) {
		map->push_back(new vector<T*>()) ;
		for (auto j = 0 ; j < maxY; j++) {
			map->at(i)->push_back(nullptr) ;
		}
	}
}

template<class T>
GameMap<T>::~GameMap() {
	for (auto i = 0 ; i < map->size() ; i++) {
		for (auto j = 0 ; j < map->at(i)->size() ; j++) {
			map->at(i)->at(j) = nullptr ;
		}
	}
}

template<class T>
template<typename N>
void GameMap<T>::place(Location<N> & where, T * mapObj) {
	map->at(where.getX())->at(where.getY()) = mapObj ;
}

template<class T>
template<typename N>
void GameMap<T>::erase(Location<N> & currentLoc) {
	place(currentLoc, nullptr) ;
}

template<class T>
void GameMap<T>::eraseAll() {
	for (auto i = 0 ; i < map->size() ; i++) {
		for (auto j = 0 ; j < map->at(i)->size() ; j++) {
			map->at(i)->at(j) = nullptr ;
		}
	}
}

template<class T>
template<typename N>
void GameMap<T>::move(Location<N> & currentLoc, Location<N> & toNewLoc) {
	T * temp = at(currentLoc) ;
	erase(currentLoc) ;
	place(toNewLoc, temp) ;
}

template<class T>
template<typename N>
T* GameMap<T>::at(Location<N> & where) {
	return map->at(where.getX())->at(where.getY()) ;
}

template<class T>
template<typename N>
Location<N> GameMap<T>::currentLoc(T *obj) {
	for (auto i = 0 ; i < map->size() ; i++) {
		for (auto j = 0 ; j < map->at(i)->size() ; j++) {
			if ((map->at(i)->at(j) != nullptr) && ((*(map->at(i)->at(j)) == *obj))) {
				Location<N> l = (Location<N>((double) i, (double) j, 0)) ;
				return l ;
			}
		}
	}
	cout << "GameMap::currentLoc calling exception" << endl ;
	throw exception() ;
}

template<class T>
template<typename N>
T* GameMap<T>::remove(Location<N> & currentLoc) {
	T * temp = at(currentLoc) ;
	erase(currentLoc) ;
	return temp ;
}

template<class T>
template<typename N>
vector<T*> * GameMap<T>::findNearby(const Location<N> & start, N x_lim, N y_lim) {
	
	vector<T*> * store = new vector<T*>() ;
	const Location<N> strt = Location<N>(start) ;
	Location<N> init = Location<N>(start) ;
	Navigator nav(Direction::oneDirection, strt, init) ;
	findAllNearby_helper(store, nav, x_lim, y_lim) ;
	return store ;
}

template<class T>
template<typename N>
void GameMap<T>::findAllNearby_helper(vector<T*> * store, Navigator nav, const N x_lim, const N y_lim) {
	if (at(nav.current) != nullptr) {
		searchSuccess = true ;
		store->push_back(at(nav.current)) ;
	}
	
	switch (nav.dir) {
		case north : {
			if ((nav.y_travelled() < y_lim) && (nav.current.y < getYBound())) {
				nav.current.y++ ;
				findAllNearby_helper(store, nav, x_lim, y_lim) ;
			}
			
		}
			break ;
			
				
		case south : {
			if ((nav.y_travelled() < y_lim) && (nav.current.y > 0)) {
				nav.current.y-- ;
				findAllNearby_helper(store, nav, x_lim, y_lim) ;
			}
		}
			break ;
			
			
		case east : {
			if ((nav.x_travelled() < x_lim) && (nav.current.x < getXBound())) {
				nav.current.x++ ;
				findAllNearby_helper(store, nav, x_lim, y_lim) ;
			}
		}
			break ;
			
				
		case west : {
			if ((nav.x_travelled() < x_lim) && (nav.current.x > 0)) {
				nav.current.x-- ;
				findAllNearby_helper(store, nav, x_lim, y_lim) ;
			}
		}
			break ;
			
				
		case ne : {
			if (((nav.x_travelled() < x_lim) && (nav.current.x < getXBound())) &&
				((nav.y_travelled() < y_lim) && (nav.current.y < getYBound())))
			{
				Location<N> n_loc((nav.current)) ;
				n_loc.y++ ;
				Navigator n_nav = Navigator(Direction::north, nav.start, n_loc) ;
				findAllNearby_helper(store, n_nav, x_lim, y_lim) ;
				
				Location<N> e_loc((nav.current)) ;
				e_loc.x++ ;
				Navigator e_nav = Navigator(Direction::east, nav.start, e_loc) ;
				findAllNearby_helper(store, e_nav, x_lim, y_lim) ;
				
				nav.current.x++ ;
				nav.current.y++ ;
				//nav.dir = ne ;
				findAllNearby_helper(store, nav, x_lim, y_lim) ;
			}
		}
			break ;
			
				
		case se : {
			if (((nav.x_travelled() < x_lim) && (nav.current.x < getXBound())) &&
				((nav.y_travelled() < y_lim) && (nav.current.y > 0)))
			{
				Location<N> s_loc((nav.current)) ;
				s_loc.y-- ;
				Navigator s_nav = Navigator(Direction::south, nav.start, s_loc) ;
				findAllNearby_helper(store, s_nav, x_lim, y_lim) ;
				
				Location<N> e_loc((nav.current)) ;
				e_loc.x++ ;
				Navigator e_nav = Navigator(Direction::east, nav.start, e_loc) ;
				findAllNearby_helper(store, e_nav, x_lim, y_lim) ;
				
				nav.current.x++ ;
				nav.current.y-- ;
				//nav.dir = ne ;
				findAllNearby_helper(store, nav, x_lim, y_lim) ;
			}
		}
			break ;
			
				
		case sw : {
			if (((nav.x_travelled() < x_lim) && (nav.current.x > 0)) &&
				((nav.y_travelled() < y_lim) && (nav.current.y > 0)))
			{
				Location<N> s_loc((nav.current)) ;
				s_loc.y-- ;
				Navigator s_nav = Navigator(Direction::south, nav.start, s_loc) ;
				findAllNearby_helper(store, s_nav, x_lim, y_lim) ;
				
				Location<N> w_loc((nav.current)) ;
				w_loc.x-- ;
				Navigator w_nav = Navigator(Direction::west, nav.start, w_loc) ;
				findAllNearby_helper(store, w_nav, x_lim, y_lim) ;
				
				nav.current.x-- ;
				nav.current.y-- ;
				//nav.dir = sw ;
				findAllNearby_helper(store, nav, x_lim, y_lim) ;
			}
		}
			break ;
			
				
		case nw : {
			if (((nav.x_travelled() < x_lim) && (nav.current.x > 0)) &&
				((nav.y_travelled() < y_lim) && (nav.current.y < getYBound())))
			{
				Location<N> n_loc((nav.current)) ;
				n_loc.y++ ;
				Navigator n_nav = Navigator(Direction::north, nav.start, n_loc) ;
				findAllNearby_helper(store, n_nav, x_lim, y_lim) ;
				
				Location<N> w_loc((nav.current)) ;
				w_loc.x-- ;
				Navigator w_nav = Navigator(Direction::west, nav.start, w_loc) ;
				findAllNearby_helper(store, w_nav, x_lim, y_lim) ;
				
				nav.current.x-- ;
				nav.current.y++ ;
				findAllNearby_helper(store, nav, x_lim, y_lim) ;
			}
		}
			break ;
			
		case oneDirection : { //the best case! (also base case)
			if ((nav.current.y < getYBound())) {
				Location<N> n_loc = Location<N>(nav.start) ;
				n_loc.y++ ;
				Navigator n_nav(north, nav.start, n_loc) ;
				findAllNearby_helper(store, n_nav, x_lim, y_lim) ;
			}
			
			if ((nav.current.y > 0)) {
				Location<N> s_loc = Location<N>(nav.start) ;
				s_loc.y-- ;
				Navigator s_nav(south, nav.start, s_loc) ;
				findAllNearby_helper(store, s_nav, x_lim, y_lim) ;
			}
			
			if (nav.current.x < getXBound()) {
				Location<N> e_loc = Location<N>(nav.start) ;
				e_loc.x++ ;
				Navigator e_nav(east, nav.start, e_loc) ;
				findAllNearby_helper(store, e_nav, x_lim, y_lim) ;
			}
			
			if ((nav.current.x > 0)) {
				Location<N> w_loc = Location<N>(nav.start) ;
				w_loc.x-- ;
				Navigator w_nav(west, nav.start, w_loc) ;
				findAllNearby_helper(store, w_nav, x_lim, y_lim) ;
			}
			if ((nav.current.x < getXBound()) && ((nav.current.y < getYBound()))) {
				Location<N> ne_loc = Location<N>(nav.start) ;
				ne_loc.x++ ;
				ne_loc.y++ ;
				Navigator ne_nav(ne, nav.start, ne_loc) ;
				findAllNearby_helper(store, ne_nav, x_lim, y_lim) ;
			}
			if ((nav.current.x < getXBound()) && ((nav.current.y > 0))) {
				Location<N> se_loc = Location<N>(nav.start) ;
				se_loc.x++ ;
				se_loc.y-- ;
				Navigator se_nav(se, nav.start, se_loc) ;
				findAllNearby_helper(store, se_nav, x_lim, y_lim) ;
			}
			if ((nav.current.x > 0) && ((nav.current.y > 0))) {
				Location<N> sw_loc = Location<N>(nav.start) ;
				sw_loc.x-- ;
				sw_loc.y-- ;
				Navigator sw_nav(sw, nav.start, sw_loc) ;
				findAllNearby_helper(store, sw_nav, x_lim, y_lim) ;
			}
			if ((nav.current.x > 0) && ((nav.current.y < getYBound()))) {
				Location<N> nw_loc = Location<N>(nav.start) ;
				nw_loc.x-- ;
				nw_loc.y++ ;
				Navigator nw_nav(se, nav.start, nw_loc) ;
				findAllNearby_helper(store, nw_nav, x_lim, y_lim) ;
			}
			
		}
			
	}

}

template<class T>
template<typename N>
bool GameMap<T>::boundsCheck(Location<N> & current) {
	if ((current.x > 0) && (current.y > 0)) {
		if ((current.x < (map->size() - 1)) && (current.y < (map->at(0)->size() - 1))) {
			return true ;
		}
	}
	return false ;
}

template<class T>
void GameMap<T>::drawSimpleGraphic(char whiteSpace, ostream & writeTo, string icon) {
	for (auto i = 0 ; i < map->at(0)->size() ; i++) {
		for (auto j = 0 ; j < map->size() ; j++) {
			if (map->at(j)->at(i) != nullptr) {
				writeTo << this->map->at(j)->at(i)->getIcon() ;
			}
			else {
				writeTo << ' ' ;
			}
		}
		writeTo << endl ;
	}
}

#endif /* defined(__GameWorld__GameMap__) */
