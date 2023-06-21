#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <comdef.h>
#include <atlbase.h>
#include <unordered_map>
#include "nin-io/ainb/ainb.h"


class AINB_FILE
{
public:
	AINB_FILE();
	~AINB_FILE();


	void loadFromFile(char* dir);
private:

	AINB* ainb;

};

extern "C" __declspec(dllexport) void* CreateAINB()
{
	return new AINB_FILE();
}

extern "C" __declspec(dllexport) void DestroyAINB(AINB_FILE* ainb)
{
	delete ainb;
}

extern "C" __declspec(dllexport) void LoadFromFileAINB(AINB_FILE * ainb, char* dir)
{
	ainb->loadFromFile(dir);
}
