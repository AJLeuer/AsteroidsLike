//
//  Navigator.cpp
//  GameWorld
//
//  Created by Adam James Leuer on 4/8/14.
//  Copyright (c) 2014 Adam James Leuer. All rights reserved.
//

#include "Navigator.h"

Navigator::Navigator(int i) {}

Navigator::Navigator(Direction d, const Position<long> * s, Position<long> c) :
	dir{d}, start{s}, current{c} {}

Navigator::Navigator(const Navigator & other) :
	dir{other.dir}, start{other.start}, current{other.current} {}

Navigator::Navigator(Navigator && other) :
	dir(other.dir), start(other.start), current(std::move(other.current)) {
		other.start = nullptr ;
}

Navigator::~Navigator() {
	
	//delete start ;
}

Navigator & Navigator::operator=(Navigator && rhs) {
	if (this != &rhs) {
		this->dir = rhs.dir ;
		this->start = rhs.start ;
		this->current = std::move(rhs.current) ;
		rhs.start = nullptr ;
	}
	return *this ;
	
}

Navigator & Navigator::operator=(const Navigator & other) {
	if (this != &other) {
		dir = other.dir ;
		start = other.start ;
		current = other.current ;
	}
	return *this ;
}




