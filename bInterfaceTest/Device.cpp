#include "stdafx.h"
#include "Device.h"


Device::Device(CString _id, CString _rid):Id(_id),RId(_rid)
{
	pSigAIMap = NULL;
	pSigAlarmMap = NULL;
	pSigAOMap = NULL;
	pSigDIMap = NULL;
	pSigDOMap = NULL;
}


Device::~Device()
{
}
