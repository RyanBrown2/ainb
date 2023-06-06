#pragma once
#include <iostream>
#include <fstream>
#include "nin-io/util/util.h"
#include "DataBlock.h"
#include "A_Block.h"
#include "B_Block.h"
#include "StringList.h"

class AINB
{
public:
	AINB();
	~AINB();
	void load(std::fstream& file);
private:
	bool isBigEndian;
	void loadFooter(std::fstream& file);
	void loadDataBody(std::fstream& file);

	struct FileHeaderData {
		char type[4]; // 0x00 | 3
		int a_blocks; // 0x0C | 4
		int b_blocks; // 0x10 | 4
		int data_body_start; // 0x20 | 4
		int string_section_start; // 0x24 | 4

		int t_footer_start; // 0x2c | 4
		// TODO data footer stuff
	};

	FileHeaderData header_data;
	struct DataFootAddresses {
		int table_section_start;
		int structure_section_start;
	};

	DataFootAddresses data_foot_addresses;
	void loadDataFootAddresses(std::fstream& file);
};