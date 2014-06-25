
#ifdef __APPLE__

#include <AppKit/NSScreen.h>
#include <Foundation/NSArray.h>

#include "Util.hpp"


bool checkHiDPI() {
	float displayScale = 1 ;
	
    if ([[NSScreen mainScreen] respondsToSelector:@selector(backingScaleFactor)]) {
        NSArray *screens = [NSScreen screens];
        for (int i = 0; i < [screens count]; i++) {
            float s = [[screens objectAtIndex:i] backingScaleFactor];
            if (s > displayScale)
                displayScale = s;
        }
    }
	
	return true ;
}
#endif