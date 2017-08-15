#pragma once

#include "ChiliWin.h"

class COMInitializer
{
public:
	COMInitializer();
	~COMInitializer();
private:
	static size_t refCount;
};