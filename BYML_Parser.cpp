#include "BYML_Parser.h"
#include <iostream>	
#include <fstream>
#include "Util.h"
#include "Nodes/NodeUtil.h"



BYML::BYML(std::fstream& file)
{
	HeaderData headerData = parseHeaderData(file);
    //BYML::headerData = headerData;
    this->headerData = headerData;
	//std::cout << "Endianness: " << (headerData.isBigEndian ? "Big" : "Small") << std::endl;
	//std::cout << "Version: " << headerData.version << std::endl;
	//std::cout << "Hash Offset: " << headerData.hashOffset << std::endl;
	//std::cout << "String Offset: " << headerData.stringOffset << std::endl;
	//std::cout << "Root Node Offset: " << headerData.rootNodeOffset << std::endl;


    file.seekg(this->headerData.hashOffset, std::ios::cur);

    // Create hash node
    Node node(file, this->headerData.isBigEndian);
}


BYML::~BYML()
{

}

HeaderData BYML::parseHeaderData(std::fstream& file)
{
    //file.clear();
    //file.seekg(0, std::ios::beg);
    
    HeaderData headerData;

    // Endianness
    char endianess[3];
    file.read(endianess, 2);
    endianess[2] = '\0';

    if (strcmp(endianess, "BY") == 0) {
        headerData.isBigEndian = true;
    }
    else {
        headerData.isBigEndian = false;
    }

    // Version
    //file.seekg(2, std::ios::beg);
    char version[3];
    file.read(version, 2);
    version[2] = '\0';
    headerData.version = convertHexCharArrayToInt(version, headerData.isBigEndian, 2);

	// Offset to the hash key table, relative to start (usually 0x010). May be 0 if no hash nodes are used. Must be a string value node (0xc2).
    //file.seekg(4, std::ios::beg);
    char hashOffset[5];
    file.read(hashOffset, 4);
    hashOffset[4] = '\0';
    headerData.hashOffset = convertHexCharArrayToInt(hashOffset, headerData.isBigEndian, 4);

    // Offset to the string table, relative to start. May be 0 if no strings are used. Must be a string value node (0xc2).
    //file.seekg(8, std::ios::beg);
    char stringOffset[5];
    file.read(stringOffset, 4);
    stringOffset[4] = '\0';
    headerData.stringOffset = convertHexCharArrayToInt(stringOffset, headerData.isBigEndian, 4);
    
    // Offset to the root node, relative to start. May be 0 if the document is totally empty. Must be either an array node (0xc0) or one of the hash nodes (0x20, 0x21 or 0xc1).
    //file.seekg(12, std::ios::beg);
    char nodeOffset[5];
    file.read(nodeOffset, 4);
    nodeOffset[4] = '\0';
    headerData.rootNodeOffset = convertHexCharArrayToInt(nodeOffset, headerData.isBigEndian, 4);

    file.seekg(-16, std::ios::cur);

    return headerData;
}
