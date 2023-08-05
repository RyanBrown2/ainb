#pragma once
#include <iostream>
#include <fstream>

namespace ainb {

	static const bool is_big_endian = false;

	static int convertCharArrayToInt(char* array, int len)
	{
		int result = 0;
		if (is_big_endian) {
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

	static char* convertIntToCharArray(int num, int len)
	{
		char* array = new char[len];
		if (is_big_endian) {
			for (int i = 0; i < len; ++i) {
				array[i] = (num >> (8 * (len - 1 - i))) & 0xFF;
			}
		}
		else {
			for (int i = 0; i < len; ++i) {
				array[i] = (num >> (8 * i)) & 0xFF;
			}
		}
		return array;
	}

	static void readIntFromStream(std::fstream& stream, int size, int& result) {
		char* buffer = new char[size];
		stream.read(buffer, size);
		result = convertCharArrayToInt(buffer, size);
		delete[] buffer;
	}

	static void writeIntToStream(std::fstream& stream, int size, int value) {
		char* buffer = convertIntToCharArray(value, size);
		stream.write(buffer, size);
		delete[] buffer;
	}

}
