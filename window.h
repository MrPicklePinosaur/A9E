#ifndef __WINDOW_H__
#define __WINDOW_H__

#include <ncurses.h>

// raii wrapper around ncurses's WINDOW
class Window
{
protected:
    WINDOW* win;
public:
    Window(int w, int h, int y, int x);
    virtual ~Window();
    void render();
private:
    virtual void pre_render();
    virtual void main_render();
};

class GameWindow final : public Window
{
public:
    GameWindow(int w, int h, int y, int x);
    ~GameWindow();
    void pre_render() override;
    void main_render() override;
};


#endif // __WINDOW_H__
