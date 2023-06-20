#pragma once
#include <iostream>
#include <fstream>
#include "nin-io/util/util.h"
#include "StringList.h"

class EntryPointCommand
{
public:
	EntryPointCommand();
	~EntryPointCommand();
	void load(std::fstream& file, StringList* string_list);

	std::string getName() { return m_name; }
	int getExecutionIndex() { return m_execution_command_index; }
	char* getGUID() { return m_guid; }

	friend std::ostream& operator<<(std::ostream& os, const EntryPointCommand block);

private:
	int m_address;
	int m_execution_command_index;

	char m_guid[17];

	std::string m_name;
	static const bool is_big_endian = false;
};
