#include "AINB_parser.h"
#include <iostream>
#include "../Util.h"
#include <string>
#include <vector>
#include <sstream>

using namespace std;



AINB::AINB(fstream& file)
{
	parse(file);

}

AINB::~AINB()
{

}

void AINB::parse(fstream& file)
{
	file.seekg(0, ios::beg);

	// Unknown One
	file.seekg(12, ios::beg);
	char headerOne[5];
	file.read(headerOne, 4);
	headerOne[4] = '\0';
	int headerOneInt = convertHexCharArrayToInt(headerOne, isBigEndian, 4);
	printf("Header One Value: %d\n", headerOneInt);
	AINB::headerOne = headerOneInt;

	// Unknown Two
	char headerTwo[5];
	file.read(headerTwo, 4);
	headerTwo[4] = '\0';
	int headerTwoInt = convertHexCharArrayToInt(headerTwo, isBigEndian, 4);
	printf("Header Two Value: %d\n", headerTwoInt);
	AINB::headerTwo = headerTwoInt;

	// Unknown Three
	char headerThree[5];
	file.read(headerThree, 4);
	headerThree[4] = '\0';
	int headerThreeInt = convertHexCharArrayToInt(headerThree, isBigEndian, 4);
	printf("Header Three Value: %d\n", headerThreeInt);
	AINB::headerThree = headerThreeInt;

	cout << endl;

	int firstDataBandAddress;
	if (headerOneInt > 0) {
		firstDataBandAddress = 0x74;
	}
	else {
		firstDataBandAddress = 0x74;
	}

	//file.seekg(0, ios::beg);
	parseDataBlock(file, firstDataBandAddress);


	//parseHead(file);

}

AINB::DataBlock AINB::parseDataBlock(fstream& file, int beginAddress)
{
	//file.clear();
	file.seekg(beginAddress, ios::beg);

	DataBlock datablock;

	datablock.version = 0; // temp for testing

	// get index
	file.seekg(2, ios::cur);
	char indexChar[3];
	file.read(indexChar, 2);
	indexChar[2] = '\0';
	int index = convertHexCharArrayToInt(indexChar, isBigEndian, 2);
	datablock.index = index;

	// get data
	file.seekg(beginAddress, ios::beg);
	file.seekg(0x2c, ios::cur);
	char dataChar[17];
	file.read(dataChar, 16);
	dataChar[16] = '\0';
	//strcpy_s(datablock.data, dataChar);



	cout << "Data Block " << to_string(index) << endl;
	cout << "Data: ";

	displayCharArrayAsHex(dataChar, 16);

	return datablock;
	//for (int i = 0; i < 16; ++i)
	//	std::cout << std::hex << (int)dataChar[i];
	//cout << endl;
}

void AINB::parseHead(fstream& file)
{
	file.clear();
	file.seekg(0, ios::beg);

	file.seekg(3, ios::cur);
	char headerAddress[2];
	file.read(headerAddress, 1);
	headerAddress[1] = '\0';

	// This address is absolute, not relative to current pos
	int headerAddressInt = convertHexCharArrayToInt(headerAddress, isBigEndian, 1);
	file.seekg(headerAddressInt, ios::beg);
	
	// Parse head data
	char headEnd[5];
	file.read(headEnd, 4);
	headEnd[4] = '\0';

	int headEndAddress = convertHexCharArrayToInt(headEnd, isBigEndian, 4);
	int headSize = headEndAddress - headerAddressInt;

	// get string begin address
	char stringBegin[5];
	file.read(stringBegin, 4);
	stringBegin[4] = '\0';

	int stringBeginAddress = convertHexCharArrayToInt(stringBegin, isBigEndian, 4);


	parseStringArray(file, stringBeginAddress);
	// get data section one end address
	//char dataSectionOneEnd[5];
	//file.read(dataSectionOneEnd, 4);
	//dataSectionOneEnd[4] = '\0';

	//int dataSectionOneEndAddress = convertHexCharArrayToInt(dataSectionOneEnd, isBigEndian, 4);
	file.seekg(4, ios::cur); // note, there is a value we are skipping over. I don't know what it is, if I read it in the future this line should be removed

	// Read data section one address
	char dataSectionOneBegin[5];
	file.read(dataSectionOneBegin, 4);
	dataSectionOneBegin[4] = '\0';

	int dataSectionOneBeginAddress = convertHexCharArrayToInt(dataSectionOneBegin, isBigEndian, 4);

	parseData(file, dataSectionOneBeginAddress);
	
}

void AINB::parseData(fstream& file, int beginAddress)
{
	
}


vector<std::string> AINB::parseStringArray(std::fstream& file, int beginAddress)
{
	file.clear();
	file.seekg(beginAddress, ios::beg);

	vector<string> strings;

	char currentString[100];

	file.getline(currentString, 100, '\0');


	while (!file.eof())
	{
		if (strlen(currentString) > 0) {
			strings.push_back(currentString);
		}
		file.getline(currentString, 100, '\0');
	}

	cout << "Strings: " << endl;
	for (int i = 0; i < strings.size(); i++)
	{
		cout << strings[i] << endl;
	}

	return strings;


}
