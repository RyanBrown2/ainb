#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <comdef.h>
#include <atlbase.h>
#include "nin-io/ainb/ainb.h"

//#ifdef __cplusplus
//	extern "C" {
//#endif

class AINB_FILE
{
public:
	AINB_FILE();
	~AINB_FILE();

	void loadFile(std::string file_dir);

	BSTR getName();

private:
	AINB* m_ainb;
	BSTR m_name;
};


extern "C" __declspec(dllexport) void* CreateAINB()
{
	return new AINB_FILE();
}

extern "C" __declspec(dllexport) void* LoadFile(AINB_FILE* ainb_file, char* file_dir)
{
	ainb_file->loadFile(file_dir);
	return ainb_file;
}

extern "C" __declspec(dllexport) BSTR GetName(AINB_FILE* ainb_file)
{
	return ainb_file->getName();
}

extern "C" __declspec(dllexport) int TestFunc(char* text)
{
	std::cout << "TestFunction" << std::endl;
	std::cout << text << std::endl;
	return 0;
}


//extern "C" __declspec(dllexport) int Adding(AINB_FILE * ainb_file, int y) {
//	return ainb_file->adding(y);
//}
