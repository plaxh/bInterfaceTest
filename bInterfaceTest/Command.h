#pragma once
#include "SU.h"
#include <set>

class Command
{
public:
	Command(int);
	~Command();
	int code;
	std::map < CString, std::set<CString>> cmdSignal;
	std::string genCMD();
};

