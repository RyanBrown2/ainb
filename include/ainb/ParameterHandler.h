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

namespace ainb {

	enum ParameterType {
		INT = 0,
		BOOL = 1,
		FLOAT = 2,
		STRING = 3,
		VEC3 = 4,
		UDT = 5
	};

	struct InternalParameterBase {
		int address = -1;
		int index = -1;
		std::string name = "";
		std::string value = "";

		int type_num = -1;

		InternalParameterBase() {}

		virtual void load(std::fstream& stream, StringList* string_list) {};
		virtual void write(std::fstream& stream, StringList* string_list) {};
	};

	template <ParameterType Type>
	struct InternalParameter : public InternalParameterBase {
		ParameterType type = Type;
		
		void load(std::fstream& stream, StringList* string_list) override;
		void write(std::fstream& stream, StringList* string_list) override;
	};

	struct CommandParameterBase
	{
		int address = -1;
		int index = -1;
		std::string name = "";
		bool is_input = false;

		// todo: once command loading gets finished, need to make this a command pointer instead
		// also will need to implement a linking pass to link the command pointers to the command
		int command_ref = -1; 

		int type_num = -1;

		CommandParameterBase() {}

		virtual ~CommandParameterBase() = default;
		virtual void load(std::fstream& stream, StringList* string_list, bool is_input) {};
		virtual void write(std::fstream& stream, StringList* string_list, bool is_input) {};
		virtual std::map<std::string, std::string>* getExtras() { return new std::map<std::string, std::string>(); }; // any data specific to the parameter type
	};

	template <ParameterType Type>
	struct CommandParameter : public CommandParameterBase
	{
		ParameterType type = Type;

		CommandParameter() {}

		void load(std::fstream& stream, StringList* string_list, bool is_input) override;
		void write(std::fstream& stream, StringList* string_list, bool is_input) override;
		std::map<std::string, std::string>* getExtras() override;
	};

	template <>
	struct CommandParameter<ParameterType::UDT> : public CommandParameterBase
	{
		std::string type_name = "";

		CommandParameter(): CommandParameterBase() {}

		void load(std::fstream& stream, StringList* string_list, bool is_input) override;
		void write(std::fstream& stream, StringList* string_list, bool is_input) override;
		std::map<std::string, std::string>* getExtras() override;
	};

	static const std::map<int, int> InternalParamEntryLengths = {
		{0, 0x0c},
		{1, 0x0c},
		{2, 0x0c},
		{3, 0x0c},
		{4, 0x14}
	};

	static const std::map<int, int> CommandParamEntryLengths = {
		{0, 0x10},
		{1, 0x4},
		{2, 0x10},
		{3, 0x4},
		{4, 0x10},
		{5, 0x4},
		{6, 0x10},
		{7, 0x4},
		{8, 0x18},
		{9, 0x4},
		{10, 0x14},
		{11, 0x8}
	};

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
	std::unique_ptr<CommandParameterBase> createCommandParameter() {
		return std::make_unique<CommandParameter<Type>>();
	}

	template <ParameterType Type>
	void pushCommandParameter(std::vector<std::unique_ptr<CommandParameterBase>>& vec) {
		vec.push_back(std::make_unique<CommandParameter<Type>>());
	}

	void createAndLoadInternalParameter(std::fstream& stream, int section_number);
	void createAndLoadCommandParameter(std::fstream& stream, int section_number);

	void updateParameterIndices();

};

}