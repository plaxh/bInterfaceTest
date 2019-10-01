#include "stdafx.h"
#include "SU.h"


SU::SU()
{
	SUId = _T("");
	UserName = _T("");
	PassWord = _T("");
	SURId = _T("");
	SUIP = _T("");
	SUPort = 0;
	SUVendor = _T("");
	SUModel = _T("");
	SUHardVer = _T("");
	SUConfigTime = _T("");
	SUVer = 0.0;
	pDeviceMap = NULL;
}


SU::~SU()
{
	
	if (pDeviceMap)
	{
		for (auto devicePair : *pDeviceMap)
		{
			 delete devicePair.second;
		}
		delete pDeviceMap;
	}
		
}
