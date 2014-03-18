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

using namespace std ;

int main(void)
{
	
	
	char first[24];
	char last[32];
	
	setlocale(LC_ALL, ""); //allows printing more types of characters (?)
	
	initscr();
	
	cbreak() ; noecho() ; //character-at-a-time  input  without echoing
	curs_set(0) ; //sets cursor to invisible

	
	mvwaddstr(stdscr, 10, 10, /*(const chtype *)*/ "Wow");
	usleep(useconds_t)
	refresh();
	
	//wgetstr(stdscr, first); - gets a string
	
	mvwaddstr(stdscr, 30, 30, "Very programming");
	refresh();
	
	
	printw("Pleased to meet you, %s %s!",first,last);
	refresh();
	getch();
	
	endwin();
	return 0;
}

