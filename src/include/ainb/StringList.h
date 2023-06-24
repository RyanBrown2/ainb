#pragma once
#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <ocidl.h>
#include "util.h"

namespace ainb {
	class StringList {
	public:
		StringList(int* start_pos);
		~StringList();

		void loadFromStream(LPSTREAM stream);

		std::string getStringFromOffset(int offset);
		int* getOffsetFromString(std::string str);
		void finalize();
		void writeToStream(LPSTREAM stream);
	private:
		int* m_start_pos = nullptr;

		// offset to string
		std::map<int, std::string> m_loaded_string_data;

		// string to offset pointer
		std::map<std::string, int*> m_string_data;
	};
}