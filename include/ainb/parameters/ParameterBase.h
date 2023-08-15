#pragma once

#include <string>
#include <fstream>

#include "ainb/StringList.h"

namespace ainb
{
	enum ParameterType {
		INT = 0,
		BOOL = 1,
		FLOAT = 2,
		STRING = 3,
		VEC3 = 4,
		UDT = 5
	};

	class ParameterBase
	{
	public:
		ParameterBase() {};
		virtual ~ParameterBase() = default;

		virtual void load(std::ifstream& file, StringList* string_list) {};
		virtual void write(std::fstream& file, StringList* string_list) {};

		void setName(std::string name) { m_name = name; }
		std::string getName() { return m_name; }

		void setIndex(int index) { m_index = index; }
		int getIndex() { return m_index; }
	protected:
		int m_index = -1;
		std::string m_name = "";
	};

}