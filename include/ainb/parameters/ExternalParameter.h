#pragma once

#include <fstream>
#include <string>

#include "ainb/StringList.h"
#include "ParameterBase.h"

namespace ainb
{
	class ExternalParameterBase : public ParameterBase
	{
	public:
		virtual ~ExternalParameterBase() = default;

		void setIndex(int index) { m_index = index; }
		int getIndex() { return m_index; }

	protected:
		int m_index = -1;

		std::string second_string = "";
	};
}