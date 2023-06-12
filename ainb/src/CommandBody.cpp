#include "nin-io/ainb/CommandBody.h"

using namespace std;


CommandBody::CommandBody()
{
	m_address = -1;
}

CommandBody::CommandBody(StringList* string_list)
{
	m_string_list = string_list;
	m_address = -1;

}

CommandBody::~CommandBody()
{

}

void CommandBody::load(fstream& file)
{
	m_address = file.tellg();
	for (int i = 0; i < 6; i++) {
		int param_num;
		readIntFromStream(file, is_big_endian, 4, param_num);
		m_table_parameters[i] = param_num;

		int param_value;
		readIntFromStream(file, is_big_endian, 4, param_value);
		m_table_values[i] = param_value;
	}

	for (int i = 0; i < 12; i++) {
		int param_num;
		readIntFromStream(file, is_big_endian, 4, param_num);
		m_structure_parameters[i] = param_num;

		int param_value;
		readIntFromStream(file, is_big_endian, 4, param_value);
		m_structure_values[i] = param_value;
	}

	// TODO: load table
}

//void CommandBody::setCommandReference(ExecutionCommand* command)
//{
//	m_command = command;
//}

