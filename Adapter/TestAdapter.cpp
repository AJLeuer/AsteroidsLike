//
//  TestAdapter.cpp
//  GameWorld
//
//  Created by Adam James Leuer on 3/19/14.
//  Copyright (c) 2014 Adam James Leuer. All rights reserved.
//

#include "TestAdapter.h"

TestAdapter::TestAdapter(int n) :
	AdapterInterface(this, n) {}

void TestAdapter::init() {
	this->AdapterInterface::init() ;
	
	//local initializations:
}

void TestAdapter::show() {
	while (World::isRunning()) {
		for (auto i = 0 ; i < (*WorldObjects)->size() ; i++) {
			(*WorldObjects)->at(i)->textDescription(&cout) ;
			Location trans = AdapterUtil::transLocation(*((*WorldObjects)->at(i)->getLocation())) ;
			cout << "Tranlated location: " ;
			cout << trans.toString() << endl ;
			cout << (*WorldObjects)->at(i)->getIcon().c_str() << endl << endl ;
		}
	}
}

void TestAdapter::close() {
	this->AdapterInterface::close() ;
	
	//local cleanup:
	
}
