
#ifndef __RENDERER_H__
#define __RENDERER_H__

class CursesRenderer
{
public:
    CursesRenderer();
    ~CursesRenderer();
private:
    void curses_init();
    void curses_exit();
};

#endif // __RENDERER_H__
