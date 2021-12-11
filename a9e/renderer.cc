
#include <ncurses.h>
#include "config.h"
#include "renderer.h"

CursesRenderer::CursesRenderer(): Renderer{}
{
    CursesInit();

    game_win = std::make_unique<Window>(SCREEN_HEIGHT-STATUS_HEIGHT, SCREEN_WIDTH, 0, 0);
    status_win = std::make_unique<Window>(STATUS_HEIGHT, SCREEN_WIDTH, SCREEN_HEIGHT-STATUS_HEIGHT, 0);

    refresh();
}

CursesRenderer::~CursesRenderer()
{
    CursesExit();
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
    for (int i = 0; i < h; ++i)
        mvwprintw(game_win->getwin(), y+i, x, std::string(w, c).c_str());
}

void
CursesRenderer::WriteStatus(const std::string& s, int line)
{
    // clear line beforehand
    wmove(status_win->getwin(), line, 0);
    wclrtoeol(status_win->getwin());

    mvwprintw(status_win->getwin(), line, 0, s.c_str());
}

void
CursesRenderer::WriteStatus(const std::string& s, int x, int y)
{
    mvwprintw(status_win->getwin(), y, x, s.c_str());
}

void
CursesRenderer::RefreshGameScreen()
{
    wrefresh(game_win->getwin());
}

void
CursesRenderer::RefreshStatusScreen()
{
    wrefresh(status_win->getwin());
}

void
CursesRenderer::DrawGameScreen()
{
    box(game_win->getwin(), 0, 0);
}

void
CursesRenderer::DrawStatusScreen()
{
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
CursesRenderer::CursesInit()
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
CursesRenderer::CursesExit()
{
    endwin();
}
