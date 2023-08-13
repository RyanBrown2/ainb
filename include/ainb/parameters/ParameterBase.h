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
	protected:
		std::string m_name = "";
	};

}