#include "stdafx.h"
#include "Command.h"



Command::Command(int _cmdCode):code(_cmdCode)
{
	
}

std::string Command::genCMD()
{
	return "";
}


Command::~Command()
{
}
