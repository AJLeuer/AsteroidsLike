
#ifdef __APPLE__

#include <AppKit/NSScreen.h>
#include <Foundation/NSArray.h>

#include "Util.hpp"

float displayScalingFactorOSX() {
    float displayScale = 1.0 ;
	
    if ([[NSScreen mainScreen] respondsToSelector:@selector(backingScaleFactor)]) {
        NSArray *screens = [NSScreen screens];
        for (int i = 0; i < [screens count]; i++) {
            float s = [[screens objectAtIndex:i] backingScaleFactor];
            if (s > displayScale)
                displayScale = s;
        }
    }
    return displayScale ;
}

bool checkHiDPIOSX() {

	bool hidpi = (displayScalingFactorOSX() == 2.0) ? true : false ;
	
	return hidpi ;
}



#endif