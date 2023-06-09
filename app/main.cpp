#include <iostream>
#include <fstream>
#include "nin-io/util/util.h"
#include "nin-io/ainb/ainb.h"
#include "yaml-cpp/yaml.h"

using namespace std;

int main(int argc, char* argv[])
{
    //const char* fileDir = "";
    //const char* fileDir = "FastLoadOff.module.ainb";
    //const char* fileDir = "LargeDungeonWater_AllinArea_895e.logic.module.ainb";
    const char* fileDir = "Set_Defense_Karakara_ee67.logic.module.ainb";
    //const char* fileDir = "CustomHouseControlActor.event.root.ainb";

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

    out << YAML::Key << "name";
    out << YAML::Value << ainb.getName();

    out << YAML::EndMap;
    cout << out.c_str();

    cout << endl;
    //std::cout << "Here's the output YAML:\n" << out.c_str(); // prints "Hello, World!"

    return 0;
}