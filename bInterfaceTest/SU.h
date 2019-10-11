#pragma once

#include "Device.h"
#include <map>

class SU
{
public:
	SU();
	~SU();
	CString UserName;
	CString PassWord;
	CString SUId;
	CString SURId;
	CString SUIP;
	int SUPort;
	CString SUVendor;
	CString SUModel;
	CString SUHardVer;
	CString SUConfigTime;
	double SUVer;
	std::map <CString, Device*>* pDeviceMap;
};

