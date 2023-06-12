#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <algorithm>
#include "nin-io/util/util.h"
#include "nin-io/ainb/ainb.h"
#include "yaml-cpp/yaml.h"
#include <cassert>

using namespace std;


int main(int argc, char* argv[])
{
	//const char* fileDir = "";
	//const char* fileDir = "BeforeInitializeOpeningField.module.ainb";
	//const char* fileDir = "KorokCarry_EventStarter.event.root.ainb";
	//const char* fileDir = "Pouch.module.ainb";
	//const char* fileDir = "B-7_cdd7.logic.root.ainb";
	//const char* fileDir = "FastLoadOff.module.ainb";
	//const char* fileDir = "LargeDungeonWater_AllinArea_895e.logic.module.ainb";
	//const char* fileDir = "Set_Defense_Karakara_ee67.logic.module.ainb";
	const char* fileDir = "CustomHouseControlActor.event.root.ainb";
	//const char* fileDir = "Npc_Ganondorf_Human.event.root.ainb";

	fstream file;
	file.open(fileDir, fstream::in | fstream::out | std::ios::binary);

	if (file.fail()) {
		std::cout << "Failed to open file" << std::endl;
		return 1;
	}

	AINB ainb;

	ainb.load(file);

	file.close();

	YAML::Emitter out;
	out << YAML::BeginMap;

	// START OF HEADER DATA

	AINB::FileHeaderData file_header_data = ainb.getFileHeaderData();
	out << YAML::Key << "type";
	out << YAML::Value << file_header_data.type;

	out << YAML::Key << "entry_point_command_count";
	out << YAML::Value << file_header_data.entry_point_command_count;

	out << YAML::Key << "execution_command_count";
	out << YAML::Value << file_header_data.execution_command_count;

	// users probably won't need this data, just for debugging
	out << YAML::Key << "command_body_start_address";
	out << YAML::Value << file_header_data.command_body_start_address;
	out << YAML::Comment("users probably won't need this data, just for debugging");

	out << YAML::Key << "string_list_start_address";
	out << YAML::Value << file_header_data.string_list_start_address;
	out << YAML::Comment("users probably won't need this data, just for debugging");

	out << YAML::Key << "parameter_table_start_address";
	out << YAML::Value << file_header_data.parameter_table_start_address;
	out << YAML::Comment("users probably won't need this data, just for debugging");

	out << YAML::Key << "parameter_structure_start_address";
	out << YAML::Value << file_header_data.parameter_structure_start_address;
	out << YAML::Comment("users probably won't need this data, just for debugging");
	// END OF HEADER DATA

	if (file_header_data.entry_point_command_count == 0) {
		out << YAML::EndMap;
		assert(out.good());
		cout << out.c_str() << endl;
		return 0;
	}

	// SEQUENCE DATA
	out << YAML::Key << "sequence_data";
	out << YAML::Value << YAML::BeginSeq;
	
	vector<SequenceHandler::SequenceNode*>* sequences = ainb.getSequences();

	SequenceHandler::SequenceNode* node = sequences->at(0);
	out << YAML::BeginMap;
	out << YAML::Key << "sequence_name";
	out << YAML::Value << node->node_command->getName();

	out << YAML::EndMap;

	//for (int i = 0; i < sequences.size(); i++) {
	//	SequenceHandler::SequenceNode* node = sequences[i];
	//	out << YAML::BeginMap;
	//	


	//	out << YAML::EndMap;
	//}


	out << YAML::EndSeq;

	out << YAML::EndMap;

	assert(out.good());
	cout << out.c_str() << endl;


	return 0;
}