
#include <memory>
#include <ncurses.h>

#include "renderer.h"
#include "window.h"
#include "config.h"

CursesRenderer::CursesRenderer()
{
    curses_init();

    game_win = std::make_unique<GameWindow>(SCREEN_HEIGHT-STATUS_HEIGHT, SCREEN_WIDTH, 0, 0);
    status_win = std::make_unique<Window>(STATUS_HEIGHT, SCREEN_WIDTH, SCREEN_WIDTH-STATUS_HEIGHT, 0);

    refresh();
    getch();
}

CursesRenderer::~CursesRenderer()
{
    curses_exit();
}

void
CursesRenderer::curses_init()
{
    // need to make sure this isn't called twice

    // some startup options for curses
	initscr();
	cbreak();
	noecho();
	curs_set(0);
	keypad(stdscr, true);
}

void
CursesRenderer::curses_exit()
{
    endwin();
}

