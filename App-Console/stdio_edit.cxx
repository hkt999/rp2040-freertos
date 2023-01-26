#include "stdio_edit.hpp"
#include "console.hpp"
#include <cstdio>
#include <cstring>
#include <cctype>

#define MAX_ARGV	16

StdioEditLine::StdioEditLine(CharDriver *pDriver, const char *prompt, void *userData):EditLine(pDriver)
{
	m_prompt = prompt;
	m_userData = userData;
	if (prompt==NULL) {
		m_prompt = "$ksh>";
	}
}

StdioEditLine::~StdioEditLine()
{
}

static void parseInputCommand(char *cmd, int &argc, char **argv, int maxArgv)
{
	enum {
		STATE_NEW_TOKEN = 0,
		STATE_PROCESSING
	};
	memset(argv, 0, sizeof(const char *) * maxArgv);
	argc = 0;

	int state = STATE_NEW_TOKEN;
	char *p = cmd;
	while (*p != 0) {
		switch (state) {
			case STATE_NEW_TOKEN:
				if ( isalnum(*p) || isalpha(*p) || (*p=='_') || (*p=='/') || (*p=='-') || (*p=='.') ) {
					argv[argc++] = p;
					state = STATE_PROCESSING;
				} 
				break;

			case STATE_PROCESSING:
				if (isalnum(*p) || (*p=='_') || (*p=='/') || (*p=='-') || (*p=='.')) {
					break; // keep the current state
				} else {
					*p = (char)0; // terminate
					state = STATE_NEW_TOKEN;
				}
				break;
		}
		p++;
	}
}

void StdioEditLine::processCommand()
{
	char *argv[MAX_ARGV];
	int argc;

	if (strlen(mWorkingBuffer) > 0) {
		char *buf = strdup(mWorkingBuffer);
		parseInputCommand(buf, argc, argv, MAX_ARGV);
		if (argc > 0) {
			auto it = m_commands.find(argv[0]);
			if (it != m_commands.end()) {
				it->second(argc, argv, m_userData);
			} else {
				printf("unknown command: %s\n", argv[0]);
			}
		}
		free(buf);
	}
	printf("%s", m_prompt.c_str());
}

void StdioEditLine::output(int c)
{
    printf("%c", c);
}

void StdioEditLine::setPrompt(const char *prompt)
{
	m_prompt = prompt;
}

void StdioEditLine::setUserData(void *userData)
{
	m_userData = userData;
}

void StdioEditLine::registerCommand(const char *cmd, callback_t func)
{
	m_commands.insert({cmd, func});
}

void StdioEditLine::unregisterCommand(const char *cmd)
{
	m_commands.erase(cmd);
}
