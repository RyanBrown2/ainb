#include "DataBlock.h"

using namespace std;

DataBlock::DataBlock(int address)
{
	DataBlock::address = address;

	this->data = new char[17];
	this->index = 0;
}

DataBlock::~DataBlock()
{
	delete[] data;
}

void DataBlock::load(fstream& file)
{
	int startAddress = file.tellg();
	file.seekg(address, ios::beg);

	//// Index
	char* indexString = new char[5];
	file.read(indexString, 4);
	indexString[4] = '\0';
	int indexInt = convertHexCharArrayToInt(indexString, false, 4);
	this->index = indexInt;
	delete[] indexString;

	//// Data
	file.read(this->data, 16);
	this->data[16] = '\0';

	file.seekg(startAddress, ios::beg);
}


void DataBlock::print()
{
	cout << "Data Block Address: " << address << endl;
	cout << "Data Block Index: " << index << endl;
	cout << "Data Block Data: ";
	displayCharArrayAsHex(data, 16);
	cout << endl;
}