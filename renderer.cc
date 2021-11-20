
#include <ncurses.h>
#include "renderer.h"

CursesRenderer::CursesRenderer()
{

}

CursesRenderer::~CursesRenderer()
{

}

void
curses_init()
{
    // some startup options for curses
	initscr();
	cbreak();
	noecho();
	curs_set(0);
	keypad(stdscr, true);
}

void
curses_exit()
{
    endwin();
}

