#pragma once

#include <fstream>
#include <string>
#include <map>

#include "ainb/StringList.h"
#include "ParameterBase.h"

namespace ainb
{

	static const std::map<int, int> InternalParamEntryLengths = {
		{0, 0x0c},
		{1, 0x0c},
		{2, 0x0c},
		{3, 0x0c},
		{4, 0x14}
	};

	class InternalParameterBase : public ParameterBase
	{
	public:
		virtual ~InternalParameterBase() = default;

		virtual void load(std::fstream& file, StringList* string_list) {};
		virtual void write(std::fstream& file, StringList* string_list) {};

		void setValue(std::string value) { m_value = value; }
		std::string getValue() { return m_value; }

	protected:
		std::string m_value = "";

		int m_type_num = -1;
	};

	template <ParameterType T>
	class InternalParameter : public InternalParameterBase
	{
	public:
		InternalParameter();
		~InternalParameter();

		void load(std::fstream& stream, StringList* string_list) override;
		void write(std::fstream& stream, StringList* string_list) override;
	private:
	};
}