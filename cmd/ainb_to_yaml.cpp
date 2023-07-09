#include <iostream>
#include <fstream>
//#include "comutil.h"
#include "ainb/ainb.h"
#include "yaml-cpp/yaml.h"
#include <cassert>

using namespace std;

YAML::Emitter& operator << (YAML::Emitter& out, ainb::InternalParameterBase* parameter) {
	out << YAML::BeginMap;

	out << YAML::Key << "name";
	out << YAML::Value << parameter->name;

	out << YAML::Key << "value";
	out << YAML::Value << parameter->value;

	out << YAML::EndMap;

	return out;
}

YAML::Emitter& operator << (YAML::Emitter& out, ainb::CommandParameterBase* parameter) {
	out << YAML::BeginMap;

	out << YAML::Key << "name";
	out << YAML::Value << parameter->name;

	out << YAML::EndMap;

	return out;
}

YAML::Emitter& operator << (YAML::Emitter& out, ainb::SequenceNode* node) {

	out << YAML::BeginMap;

	out << YAML::Key << "name";
	out << YAML::Value << node->getName();

	//out << YAML::Key << "";
	//out << YAML::Value << 

	out << YAML::EndMap;

	return out;
}

const static map<ainb::ParameterType, string> typeStrings {
	{ ainb::ParameterType::INT, "int"},
	{ ainb::ParameterType::BOOL, "bool"},
	{ ainb::ParameterType::FLOAT, "float"},
	{ ainb::ParameterType::STRING, "string"},
	{ ainb::ParameterType::VEC3, "vector3"},
	{ ainb::ParameterType::UDT, "udt"},
};

void handleInternalParameters(YAML::Emitter& out, ainb::AINB* ainb)
{
	out << YAML::Key << "internal_parameters";
	out << YAML::Value << YAML::BeginMap;
	int* internal_counts = ainb->getInternalParameterCounts();
	for (int i = 0; i < 6; i++)
	{
		int count = internal_counts[i];
		if (count == 0) {
			continue;
		}
		out << YAML::Key << typeStrings.at((ainb::ParameterType)i);
		out << YAML::Value << YAML::BeginSeq;
		for (int j = 1; j < count; j++)
		{
			out << ainb->getInternalParameter(i, j);
		}
		out << YAML::EndSeq;
	}
	out << YAML::EndMap;

}

void handleCommandParameters(YAML::Emitter& out, ainb::AINB* ainb)
{
	out << YAML::Key << "command_parameters";
	out << YAML::Value << YAML::BeginMap;
	int* command_counts = ainb->getCommandParameterCounts();
	for (int i = 0; i < 12; i++)
	{
		int count = command_counts[i];
		if (count == 0) {
			continue;
		}
		string section_name = typeStrings.at((ainb::ParameterType)(i/2)) + (i % 2 == 0 ? "_input" : "_output");
		//cout << section_name << endl;
		out << YAML::Key << section_name;
		out << YAML::Value << YAML::BeginSeq;
		for (int j = 1; j < count; j++)
		{
			out << ainb->getCommandParameter(i, j);
		}
		out << YAML::EndSeq;
	}
	out << YAML::EndMap;

}

void handleSequenceNodes(YAML::Emitter& out, ainb::AINB* ainb)
{
	out << YAML::Key << "sequence_nodes";
	out << YAML::Value << YAML::BeginSeq;
	for (int i = 0; i < ainb->getSequenceNodes().size(); i++)
	{
		out << ainb->getSequenceNodes().at(i);
	}
	out << YAML::EndSeq;
}

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

	ainb::AINB* ainb = new ainb::AINB(file);

	YAML::Emitter out;
	out << YAML::BeginMap;

	out << YAML::Key << "entry_command_count";
	out << YAML::Value << ainb->getEntryCommandCount();

	out << YAML::Key << "execution_command_count";
	out << YAML::Value << ainb->getExecutionCommandCount();

	handleInternalParameters(out, ainb);

	handleCommandParameters(out, ainb);

	handleSequenceNodes(out, ainb);

	out << YAML::EndMap;

	assert(out.good());


	string yaml_out_dir = ainb->getName() + ".yml";
	ofstream yaml_out(yaml_out_dir);

	cout << "Writing to file" << endl;

	yaml_out << out.c_str();

	yaml_out.close();


	string ainb_out_dir = ainb->getName() + ".modified" + ".ainb";
	fstream ainb_out(ainb_out_dir, fstream::out | ios::binary);

	cout << "Writing ainb file" << endl;

	ainb->writeToStream(ainb_out);

	ainb_out.close();

	return 0;
}