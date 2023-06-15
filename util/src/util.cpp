#include "nin-io/util/util.h"

int convertHexCharArrayToInt(char* array, bool isBigEndian, int len) {
    int result = 0;
    if (isBigEndian) {
        for (int i = 0; i < len; ++i) {
            result |= ((uint8_t)array[i]) << (8 * (len - 1 - i));
        }
    }
    else {
        for (int i = 0; i < len; ++i) {
            result |= ((uint8_t)array[i]) << (8 * i);
        }
    }
    return result;
}

void displayCharArrayAsHex(std::ostream& os, char* arr, int len)
{
    for (int i = 0; i < len; ++i)
    {
        os << std::setw(2) << std::setfill('0') << std::hex << (static_cast<int>(arr[i]) & 0xFF) << " ";
    }
    os << std::dec << std::endl;
}


void readIntFromStream(std::istream& stream, bool isBigEndian, int size, int& result)
{
	char* buffer = new char[size];
	stream.read(buffer, size);
	result = convertHexCharArrayToInt(buffer, isBigEndian, size);
	delete[] buffer;
}

char* rawToHex(const char* rawData, int length)
{
	std::stringstream hexStream;
	hexStream << std::hex << std::setfill('0');

	for (std::size_t i = 0; i < length; ++i)
	{
		hexStream << std::setw(2) << static_cast<unsigned int>(static_cast<unsigned char>(rawData[i]));
	}

	std::string hexString = hexStream.str();
	char* hexData = new char[hexString.length() + 1];
	std::strcpy(hexData, hexString.c_str());

	return hexData;
}
