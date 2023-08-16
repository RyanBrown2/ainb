#pragma once

#include <vector>

#include "ExternalHandler.h"
#include "ParameterHandler.h"
#include "SequenceHandler.h"
#include "SequenceNode.h"

namespace ainb
{

class AddressManager
{
public:
	struct AddressData
	{
		int headers_end_address = -1; // 0x20 in header
		int string_list_start_address = -1; // 0x24 in header
		// 4 bytes before string list start address
		int string_list_4byte_padding = -1; // 0x28 in header - will probably be renamed
		int internal_param_start_address = -1; // 0x2c in header
		int param_unknown_section_1_start_address = -1; // 0x30 in header - will be renamed
		int command_param_start_address = -1; // 0x34 in header
		int command_param_end_address = -1; // 0x38 in header - not sure if should be renamed
		// this section comes before anything else in the param section
		int param_unknown_section_2_start_address = -1; // 0x3c in header - will be renamed
		// beginning of overall param section
		int param_section_start_address = -1; // 0x40 in header
		int func_start_address = -1; // 0x44 in header - not sure if should be renamed
		// this is 4 bytes before the "ff ff ff ff" near the end of the file (comes before string list)
		int unknown_1_address = -1; // 0x48 in header
		int param_unknown_section_3_start_address = -1; // 0x4c in header - will be renamed
		int param_unknown_section_4_start_address = -1; // 0x50 in header - will be renamed
		// 0x54 is null in header
		// 0x58 is null in header
		int func_end_address = -1; // 0x5c in header - not sure if should be renamed
		// 0x60 is non-address data in header
		// 0x64 is non-address data in header
		// there is a 4-byte piece of data near the end of the file
		int end_data_4byte_padding = -1; // 0x68 in header - will be renamed
		int string_list_8byte_padding = -1; // 0x6c in header - will be renamed
		// address of the 4-byte piece of data near the end of the file
		int end_data_address = -1; // 0x70 in header - will be renamed

		// not in header
		int node_bodies_start_address;
	};

	AddressManager(ParameterHandler* parameter_handler, SequenceHandler* sequence_handler, ExternalHandler* external_handler);
	~AddressManager();

	AddressData* calcAddressData();

private:
	ParameterHandler* m_parameter_handler = nullptr;
	SequenceHandler* m_sequence_handler = nullptr;
	ExternalHandler* m_external_handler = nullptr;

	AddressData* m_address_data = nullptr;

	int calcNodeBodiesSize();

	int calcInternalParamSize();
	int calcCommandParamSize();

};

}