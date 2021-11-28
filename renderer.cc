
#include <memory>
#include <ncurses.h>
#include <string>

#include "renderer.h"
#include "window.h"
#include "config.h"

CursesRenderer::CursesRenderer()
{
    curses_init();

    game_win = std::make_unique<Window>(SCREEN_HEIGHT-STATUS_HEIGHT, SCREEN_WIDTH, 0, 0);
    status_win = std::make_unique<Window>(STATUS_HEIGHT, SCREEN_WIDTH, SCREEN_WIDTH-STATUS_HEIGHT, 0);

    refresh();
}

CursesRenderer::~CursesRenderer()
{
    curses_exit();
}

void
CursesRenderer::DrawChar(char c, int x, int y)
{
    if (x < 0 || y < 0) return; 
    if (x > SCREEN_WIDTH || y > SCREEN_HEIGHT) return; // TODO account for borders and stuff

    mvwprintw(game_win->getwin(), y, x, std::string(1, c).c_str());
}

void
CursesRenderer::DrawBox(char c, int x, int y, int w, int h)
{
    const char* boxline = std::string(w, c).c_str();
    for (int i = 0; i < h; ++i)
        mvwprintw(game_win->getwin(), y+i, x, boxline);
}

void
CursesRenderer::WriteStatus(const std::string& s, int line)
{
    mvwprintw(status_win->getwin(), line, 0, s.c_str());
}

void
CursesRenderer::RenderGameScreen()
{
    wrefresh(game_win->getwin());
}

void
CursesRenderer::RenderStatusScreen()
{
    wrefresh(status_win->getwin());
}

void
CursesRenderer::ClearGameScreen()
{
    werase(game_win->getwin());
}

void
CursesRenderer::ClearStatusScreen()
{
    werase(status_win->getwin());
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

