#include <iostream>
#include <fstream>
#include "ainb/ainb.h"
#include "yaml-cpp/yaml.h"

using namespace std;

YAML::Emitter& operator << (YAML::Emitter& out, ainb::ParameterHandler::InternalParameterBase* parameter) {
	out << YAML::BeginMap;

	out << YAML::Key << "name";
	out << YAML::Value << parameter->name;

	out << YAML::Key << "value";
	out << YAML::Value << parameter->value;

	out << YAML::EndMap;

	return out;
}

YAML::Emitter& operator << (YAML::Emitter& out, ainb::ParameterHandler::CommandParameterBase* parameter) {
	out << YAML::BeginMap;

	out << YAML::Key << "name";
	out << YAML::Value << parameter->name;

	out << YAML::EndMap;

	return out;
}

const static map<ainb::ParameterHandler::ParameterType, string> typeStrings {
	{ ainb::ParameterHandler::ParameterType::INT, "int"},
	{ ainb::ParameterHandler::ParameterType::BOOL, "bool"},
	{ ainb::ParameterHandler::ParameterType::FLOAT, "float"},
	{ ainb::ParameterHandler::ParameterType::STRING, "string"},
	{ ainb::ParameterHandler::ParameterType::VEC3, "vector3"},
	{ ainb::ParameterHandler::ParameterType::UDT, "udt"},
};

int main(int argc, char* argv[])
{
	if (argc < 2) {
		std::cout << "Usage: ainb-parse <file>" << std::endl;
		return 1;
	}

	const char* fileDir = argv[1];
	cout << "Opening file: " << fileDir << endl;

	//fstream file;
	fstream file;
	file.open(fileDir, fstream::in | fstream::out | ios::binary);

	if (file.fail()) {
		std::cout << "Failed to open file" << std::endl;
		return 1;
	}

	ainb::AINB* ainb = ainb::AINB::loadFromStream(file);

	YAML::Emitter out;
	out << YAML::BeginMap;

	out << YAML::Key << "entry_command_count";
	out << YAML::Value << ainb->getEntryCommandCount();

	out << YAML::Key << "execution_command_count";
	out << YAML::Value << ainb->getExecutionCommandCount();

	out << YAML::Key << "internal_parameters";
	out << YAML::Value << YAML::BeginMap;
	int* internal_counts = ainb->getInternalParameterCounts();
	for (int i = 0; i < 6; i++)
	{
		int count = internal_counts[i];
		if (count == 0) {
			continue;
		}
		out << YAML::Key << typeStrings.at((ainb::ParameterHandler::ParameterType)i);
		out << YAML::Value << YAML::BeginSeq;
		for (int j = 1; j < count; j++)
		{
			out << ainb->getInternalParameterBase(i, j);
		}
		out << YAML::EndSeq;
	}
	out << YAML::EndMap;

	out << YAML::Key << "command_parameters";
	out << YAML::Value << YAML::BeginMap;
	int* command_counts = ainb->getCommandParameterCounts();
	for (int i = 0; i < 12; i++)
	{
		int count = command_counts[i];
		if (count == 0) {
			continue;
		}
		//ainb::ParameterHandler::ParameterType type_num = (ainb::ParameterHandler::ParameterType)(i / 2);

		//string section_type = 
		string section_name = typeStrings.at((ainb::ParameterHandler::ParameterType)(i/2)) + (i % 2 == 0 ? "_input" : "_output");
		//cout << section_name << endl;
		out << YAML::Key << section_name;
		out << YAML::Value << YAML::BeginSeq;
		for (int j = 1; j < count; j++)
		{
			out << ainb->getCommandParameterBase(i, j);
		}
		out << YAML::EndSeq;
	}

	out << YAML::EndMap;

	out << YAML::EndMap;

	assert(out.good());

	ofstream yaml_out("out.yml");

	yaml_out << out.c_str();

	yaml_out.close();

	return 0;
}