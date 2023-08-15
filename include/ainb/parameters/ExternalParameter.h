#pragma once

#include <fstream>
#include <string>

#include "ainb/StringList.h"
#include "ainb/util.h"
#include "ParameterBase.h"

namespace ainb
{
	static const std::map<int, int> ExternalParamAllocSizes = {
		{0, 4},
		{1, 4},
		{2, 4},
		{3, 4},
		{4, 12},
		{5, 4}
	};

	class ExternalParameterBase : public ParameterBase
	{
	public:
		virtual ~ExternalParameterBase() = default;

		virtual void load(std::fstream& stream, StringList* string_list) {};
		virtual void write(std::fstream& stream, StringList* string_list) {};

		void setIndex(int index) { m_index = index; }
		int getIndex() { return m_index; }

	protected:
		int m_index = -1;

		std::string m_second_string = "";

		int m_type_num = -1;
		int m_alloc_size = -1;
	};

	template <ParameterType T>
	class ExternalParameter : public ExternalParameterBase
	{
	public:
		ExternalParameter();
		~ExternalParameter();

		void load(std::fstream& stream, StringList* string_list);
		void write(std::fstream& stream, StringList* string_list);

	private:

	};
}