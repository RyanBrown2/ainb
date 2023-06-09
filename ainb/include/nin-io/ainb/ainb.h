#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "nin-io/util/util.h"
#include "BaseCommand.h"
#include "A_Command.h"
#include "B_Command.h"
#include "StringList.h"
#include "ParameterNode.h"
#include "Parameters.h"

class AINB
{
public:
	AINB();
	~AINB();
	void load(std::fstream& file);

	struct AinbHeaderData {
		char type[4]; // 0x00 | 3
		int a_commands; // 0x0C | 4
		int b_commands; // 0x10 | 4
		int data_body_start; // 0x20 | 4
		int string_section_start; // 0x24 | 4

		int footer_start; // 0x2c | 4
	};

	std::string getName();
	AinbHeaderData getHeaderData();

	std::vector<A_Command> getACommands();
	std::vector<B_Command> getBCommands();

	
private:
	static const bool is_big_endian = false;
	void loadFooter(std::fstream& file);
	void loadDataBody(std::fstream& file);

	std::string m_name;
	AinbHeaderData m_header_data;

	std::vector<A_Command> m_a_commands;
	std::vector<B_Command> m_b_commands;	

};