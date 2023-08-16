#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <variant>
#include <memory>
#include <map>

#include "StringList.h"
#include "parameters/ParameterBase.h"
#include "parameters/InternalParameter.h"
#include "parameters/CommandParameter.h"

namespace ainb {

class ParameterHandler {
public:
	ParameterHandler(StringList* string_list);
	~ParameterHandler();

	void loadInternalParameters(std::fstream& stream, int end_address);
	void loadCommandParameters(std::fstream& stream, int end_address);

	std::vector<int>* getActiveInternalParameterTypes() { return &m_active_internal_parameter_types; }
	std::vector<int>* getActiveCommandParameterTypes() { return &m_active_command_parameter_types; }

	InternalParameterBase* getInternalParameter(int section_num, int parameter_num);
	CommandParameterBase* getCommandParameter(int section_num, int parameter_num);

	// get a vector of all the parameters in a section
	std::vector<std::unique_ptr<InternalParameterBase>>* getInternalParameters(int section_num);

	std::vector<std::unique_ptr<CommandParameterBase>>* getCommandParameters(int section_num);

	void writeInternalParametersToStream(std::fstream& stream);
	void writeCommandParametersToStream(std::fstream& stream);

	void finalize();

private:
	StringList* m_string_list;

	std::map<int, std::vector<std::unique_ptr<InternalParameterBase>>> m_internal_parameters;

	std::map<int, std::vector<std::unique_ptr<CommandParameterBase>>> m_command_parameters;

	std::vector<int> m_active_internal_parameter_types;

	// vector containing the "section" number of each parameter list that is active
	std::vector<int> m_active_command_parameter_types;

	template <ParameterType Type>
	std::unique_ptr<CommandParameterBase> createCommandParameter()
	{
		return std::make_unique<CommandParameter<Type>>();
	}

	template <ParameterType Type>
	void pushCommandParameter(std::vector<std::unique_ptr<CommandParameterBase>>& vec)
	{
		vec.push_back(std::make_unique<CommandParameter<Type>>());
	}

	void createAndLoadInternalParameter(std::fstream& stream, int section_number);
	void createAndLoadCommandParameter(std::fstream& stream, int section_number);

	void updateParameterIndices();

};

}