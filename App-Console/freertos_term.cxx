#include <cstdio>
#include <cstdlib>
#include "stdio_edit.hpp"
#include "pico/stdlib.h"

bool m_running = false;

class PicoCharDriver : public CharDriver
{
    public:
        PicoCharDriver() {
            #ifdef DEBUG
            stdio_init_all();
            // Pause to allow the USB path to initialize
            #endif
        }
        ~PicoCharDriver() {}

    public:
        virtual void putc(int c) override;
        virtual int getc(uint32_t timeout_ms) override;
};

void PicoCharDriver::putc(int c)
{
    if (c=='\n') {
        putchar_raw('\r');
    }
    putchar_raw(c);
}

int PicoCharDriver::getc(uint32_t timeout_us)
{
    return getchar_timeout_us(timeout_us);
}

// create driver and console
PicoCharDriver picoDriver;
static StdioEditLine edit(&picoDriver, "");

void stopConsole()
{
    m_running = false;
}

void startConsole(const char *prompt, void *userData)
{
    edit.setPrompt(prompt);
    edit.setUserData(userData);
    printf("%s", prompt);
    m_running = true;
    while (m_running) {
        int c = picoDriver.getc(1000); // 1ms
        if (c>=0) {
            edit.put(c);
        }
    }
}

void registerCommand(const char *cmd, callback_t func)
{
    edit.registerCommand(cmd, func);
}
