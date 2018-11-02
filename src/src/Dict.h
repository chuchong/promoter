#pragma once
#include "CharString.h"
class Dict
{
public:
	virtual void addCopyOf(CharString *str) = 0;
	virtual void remove(CharString *str) = 0;
	virtual bool search(CharString *str) = 0;
	//virtual void generateINI(std::wstring url) = 0;
	//virtual void getINI(std::wstring url) = 0;
	Dict();
	~Dict();
};

