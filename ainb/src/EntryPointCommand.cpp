#include "nin-io/ainb/EntryPointCommand.h"

using namespace std;

ostream& operator<<(ostream& os, EntryPointCommand command) {
	os << command.m_name << endl;
	os << "Address: " << hex << command.m_address << endl;
	os << "Execute Index: " << hex << command.m_execution_command_index << endl;
	os << "GUID: ";
	displayCharArrayAsHex(os, command.m_guid, 16);
	return os;
}

EntryPointCommand::EntryPointCommand()
{
	m_address = -1;
	m_execution_command_index = -1;
	m_name = "";
}

EntryPointCommand::~EntryPointCommand()
{

}

void EntryPointCommand::load(fstream& file, StringList* string_list)
{

	m_address = file.tellg();

	int name_tag;
	readIntFromStream(file, is_big_endian, 4, name_tag);
	m_name = string_list->getString(name_tag);

	file.read(m_guid, 16);
	m_guid[16] = '\0';

	readIntFromStream(file, is_big_endian, 4, m_execution_command_index);

}