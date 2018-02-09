//
//  Util.cpp
//  AsteroidsLikeTest
//
//  Created by Adam J Leuer on 2018-2-7.
//  Copyright © 2018 Adam James Leuer. All rights reserved.
//

#include <list>

#include <gtest/gtest.h>

#include "../../Util/Util.hpp"

using std::list;
using std::byte;
using Util::contains;

TEST(contains, correctlyDeterminesIfAContainerHoldsTheGivenElement) {
	
	byte byteToFind = byte(0x0A);
	list<byte> bytes ({byte(0xFE), byte(0x11), byte(0x0F), byte(0x0A), byte(0x00), byte(0x11)});
	
	EXPECT_TRUE(contains(bytes, byteToFind));
}

TEST(contains, correctlyDeterminesThatAContainerDoesNotHoldTheGivenElement) {
	
	byte byteToFind = byte(0x0A);
	list<byte> bytes ({byte(0xFE), byte(0x11), byte(0x0F), byte(0x0B), byte(0x00), byte(0x11)});
	
	EXPECT_FALSE(contains(bytes, byteToFind));
}
