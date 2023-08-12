#include "ainb/ExternalHandler.h"

using namespace std;
using namespace ainb;

ExternalHandler::ExternalHandler()
{

}

ExternalHandler::~ExternalHandler()
{

}

void ExternalHandler::loadFromStream(fstream& stream)
{
	m_header.load(stream);
}

void ExternalHandler::writeToStream(fstream& stream)
{

}

void ExternalHandler::HeaderEntry::load(fstream& stream)
{
	readIntFromStream(stream, 2, entry_count);
	readIntFromStream(stream, 2, start_index);
	readIntFromStream(stream, 4, allocation_offset);
}

void ExternalHandler::Header::load(fstream& stream)
{
	for (int i = 0; i < 6; i++)
	{
		entries[i].load(stream);
	}
}