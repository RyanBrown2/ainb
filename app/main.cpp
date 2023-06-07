#include <iostream>
#include <fstream>
#include "nin-io/util/util.h"
#include "nin-io/ainb/ainb.h"

using namespace std;

int main(int argc, char* argv[])
{
    //const char* fileDir = "";
    //const char* fileDir = "Title.module.ainb";
    //const char* fileDir = "FastLoadOff.module.ainb";
    //const char* fileDir = "LargeDungeonWater_AllinArea_895e.logic.module.ainb";
    const char* fileDir = "Set_Defense_Karakara_ee67.logic.module.ainb";
    //const char* fileDir = "CustomHouseControlActor.event.root.ainb";
    //const char *fileDir = "Npc_Ganondorf_Human.event.root.ainb";

    //const char* fileDir = "NpcGoronSage.utility.root.ainb";

    fstream file;
    file.open(fileDir, fstream::in | fstream::out | std::ios::binary);

    if (file.fail()) {
		std::cout << "Failed to open file" << std::endl;
		return 1;
	}

    AINB ainb;

    ainb.load(file);

    file.close();

    return 0;
}