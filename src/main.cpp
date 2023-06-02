// BYML-Parser.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
//#include <iterator>
//#include <vector>
#include <string>
#include <cstring>
#include "Util.h"
#include "ainb/AINB_parser.h"


using namespace std;

int main()
{
    //const char *fileDir = "";
    //const char *fileDir = "FormalSequence.root.ainb";
    //const char *fileDir = "FastLoadOff.module.ainb";
    const char *fileDir = "GuideDive_a6f2.logic.module.ainb";
    //const char *fileDir = "LargeDungeonThunder_abdc.logic.module.ainb";
    //const char *fileDir = "test.byml";

    fstream file;
    file.open(fileDir, fstream::in | fstream::out | std::ios::binary);

    if (file.fail()) {
		std::cout << "Failed to open file" << std::endl;
		return 1;
	}

    file.seekg(0, ios::beg);

    //BYML fileBYML(file);

    AINB ainb(file);

    //AINB ainb(file);


    file.close();

    return 0;

}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file