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



YAML::Emitter& operator << (YAML::Emitter& out, SequenceHandler::SequenceNode* node) {
	//out << YAML::Flow;
	out << YAML::BeginMap;
	
	out << YAML::Key << "command_name";
	out << YAML::Value << node->node_command->getName();
	out << YAML::Key << "index";
	out << YAML::Value << node->node_command->getIndex();
	out << YAML::Key << "command_id";
	out << YAML::Value << node->node_command->getCommandID();
	out << YAML::Key << "guid";
	out << YAML::Value << rawToHex(node->node_command->getGuid(), 16);
	out << YAML::Key << "unknown_1";
	out << YAML::Value << node->node_command->getUnknown1();
	out << YAML::Key << "unknown_2";
	out << YAML::Value << node->node_command->getUnknown2();

	// Parameters

	out << YAML::Key << "parameters";

	out << YAML::Value << YAML::BeginMap;

	out << YAML::Key << "table_params";
	out << YAML::Flow;
	vector<CommandBody::Parameter>* table_params = node->node_body->getTableParameters();
	out << YAML::Value << YAML::BeginSeq;
	for (int i = 0; i < table_params->size(); i++) {
		out << YAML::BeginMap;
		out << YAML::Key << "table_num";
		out << YAML::Value << table_params->at(i).section_num;
		out << YAML::Key << "index";
		out << YAML::Value << table_params->at(i).index;
		out << YAML::Key << "param_name";
		out << YAML::Value << table_params->at(i).param_name;
		out << YAML::Key << "value";
		out << YAML::Value << table_params->at(i).value;
		out << YAML::EndMap;
	}

	out << YAML::EndSeq;
	out << YAML::Key << "structure_params";
	out << YAML::Flow;
	vector<CommandBody::Parameter>* structure_params = node->node_body->getStructureParameters();
	out << YAML::Value << YAML::BeginSeq;
	for (int i = 0; i < structure_params->size(); i++) {
		out << YAML::BeginMap;
		out << YAML::Key << "section_num";
		out << YAML::Value << structure_params->at(i).section_num;
		out << YAML::Key << "index";
		out << YAML::Value << structure_params->at(i).index;
		out << YAML::Key << "param_name";
		out << YAML::Value << structure_params->at(i).param_name;
		out << YAML::Key << "value";
		out << YAML::Value << structure_params->at(i).value;
		out << YAML::EndMap;
	}

	out << YAML::EndSeq;
	out << YAML::EndMap;

	// Call Table

	out << YAML::Key << "call_table";
	out << YAML::Flow;
	out << YAML::Value << YAML::BeginMap;

	vector<int>* ordered_parameters = &node->ordered_param_indices;
	map<int, vector<string>>* sorted_call_table = &node->sorted_call_table;
	for (int i = 0; i < ordered_parameters->size(); i++) {
		int index = ordered_parameters->at(i);
		vector<string> params = sorted_call_table->at(index);
		out << YAML::Key << index;
		out << YAML::Value << params;
	}

	out << YAML::EndMap;
	
	vector<SequenceHandler::SequenceNode*>* callees = &node->callees;

	out << YAML::Key << "callees";
	out << YAML::Value << YAML::BeginSeq;
	for (int i = 0; i < callees->size(); i++) {
		out << callees->at(i);
	}
	out << YAML::EndSeq;

	out << YAML::EndMap;
	return out;
}

YAML::Emitter& operator << (YAML::Emitter& out, ParameterHandler::TableParameter param)
{
	out << YAML::BeginMap;
	out << YAML::Key << "name";
	out << YAML::Value << param.name;
	out << YAML::Key << "address";
	out << YAML::Value << param.address;
	out << YAML::Key << "value";
	out << YAML::Value << param.value;

	out << YAML::EndMap;
	return out;
}

YAML::Emitter& operator << (YAML::Emitter& out, ParameterHandler::StructureParameter param)
{
	out << YAML::BeginMap;
	out << YAML::Key << "name";
	out << YAML::Value << param.name;
	out << YAML::Key << "address";
	out << YAML::Value << param.address;
	if (param.section_num == 10) {
		out << YAML::Key << "second_string";
		out << YAML::Key << param.second_string;
	} else {
		out << YAML::Key << "tag";
		out << YAML::Value << param.tag;
	}

	out << YAML::EndMap;
	return out;
}

int main(int argc, char* argv[])
{

	if (argc < 2) {
		std::cout << "Usage: ainb-parse <file>" << std::endl;
		return 1;
	}

	const char* fileDir = argv[1];
	cout << "Opening file: " << fileDir << endl;

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
	/*
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
	*/
	// END OF HEADER DATA

	// PARAMETER DATA
	out << YAML::Key << "parameters";

	out << YAML::Value << YAML::BeginMap;
	ParameterHandler* parameter_handler = ainb.getParameterHandler();
	out << YAML::Key << "active_tables";
	out << YAML::Flow;
	out << YAML::Value << *parameter_handler->getActiveTables();
	out << YAML::Key << "active_structures";
	out << YAML::Flow;
	out << YAML::Value << *parameter_handler->getActiveStructures();


	out << YAML::Key << "parameter_tables";
	out << YAML::Value << YAML::BeginSeq;
	vector<int>* active_tables = parameter_handler->getActiveTables();
	for (int i = 0; i < active_tables->size(); i++) {
		out << YAML::BeginMap;
		int table_index = active_tables->at(i);
		out << YAML::Key << "tabel_index";
		out << YAML::Value << table_index;

		vector<ParameterHandler::TableParameter>* table_parameters = parameter_handler->getTableParameters(table_index);
		out << YAML::Key << "length";
		out << YAML::Value << table_parameters->size();
		out << YAML::Key << "parameters";
		out << YAML::Value << YAML::BeginSeq;

		for (int i = 0; i < table_parameters->size(); i++) {
			out << table_parameters->at(i);
		}
		out << YAML::EndSeq;
		out << YAML::EndMap;
	}


	out << YAML::EndSeq;

	out << YAML::Key << "parameter_structures";
	out << YAML::Value << YAML::BeginSeq;
	vector<int>* active_structures = parameter_handler->getActiveStructures();
	for (int i = 0; i < active_structures->size(); i++) {
		out << YAML::BeginMap;
		int structure_index = active_structures->at(i);
		out << YAML::Key << "structure_index";
		out << YAML::Value << structure_index;

		vector<ParameterHandler::StructureParameter>* structure_parameters = parameter_handler->getStructureParameters(structure_index);
		out << YAML::Key << "length";
		out << YAML::Value << structure_parameters->size();
		out << YAML::Key << "parameters";
		out << YAML::Value << YAML::BeginSeq;
		for (int i = 0; i < structure_parameters->size(); i++) {
			out << structure_parameters->at(i);
		}
		out << YAML::EndSeq;
		out << YAML::EndMap;
	}

	out << YAML::EndSeq;

	out << YAML::EndMap;

	// END OF PARAMETER DATA

	// SEQUENCE DATA

	if (file_header_data.entry_point_command_count == 0) {
		cout << "No entry point commands found, most likely a logic file | ";
		cout << "Manually parsing sequences" << endl;
	}

	out << YAML::Key << "sequences";
	out << YAML::Value << YAML::BeginSeq;
	vector<SequenceHandler::SequenceNode*>* sequences = ainb.getSequences();
	for (int i = 0; i < sequences->size(); i++) {
		out << YAML::BeginMap;
		SequenceHandler::SequenceNode* sequence_root = sequences->at(i);
		out << YAML::Key << "sequence_root";
		out << YAML::Value << sequence_root;
		out << YAML::EndMap;
	}
	out << YAML::EndSeq;
	// END OF SEQUENCE DATA



	out << YAML::EndMap;

	assert(out.good());

	ofstream fout;
	string outDir = ainb.getName() + ".yml";
	fout.open(outDir);
	cout << "Writing data to: " << outDir << endl;
	fout << out.c_str();
	fout.close();

	return 0;
}