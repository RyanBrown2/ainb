#pragma once
#include <iostream>
#include <ocidl.h>
#include <cstdint>

namespace ainb {

	static const bool is_big_endian = false;

	static int convertCharArrayToInt(char* array, int len) {
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

	static void readIntFromStream(LPSTREAM stream, int& result) {
		char* buffer = new char[4];
		stream->Read(buffer, sizeof(int), 0);
		result = convertCharArrayToInt(buffer, 4);
		delete[] buffer;
	}

	static LARGE_INTEGER intToLargeInt(int i) {
		LARGE_INTEGER li;
		li.QuadPart = i;
		return li;
	}

	enum SeekOrigin {
		START = STREAM_SEEK_SET,
		CURRENT = STREAM_SEEK_CUR,
		END = STREAM_SEEK_END
	};

	static void streamSeek(LPSTREAM stream, int pos, SeekOrigin origin) {
		stream->Seek(intToLargeInt(pos), origin, 0);
	}

	static int streamTell(LPSTREAM stream) {
		LARGE_INTEGER liMove;
		liMove.QuadPart = 0;

		ULARGE_INTEGER uliPosition;
		HRESULT hr = stream->Seek(liMove, STREAM_SEEK_CUR, &uliPosition);

		if (FAILED(hr)) {
			// Handle error, possibly with _com_error(hr).ErrorMessage()
		}
		else {
			return uliPosition.QuadPart;
		}
	}

}
