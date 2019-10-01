#pragma once
#include <map>
#include "signalAI.h"
#include "signalAlarm.h"
#include "signalAO.h"
#include "signalDI.h"
#include "signalDO.h"

class Device
{
public:
	Device(CString id,CString rid);
	~Device();
	CString Id;
	CString RId;
	std::map <CString, signalAI*> * pSigAIMap;
	std::map <CString, signalAlarm*> * pSigAlarmMap;
	std::map <CString, signalAO*> * pSigAOMap;
	std::map <CString, signalDI*> * pSigDIMap;
	std::map <CString, signalDO*> * pSigDOMap;

};

