#include <cstring>
#include <cstdlib>
#include <cstdio>
#include "history.hpp"

History::History()
{
}

History::~History()
{
}

void History::add(const char *p)
{
	string s = p;
	mCommands.push_front(s);
	mIter = mCommands.begin();
}

void History::up(string &s)
{
	++mIter;
	if (mIter != mCommands.end()) {
		s = *mIter;
	}
}

void History::down(string &s)
{
	if (mIter == mCommands.begin()) {
		return;
	}
	--mIter;
	s = *mIter;
}

void History::update(const char *p)
{
	mIter = mCommands.begin();
	if (mIter != mCommands.end())
		*mIter = p;
}