#ifndef __INPUTER_H__
#define __INPUTER_H__

#include <memory>
#include <thread>
#include <bitset>
#include <iostream>

class Inputer
{
public:
    virtual ~Inputer() {}
    virtual bool GetKeyDown(char c) = 0;
    virtual void ClearKeyMap() = 0;
};

class CursesInputer final : public Inputer
{
    std::unique_ptr<std::thread> listen_thread;
    std::bitset<256> key_map;
public:
    CursesInputer();
    ~CursesInputer();
    bool GetKeyDown(char c) override;
    void ClearKeyMap() override;
private:
    void ListenInputer();
    void SetKeyDown(char c);
};

CursesInputer::CursesInputer():
    Inputer{}, listen_thread{std::make_unique<std::thread>(&CursesInputer::ListenInputer, this)} {}

CursesInputer::~CursesInputer() {}

void
CursesInputer::ListenInputer()
{
    while (true) {

        // not using the builtin curses getch since that doesn't have multithread support
        int c = getchar();
        if (c < 0) continue;

        SetKeyDown(c);
    }
}

bool
CursesInputer::GetKeyDown(char c)
{
    return key_map.test(static_cast<size_t>(c));
}

void
CursesInputer::ClearKeyMap()
{
    key_map.reset();
}

void
CursesInputer::SetKeyDown(char c)
{
    key_map.set(static_cast<size_t>(c));
}

#endif // __INPUTER_H__
