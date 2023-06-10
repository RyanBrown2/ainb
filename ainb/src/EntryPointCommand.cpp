#include "nin-io/ainb/EntryPointCommand.h"

using namespace std;

ostream& operator<<(ostream& os, const EntryPointCommand command) {
	os << command.m_name << endl;
	os << "Address: " << hex << command.m_address << endl;
	os << "Execute Index: " << hex << command.m_unknown2 << endl;
	os << "GUID: ";
	displayCharArrayAsHex(os, command.m_guid, 16);
	return os;
}

EntryPointCommand::EntryPointCommand() : BaseCommand(BlockType::A)
{
	m_unknown2 = -1;
}

EntryPointCommand::~EntryPointCommand()
{

}

void EntryPointCommand::load(fstream& file)
{
	BaseCommand::load(file);

	// unknown 1
	readIntFromStream(file, is_big_endian, 4, m_string_pointer);

	// data
	file.read(m_guid, 16);

	// unknown 2
	readIntFromStream(file, is_big_endian, 4, m_unknown2);
}
