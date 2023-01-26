#pragma once
#include <stdint.h>
#include "history.hpp"

#define BUFLEN  80
enum {
    STATE_STANDBY = 0,
    STATE_ESCAPE,
    STATE_FUNCTION1,
    STATE_FUNCTION2,
    NUM_STATE
};

class CharDriver
{
    public:
        virtual ~CharDriver() {}
        virtual int getc(uint32_t timeout_us) = 0;
        virtual void putc(int c) = 0;
};

class EditLine
{
    protected:
        CharDriver *mpDriver;
        History mHistory;
        int mCursor;
		int mLen;
        char mWorkingBuffer[BUFLEN];
        void (*mStateFunc)(EditLine *obj, int c);

        void clear();
        void update(string &line);

    public:
        EditLine(CharDriver *pDriver);
        virtual ~EditLine();

        void keyLeft();
        void keyRight();
        void keyUp();
        void keyDown();
        void keyDelete();
        void keyBackspace();
        void keyTab();
        void keyPageUp();
        void keyPageDown();
        void put(int c);

        friend void stateStandBy(EditLine *edit, int c);
        friend void stateEscape(EditLine *edit, int c);
        friend void stateFunc1(EditLine *edit, int c);
        friend void stateFunc2(EditLine *edit, int c);

        void insert(int c);
        void pushLine();

    public: // virtual interface
        virtual void processCommand() = 0;
};

