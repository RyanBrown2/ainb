#include "nin-io/ainb/BaseCommand.h"

using namespace std;

BaseCommand::BaseCommand(BlockType type)
{
	//DataBlock::m_address = address;
	m_address = -1;
	m_data_dump = new char[17];
	//m_data_dump = new char[17];

	m_blockType = type;
	m_string_pointer = -1;
}

BaseCommand::~BaseCommand()
{
	//delete[] data;
}

void BaseCommand::load(fstream& file)
{
	m_address = file.tellg();
}
