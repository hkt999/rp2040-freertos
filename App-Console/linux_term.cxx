#include <termios.h>
#include <cstdio>
#include <cstdlib>
#include "stdio_edit.hpp"

struct termios save;
bool m_running = false;

static StdioEditLine edit("");
int on_exit(void (*function)(int , void *), void *arg);

void exit_callback() 
{
    tcsetattr( fileno(stdin), TCSANOW, &save);
}

void stopConsole()
{
    m_running = false;
}

void startConsole(const char *prompt, void *userData)
{
    struct termios term;
    tcgetattr( fileno(stdin), &term );
    save = term;

    term.c_lflag &= ~ICANON;
    term.c_lflag &= ~ECHO;
    tcsetattr( fileno(stdin), TCSANOW, &term);

    atexit(exit_callback);
    edit.setPrompt(prompt);
    edit.setUserData(userData);
    printf("%s", prompt);
    m_running = true;
    while (m_running) {
        char c = fgetc(stdin);
        edit.putChar(c);
    }
    tcsetattr( fileno(stdin), TCSANOW, &save);
}

void registerCommand(const char *cmd, callback_t func)
{
    edit.registerCommand(cmd, func);
}
