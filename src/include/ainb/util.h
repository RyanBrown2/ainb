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

	// int32_t
	//static void readIntFromStreamPtr(LPSTREAM stream, int& result) {
	//	char* buffer = new char[4];
	//	stream->Read(buffer, sizeof(int), 0);
	//	result = convertCharArrayToInt(buffer, 4);
	//	delete[] buffer;
	//}

	// int16_t
	//static void read2ByteIntFromStreamPtr(LPSTREAM stream, int& result)
	//{
	//	char* buffer = new char[2];
	//	stream->Read(buffer, sizeof(int16_t), 0);
	//	result = convertCharArrayToInt(buffer, 2);
	//	delete[] buffer;
	//}

	//static LARGE_INTEGER intToLargeInt(int i)
	//{
	//	LARGE_INTEGER li;
	//	li.QuadPart = i;
	//	return li;
	//}

	//enum SeekOrigin
	//{
	//	START = STREAM_SEEK_SET,
	//	CURRENT = STREAM_SEEK_CUR,
	//	END = STREAM_SEEK_END
	//};

	//static void streamSeek(LPSTREAM stream, int pos, SeekOrigin origin)
	//{
	//	stream->Seek(intToLargeInt(pos), origin, 0);
	//}

	//static int streamTell(LPSTREAM stream)
	//{
	//	LARGE_INTEGER liMove;
	//	liMove.QuadPart = 0;

	//	ULARGE_INTEGER uliPosition;
	//	HRESULT hr = stream->Seek(liMove, STREAM_SEEK_CUR, &uliPosition);

	//	if (FAILED(hr)) {
	//		// Handle error, possibly with _com_error(hr).ErrorMessage()
	//		return -1;
	//	}
	//	else {
	//		return uliPosition.QuadPart;
	//	}
	//}

}
