//
//  main.cpp
//  GameWorld
//
//  Created by Adam James Leuer on 3/11/14.
//  Copyright (c) 2014 Adam James Leuer. All rights reserved.
//

#include <ncurses.h>
#include <unistd.h>

#include <iostream>

#include "../GameWorld/World.h"
#include "../Adapter/Adapter.h"

using namespace std ;

int main(void)
{
	
	srand(time(NULL)) ;



	for (unsigned i = 0 ; i < 100 ; i++) {
		int r = rand() % 4 ;
		
		switch (r) {
			case 0:
			{
				mvwaddstr(stdscr, rand()%35, rand()%150, /*(const chtype *)*/ "Wow");
				wrefresh(stdscr) ;
				usleep(500000) ; //sleep for 250000 microseconds (i.e 0.25 sec)
				wclear(stdscr) ;
				wrefresh(stdscr) ; //need to call again(?)
				break ;
			}
			case 1:
			{
				mvwaddstr(stdscr, rand()%35, rand()%150, "Very programming");
				wrefresh(stdscr) ;
				usleep(500000) ;
				wclear(stdscr) ;
				wrefresh(curscr);
				break;
			}
			case 2:
			{
				mvwaddstr(stdscr, rand()%35, rand()%150, "🔰");
				wrefresh(stdscr) ;
				usleep(500000) ;
				wclear(stdscr) ;
				wrefresh(curscr);
				break;
			}
			default:
			{
				mvwaddstr(stdscr, rand()%35, rand()%150, "So skill");
				wrefresh(stdscr) ;
				usleep(500000) ;
				wclear(stdscr) ;
				wrefresh(curscr);
				break;
			}
		}
	}
	
	
	endwin();
	return 0;
}

