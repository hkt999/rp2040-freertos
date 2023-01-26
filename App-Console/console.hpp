#pragma once

typedef void (*callback_t)(int argc, char **argv, void *userData);

void startConsole(const char *prompt, void *userData);
void stopConsole();
void registerCommand(const char *cmd, callback_t func);