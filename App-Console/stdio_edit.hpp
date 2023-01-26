#pragma once

#include <unistd.h>
#include "console.hpp"
#include "edit_line.hpp"
#include "ministd/map.hpp"
#include "ministd/string.hpp"

using namespace ministd;

class StdioEditLine : public EditLine {
    protected:
        string m_prompt;
        map<string, callback_t> m_commands;
        void *m_userData;

    public:
        StdioEditLine(CharDriver *pDriver, const char *prompt, void *userData = nullptr);
        ~StdioEditLine();

    public:
        void output(int c);

        void processCommand();
        void setPrompt(const char *prompt);
		void setUserData(void *userData);

        void registerCommand(const char *cmd, callback_t func);
        void unregisterCommand(const char *cmd);
};

