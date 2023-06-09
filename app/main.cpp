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

YAML::Emitter& operator << (YAML::Emitter& out, A_Command& command) {
    out << YAML::Flow;
    out << YAML::BeginMap;

    out << YAML::Key << "name";
    out << YAML::Value << command.getName();

    out << YAML::Key << "guid";
    char* guid = command.getGuid();
    stringstream guid_string;
    for (int i = 0; i < 16; i++) {
        guid_string << hex << (static_cast<int>(guid[i]) & 0xFF);
    }
    out << YAML::Value << guid_string.str();

    out << YAML::EndMap;
    return out;
}

YAML::Emitter& operator << (YAML::Emitter& out, B_Command& command) {
    //out << YAML::Flow;
    out << YAML::BeginMap;

    out << YAML::Key << "name";
    out << YAML::Value << command.getName();
    out << YAML::Key << "index";
    out << YAML::Value << command.getIndex();

    out << YAML::Key << "guid";
    char* guid = ((BaseCommand)command).getGuid();
    stringstream guid_string;
    for (int i = 0; i < 16; i++) {
        guid_string << hex << (static_cast<int>(guid[i]) & 0xFF);
    }
    out << YAML::Value << guid_string.str();

    // Body Data

    out << YAML::Key << "body";
    out << YAML::Value;
    out << YAML::BeginMap;

    B_Command::BodyData body_data = command.getBodyData();
    map<int, int> position_to_key = body_data.position_to_key;
    map<int, int> value_map = body_data.value_map;

    B_Command::Table table = body_data.table;

    out << YAML::Key << "data";
    out << YAML::Value;
    out << YAML::Flow;
    out << YAML::BeginSeq;

    for (const auto& pair : position_to_key) {
        int pos = pair.first;
        int key = pair.second;
        int value = value_map[key];

        out << YAML::BeginMap;
        out << YAML::Key << "position";
        out << YAML::Value << pos;
        out << YAML::Key << "key";
        out << YAML::Value << key;
        out << YAML::Key << "value";
        out << YAML::Value << value;
		out << YAML::EndMap;
    }
    out << YAML::EndSeq;

    if (body_data.table.entries.size() == 0) {

		out << YAML::EndMap;
		out << YAML::EndMap;
        return out;
    }

    out << YAML::Key << "table";
    out << YAML::Value;
    out << YAML::BeginSeq;
    B_Command::Table body_table = body_data.table;
    for (int i = 0; i < body_table.entries.size(); i++) {
		int value = body_table.entries[i].value1;
        string entry_string = body_table.entries[i].value2;
        out << YAML::BeginMap;
        out << YAML::Key << "string";
        out << YAML::Value << entry_string;
        out << YAML::Key << "value";
        out << YAML::Value << value;
		out << YAML::EndMap;
    }
    out << YAML::EndSeq;


    out << YAML::EndMap;

    out << YAML::EndMap;
    return out;
}

int main(int argc, char* argv[])
{
    //const char* fileDir = "";
    //const char* fileDir = "FastLoadOff.module.ainb";
    //const char* fileDir = "LargeDungeonWater_AllinArea_895e.logic.module.ainb";
    //const char* fileDir = "Set_Defense_Karakara_ee67.logic.module.ainb";
    const char* fileDir = "CustomHouseControlActor.event.root.ainb";

    fstream file;
    file.open(fileDir, fstream::in | fstream::out | std::ios::binary);

    if (file.fail()) {
		std::cout << "Failed to open file" << std::endl;
		return 1;
	}

    AINB ainb;

    ainb.load(file);

    file.close();

    vector<int> data_chunks;

    
    vector<A_Command> a_commands = ainb.getACommands();
    vector<B_Command> b_commands = ainb.getBCommands();

    YAML::Emitter out;
    out << YAML::BeginMap;

    out << YAML::Key << "name";
    out << YAML::Value << ainb.getName();

    // header data
    AINB::AinbHeaderData header_data = ainb.getHeaderData();

    out << YAML::Key << "header_data";
    out << YAML::BeginMap;
    out << YAML::Key << "a_command_count";
    out << YAML::Value << header_data.a_commands;
    out << YAML::Key << "b_command_count";
    out << YAML::Value << header_data.b_commands;
    out << YAML::EndMap;

    // A Commands
    out << YAML::Key << "a_commands";
    //out << YAML::Flow;
    out << YAML::BeginSeq;
    for (int i = 0; i < a_commands.size(); i++) {
        A_Command command = a_commands[i];
        out << command;
    }
    out << YAML::EndSeq;

    // B Commands
    out << YAML::Key << "b_commands";
    //out << YAML::Flow;
    out << YAML::BeginSeq;
    for (int i = 0; i < b_commands.size(); i++) {
        B_Command command = b_commands[i];
        out << command;
    }
    out << YAML::EndSeq;




    out << YAML::EndMap;
    assert(out.good());
    cout << out.c_str();

    ofstream out_file;
    string output_dir = ainb.getName() + ".yml";
    out_file.open(output_dir);
    out_file << out.c_str();
    out_file.close();

    cout << endl;
    //std::cout << "Here's the output YAML:\n" << out.c_str(); // prints "Hello, World!"

    return 0;
}