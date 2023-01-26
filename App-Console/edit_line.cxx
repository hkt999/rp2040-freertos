#include "edit_line.hpp"
#include <cstring>

void stateStandBy(EditLine *edit, int c);
void stateEscape(EditLine *edit, int c);
void stateFunc1(EditLine *edit, int c);
void stateFunc2(EditLine *edit, int c);

EditLine::EditLine(CharDriver *pDriver):mCursor(0), mLen(0)
{
    mpDriver = pDriver;
    mStateFunc = stateStandBy;
    mHistory.add("");
    memset(mWorkingBuffer, 0, sizeof(mWorkingBuffer));
}

EditLine::~EditLine()
{
}

void stateStandBy(EditLine *edit, int c)
{
    switch (c) {
        case 9: // tab
            edit->keyTab();
            break;
        case 10: // end of line, send command to parser
        case 13:
            edit->pushLine();
            break;
        case 27: // escape code to check the fucntion
            edit->mStateFunc = stateEscape;
            break;
        case 8: // backspace
        case -1: // backspace
            edit->keyBackspace();
            break;
        default:
            edit->insert(c);
            break;
    }
}

void stateEscape(EditLine *edit, int c)
{
    switch (c) {
        case 91: // [
            edit->mStateFunc = stateFunc1;
            break;
    }
}

void stateFunc1(EditLine *edit, int c)
{
    switch (c) {
        case 53: // page up
            edit->keyPageUp();
            break;
        case 54: // page down
            edit->keyPageDown();
            break;
        case 65: // up
            edit->keyUp();
            edit->mStateFunc = stateStandBy;
            break;
        case 66: // down
            edit->keyDown();
            edit->mStateFunc = stateStandBy;
            break;
        case 67: // right
            edit->keyRight();
            edit->mStateFunc = stateStandBy;
            break;
        case 68: // left
            edit->keyLeft();
            edit->mStateFunc = stateStandBy;
            break;
        case 51: // check delete
            edit->mStateFunc = stateFunc2;
            break;
    }
}

void stateFunc2(EditLine *edit, int c)
{
    switch (c) {
        case 126: // delete
            edit->keyDelete();
            edit->mStateFunc = stateStandBy;
            break;
    }
}

void EditLine::keyDelete()
{
    if (mCursor < mLen) {
        for (int i=mCursor; i<mLen-1; i++) {
            mWorkingBuffer[i] = mWorkingBuffer[i+1];
        }
        mLen--;
        for (int i=mCursor; i<mLen; i++) {
            mpDriver->putc(mWorkingBuffer[i]);
        }
        mpDriver->putc(' '); // clear the tail
        for (int i=mCursor; i<mLen+1; i++) {
            mpDriver->putc('\b');
        }
    }
}

void EditLine::keyBackspace()
{
    if (mCursor>0) {
        keyLeft();
        keyDelete();
    }
}

void EditLine::keyLeft()
{
    if (mCursor > 0) {
        mCursor--;
        mpDriver->putc('\b');
    }
}

void EditLine::keyRight()
{
    if (mCursor < mLen) {
        mpDriver->putc(mWorkingBuffer[mCursor]);
        mCursor++;
    }
}

void EditLine::clear()
{
    for (int i=0; i<mLen; i++)
        mpDriver->putc('\b');
    for (int i=0; i<mLen; i++)
        mpDriver->putc(' ');
    for (int i=0; i<mLen; i++)
        mpDriver->putc('\b');

    mLen = mCursor = strlen(mWorkingBuffer);
}

void EditLine::update(string &line)
{
    clear();
    strcpy(mWorkingBuffer, line.c_str());
    mLen = mCursor = strlen(mWorkingBuffer);
    for (int i=0; i<mLen; i++) {
        mpDriver->putc(mWorkingBuffer[i]);
    }
}

void EditLine::keyUp()
{
    string line;
    mHistory.up(line);
    if (line.length() > 0) {
        update(line);
    }
}

void EditLine::keyDown()
{
    string line;
    mHistory.down(line);
    if (line.length() > 0) {
        update(line);
    }
}

void EditLine::keyTab()
{
    /* do nothing here */
}

void EditLine::keyPageUp()
{
    /* do nothing here */
}

void EditLine::keyPageDown()
{
    /* do nothing here */
}

void EditLine::put(int c)
{
    mStateFunc(this, c);
}

void EditLine::insert(int c)
{
	if (mLen >= sizeof(mWorkingBuffer))
		return;

	for (int i = mLen; i > mCursor; i--) {
		mWorkingBuffer[i] = mWorkingBuffer[i-1];
	}

    mWorkingBuffer[mCursor++] = c;
	mLen++;

    for (int i=mCursor-1; i<mLen; i++)
        mpDriver->putc(mWorkingBuffer[i]);

    for (int i=mCursor; i<mLen; i++)
        mpDriver->putc('\b');
}

void EditLine::pushLine()
{
    mWorkingBuffer[mLen] = 0;
    if (mLen ) {
        mpDriver->putc('\n');
        processCommand();
        mHistory.update(mWorkingBuffer);
        bzero(mWorkingBuffer, sizeof(mWorkingBuffer));
        mCursor = mLen = 0;
        mHistory.add("");
    } else {
        mpDriver->putc('\n');
        processCommand();
    }
    bzero(mWorkingBuffer, sizeof(mWorkingBuffer));
}

