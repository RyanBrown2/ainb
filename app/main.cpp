#include <iostream>
#include <fstream>
#include "nin-io/util/util.h"
#include "nin-io/ainb/ainb.h"

using namespace std;

int main(int argc, char* argv[])
{
    //const char* fileDir = "";
    const char* fileDir = "Cannon15_IviesOnlyExistBeforeOpen_b7b6.logic.module.ainb";
    //const char* fileDir = "FormalSequence.root.ainb";
    //const char *fileDir = "Npc_Ganondorf_Human.event.root.ainb";
    //const char *fileDir = "GuideDive_a6f2.logic.module.ainb";

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