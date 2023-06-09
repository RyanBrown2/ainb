#include "nin-io/ainb/A_Command.h"

using namespace std;

ostream& operator<<(ostream& os, const A_Command command) {
	os << command.m_name << endl;
	os << "Address: " << command.m_address << endl;
	os << "Unknown 2: " << hex << command.m_unknown2 << endl;
	os << "GUID: ";
	displayCharArrayAsHex(os, command.m_guid, 16);
	return os;
}

A_Command::A_Command() : BaseCommand(BlockType::A)
{
	m_unknown2 = -1;
}

A_Command::~A_Command()
{

}

void A_Command::load(fstream& file)
{
	BaseCommand::load(file);

	// unknown 1
	readIntFromStream(file, is_big_endian, 4, m_string_pointer);

	// data
	file.read(m_guid, 16);

	// unknown 2
	readIntFromStream(file, is_big_endian, 4, m_unknown2);
}
