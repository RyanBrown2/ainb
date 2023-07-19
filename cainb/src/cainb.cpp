#include "cainb.h"

using namespace std;
using namespace ainb;

AINB* Create(char* dir)
{
	fstream file;
	file.open(dir, fstream::in | ios::binary);

	if (file.fail())
	{
		return nullptr;
	}

	AINB* ainb = new ainb::AINB(file);
	file.close();
	return ainb;
}

int GetEntryCommandCount(AINB* ainb)
{
	return ainb->getEntryCommandCount();
}

int GetExecutionCommandCount(AINB* ainb)
{
	return ainb->getExecutionCommandCount();
}

int* GetInternalParameterCounts(AINB* ainb)
{
	return ainb->getInternalParameterCounts();
}

int* GetCommandParameterCounts(AINB* ainb)
{
	return ainb->getCommandParameterCounts();
}
