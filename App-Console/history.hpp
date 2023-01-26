#pragma once

#include "ministd/string.hpp"
#include "ministd/list.hpp"

using namespace ministd;

class History
{
	protected:
		list<string> mCommands;
		list<string>::iterator mIter;

	public:
		History();
		~History();

	public:
		void update(const char *p);
		void add(const char *line);
		void up(string &line);
		void down(string &line);
};

