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

	if (argc < 2) {
		std::cout << "Usage: ainb-parse <file>" << std::endl;
		return 1;
	}

	const char* fileDir = argv[1];
	cout << "Opening file: " << fileDir << endl;

	fstream file;
	file.open(fileDir, fstream::in | fstream::out | ios::binary);

	if (file.fail()) {
		std::cout << "Failed to open file" << std::endl;
		return 1;
	}

	AINB* ainb = new AINB();

	ainb->loadFromFile(file);

	file.close();

	YAML::Emitter out;
	out << YAML::BeginMap;

	// HEADER DATA
	out << YAML::Key << "header_data";
	out << YAML::Value << YAML::BeginMap;
	out << YAML::Key << "type";
	out << YAML::Value << ainb->getType();
	out << YAML::Key << "entry_point_command_count";
	out << YAML::Value << ainb->getEntryPointCommandCount();
	out << YAML::Key << "execution_command_count";
	out << YAML::Value << ainb->getExecutionCommandCount();
	out << YAML::EndMap;

	out << YAML::EndMap;
	assert(out.good());

	string ouput_dir = "test.yml";
	fstream output;
	output.open(ouput_dir, fstream::out);
	cout << "Writing data to: " << ouput_dir << endl;
	output << out.c_str();
	output.close();

	return 0;

	string ainb_copy_dir = "test.ainb";
	fstream ainb_copy;
	ainb_copy.open(ainb_copy_dir, fstream::out | ios::binary);
	cout << "Writing data to: " << ainb_copy_dir << endl;
	ainb->writeToFile(ainb_copy);

	ainb_copy.close();


	return 0;
}
