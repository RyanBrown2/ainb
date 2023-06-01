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
	char unknownOne[5];
	file.read(unknownOne, 4);
	unknownOne[4] = '\0';
	int unknownOneAddress = convertHexCharArrayToInt(unknownOne, isBigEndian, 4);
	printf("Unknown One Value: %d\n", unknownOneAddress);
	AINB::unknownOneAddress = unknownOneAddress;

	// Unknown Two
	char unknownTwo[5];
	file.read(unknownTwo, 4);
	unknownTwo[4] = '\0';
	int unknownTwoAddress = convertHexCharArrayToInt(unknownTwo, isBigEndian, 4);
	printf("Unknown Two Value: %d\n", unknownTwoAddress);
	AINB::unknownTwoAddress = unknownTwoAddress;

	// Unknown Three
	char unknownThree[5];
	file.read(unknownThree, 4);
	unknownThree[4] = '\0';
	int unknownThreeAddress = convertHexCharArrayToInt(unknownThree, isBigEndian, 4);
	printf("Unknown Three Value: %d\n", unknownThreeAddress);
	AINB::unknownThreeAddress = unknownThreeAddress;

	parseHead(file);

	//// Header
	//file.seekg(32, ios::cur);

	//char headerEnd[5];
	//file.read(headerEnd, 4);
	//headerEnd[4] = '\0';

	//int headerEndAddress = convertHexCharArrayToInt(headerEnd, false, 2);

	//char stringBegin[5];
	//file.read(stringBegin, 4);
	//stringBegin[4] = '\0';

	//int stringBeginAddress = convertHexCharArrayToInt(stringBegin, isBigEndian, 2);



}

void AINB::parseHead(fstream & file)
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
