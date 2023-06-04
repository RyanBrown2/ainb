#pragma once
#include <iostream>
#include <fstream>
#include "DataBlock.h"
#include "nin-io/util/util.h"

class B_Block : public DataBlock
{
public:
	B_Block(int address);
	~B_Block();
	static B_Block load(std::fstream& file);

	// temp public for testing
	int m_index; // 0x02
	int m_storedAddress; // 0x14
	char *m_dataChunk; // 0x0c
	// unknown values
	int m_unknown1; // 0x00
	int m_unknown2; // 0x06
	int m_unknown3; // 0x08
private:


};
