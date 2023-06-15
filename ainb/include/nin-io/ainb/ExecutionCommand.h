#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "StringList.h"
#include "nin-io/util/util.h"

class ExecutionCommand
{
public:
	ExecutionCommand();
	~ExecutionCommand();
	void load(std::fstream& file, StringList* string_list);

	int getIndex() { return m_index; }
	std::string getName() { return m_name; }
	int getAddress() { return m_address; }
	int getBodyAddress() { return m_body_pointer; }

	int getCommandID() { return m_command_id; }
	char* getGuid() { return m_guid; }

	int getUnknown1() { return m_unknown1; }
	int getUnknown2() { return m_unknown2; }

	friend std::ostream& operator<<(std::ostream& os, const ExecutionCommand block);

private:

	std::string m_name;
	int m_address;

	char* m_guid;

	int m_index; // 0x02
	int m_body_pointer; // 0x14
	int m_command_id; // 0x0c

	// unknown values
	int m_unknown1; // 0x00
	int m_unknown2; // 0x06

	static const bool is_big_endian = false;

};