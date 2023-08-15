#pragma once
#include <iostream>
#include <fstream>
#include "util.h"
#include "parameters/ExternalParameter.h"

namespace ainb
{

class ExternalHandler
{
public:
	ExternalHandler();
	~ExternalHandler();

	void loadFromStream(std::fstream& stream);
	void writeToStream(std::fstream& stream);

private:
	struct HeaderEntry
	{
		int entry_count;
		int start_index;
		int allocation_offset;

		void load(std::fstream& stream);
	};

	struct Header
	{
		HeaderEntry entries[6];

		void load(std::fstream& stream);

	};

	Header m_header;
};

}