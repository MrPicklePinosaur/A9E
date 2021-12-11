#ifndef __A9E_RENDERER_H__
#define __A9E_RENDERER_H__

#include <memory>
#include <string>

#include "window.h"

class Renderer
{
public:
    Renderer() {}
    virtual ~Renderer() {}
    virtual void DrawChar(char c, int x, int y) = 0;
    virtual void DrawBox(char c, int x, int y, int w, int h) = 0;
    virtual void WriteStatus(const std::string& s, int line) = 0;
    virtual void WriteStatus(const std::string& s, int x, int y) = 0;
    virtual void RefreshGameScreen() = 0;
    virtual void RefreshStatusScreen() = 0;
    virtual void DrawGameScreen() = 0;
    virtual void DrawStatusScreen() = 0;
    virtual void ClearGameScreen() = 0;
    virtual void ClearStatusScreen() = 0;
};

class CursesRenderer final : public Renderer
{
    std::unique_ptr<Window> game_win;
    std::unique_ptr<Window> status_win;
public:
    CursesRenderer();
    ~CursesRenderer();
    void DrawChar(char c, int x, int y) override;
    void DrawBox(char c, int x, int y, int w, int h) override;
    void WriteStatus(const std::string& s, int line) override;
    void WriteStatus(const std::string& s, int x, int y) override;
    void RefreshGameScreen() override;
    void RefreshStatusScreen() override;
    void DrawGameScreen() override;
    void DrawStatusScreen() override;
    void ClearGameScreen() override;
    void ClearStatusScreen() override;
private:
    void CursesInit();
    void CursesExit();
};

#endif // __A9E_RENDERER_H__
