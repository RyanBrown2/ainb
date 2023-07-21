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

InternalParameterData GetInternalParameter(AINB* ainb, int type_index, int param_index)
{
	return convertInternalParameterData(ainb->getInternalParameter(type_index, param_index));
}

CommandParameterData GetCommandParameter(AINB* ainb, int type_index, int param_index)
{
	//CommandParameterData data{};

	//_bstr_t name;
	//BSTR udt_name; // only used by udt params

	//return data;
	return convertCommandParameterData(ainb->getCommandParameter(type_index, param_index));

}
