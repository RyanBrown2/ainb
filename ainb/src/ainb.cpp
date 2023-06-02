#include "ainb/Ainb.h"
#include "util/util.h"
#include <iostream>
#include <fstream>
#include "ainb/DataBlock.h"
#include "ainb/A_Block.h"
#include "ainb/B_Block.h"
#include <vector>

using namespace std;

AINB::AINB()
{
}

AINB::~AINB()
{

}

void AINB::load(fstream& file)
{
	cout << "Loading AINB" << endl << endl;
	bool isBigEndian = false;

	file.seekg(0, ios::beg);

	// Reading Header Data
	char type[4];
	file.read(type, 3);
	type[3] = '\0';

	// Check if file is AINB
	if (strcmp(type, "AIB") != 0) {
		cout << "File is not AINB" << endl;
		return;
	}

	char headerDataStartChar[2];
	file.read(headerDataStartChar, 1);
	headerDataStartChar[1] = '\0';
	int headerDataStart = convertHexCharArrayToInt(headerDataStartChar, isBigEndian, 1);

	// find number of type A blocks
	file.seekg(0xC, ios::beg);
	char a_BlockCountChar[5];
	file.read(a_BlockCountChar, 4);
	a_BlockCountChar[4] = '\0';
	int a_BlockCount = convertHexCharArrayToInt(a_BlockCountChar, isBigEndian, 4);
	cout << "Type A Block Count: " << a_BlockCount << endl;

	// find end address of block section
	file.seekg(0x20, ios::beg);
	char blockEndAddressChar[5];
	file.read(blockEndAddressChar, 4);
	blockEndAddressChar[4] = '\0';
	int blockEndAddress = convertHexCharArrayToInt(blockEndAddressChar, isBigEndian, 4);

	// Start Parsing DataBlocks
	cout << "Loading Type A Blocks" << endl;
	file.seekg(0x74, ios::beg);

	vector<A_Block> aBlocks;
	for (int i = 0; i < a_BlockCount; i++) {
		A_Block aBlock = A_Block::load(file);
		aBlocks.push_back(aBlock);
	}

	cout << "Finished Loading Type A Blocks" << endl << endl;


	cout << "Loading Type B Blocks" << endl;
	int currentAddress = file.tellg();
	int bBlockCount = (blockEndAddress - currentAddress) / 0x3c;

	cout << "Type B Block Count: " << bBlockCount << endl;

	vector<B_Block> bBlocks;
	while (currentAddress < blockEndAddress) {
		B_Block bBlock = B_Block::load(file);
		currentAddress = file.tellg();
		cout << "Current Address: " <<  std::hex << currentAddress << endl;
		bBlocks.push_back(bBlock);
	}


	cout << "Finished Loading Type B Blocks" << endl << endl;


	//if (headerBlockCount != 0) {
	//	cout << "TODO: handle header blocks" << endl;
	//	cout << "Stopping AINB load" << endl;
	//	return;
	//}
	
	// read data blocks



	//displayCharArrayAsHex(dataBlock.data, 16);


}
