//
//  GameObject.cpp
//  GameWorld
//
//  Created by Adam James Leuer on 3/11/14.
//  Copyright (c) 2014 Adam James Leuer. All rights reserved.
//

#include "GameObject.h"

unsigned GameObject::IDs = 0 ;

GameObject::GameObject() :
	ID(IDs)
{
	IDs++ ;
}

GameObject::GameObject(const GameObject & other) :
	ID(IDs)
{
	IDs++ ;
	//copy member vars
}

GameObject::~GameObject() {
	//nothing yet
}

GameObject & GameObject::operator=(const GameObject &) {
	if (this != &rhs) {
		<#statements#>
	}
}

void GameObject::operator()() {
	//todo
}

void GameObject::operator()(GameObject &sentObject) {
	//todo
}

void GameObject::notify() {
	//todo
}

void GameObject::passMessage(Message *, GameObject &recipient) {
	//todo
}





stringstream * GameObject::draw() {
	//todo
	return new stringstream() ;
}
