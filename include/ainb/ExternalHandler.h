#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <variant>
#include <memory>
#include <map>

#include "StringList.h"
#include "util.h"
#include "parameters/ParameterBase.h"
#include "parameters/ExternalParameter.h"

namespace ainb
{

class ExternalHandler
{
public:
	ExternalHandler(StringList* string_list);
	~ExternalHandler();

	void loadFromStream(std::fstream& stream);
	void writeToStream(std::fstream& stream);

	// temp just so the address manager can function
	int getSize();

private:

	struct HeaderEntry
	{
		int entry_count = -1;
		int start_index = -1;
		int allocation_offset = -1;
	};

	StringList* m_string_list;

	std::map<int, std::vector<std::unique_ptr<ExternalParameterBase>>> m_parameters;

	void createAndLoadParameter(std::fstream& stream, int index, ParameterType type);

	int m_external_file_count = 0;
};
}