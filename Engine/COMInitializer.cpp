#include "COMInitializer.h"
#include <objbase.h>

size_t COMInitializer::refCount = 0u;

COMInitializer::COMInitializer()
{
	if( refCount == 0u )
	{
		CoInitializeEx( NULL,COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE );
		refCount++;
	}
}

COMInitializer::~COMInitializer()
{
	if( --refCount == 0u )
	{
		CoUninitialize();
	}
}
