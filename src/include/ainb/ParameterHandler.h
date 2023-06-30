#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <variant>
#include <map>
#include <ocidl.h>
#include "StringList.h"
#include "util.h"

namespace ainb {

class ParameterHandler {
public:
	ParameterHandler(StringList* string_list);
	~ParameterHandler();

	// todo: handle section 3 strings
	// might create template
	struct TableParameter {
		int address = -1;
		int index = -1;
		std::string name = "";
		int value = 0;
		
		void load(LPSTREAM stream, StringList* string_list, int section_num);
	};

	enum ParameterType {
		INT = 0,
		BOOL = 1,
		FLOAT = 2,
		STRING = 3,
		VEC3 = 4,
		UDF = 5
	};

	struct CommandParameterBase
	{
		int address = -1;
		int index = -1;
		std::string name = "";
		int command_ref = -1;
		bool is_input = false;

		CommandParameterBase() {}
		//CommandParameterBase(bool is_input) : is_input(is_input) {}

		virtual ~CommandParameterBase() = default;
		virtual void load(LPSTREAM stream, StringList* string_list, bool is_input) {};
	};

	template <ParameterType Type>
	struct CommandParameter : public CommandParameterBase
	{
		//CommandParameter(bool input) : CommandParameterBase(input) {};

		CommandParameter() {}

		void load(LPSTREAM stream, StringList* string_list, bool is_input) override;
	};

	template <>
	struct CommandParameter<ParameterType::UDF> : public CommandParameterBase
	{
		std::string type_name = "";

		CommandParameter() {}
		//CommandParameter(bool input) : CommandParameterBase(input) {}

		void load(LPSTREAM stream, StringList* string_list, bool is_input) override;
	};

	void loadTableParameters(LPSTREAM stream, int end_address);

	void loadCommandParameters(LPSTREAM stream, int end_address);

	std::vector<int>* getActiveTables() { return &m_active_tables; }
	std::vector<int>* getActiveParameterLists() { return &m_active_paramter_lists; }

	TableParameter* getParameterFromTable(int table_num, int parameter_num);

	std::vector<TableParameter>* getTableParameters(int table_num);

	std::vector<std::unique_ptr<CommandParameterBase>>* getCommandParameters(int section_num);

private:
	StringList* m_string_list;

	// table_num to vector of parameters
	std::map<int, std::vector<TableParameter>> m_table_parameters;

	std::map<int, std::vector<std::unique_ptr<CommandParameterBase>>> m_command_parameters;

	std::vector<int> m_active_tables;

	// vector containing the "section" number of each parameter list that is active
	std::vector<int> m_active_paramter_lists;

	template <ParameterType Type>
	std::unique_ptr<CommandParameterBase> createCommandParameter() {
		return std::make_unique<CommandParameter<Type>>();
	}

	template <>
	std::unique_ptr<CommandParameterBase> createCommandParameter<ParameterType::UDF>() {
		return std::make_unique<CommandParameter<ParameterType::UDF>>();
	}

	template <ParameterType Type>
	void pushCommandParameter(std::vector<std::unique_ptr<CommandParameterBase>>& vec) {
		vec.push_back(std::make_unique<CommandParameter<Type>>());
	}

	template <>
	void pushCommandParameter<ParameterType::UDF>(std::vector<std::unique_ptr<CommandParameterBase>>& vec) {
		vec.push_back(std::make_unique<CommandParameter<ParameterType::UDF>>());
		// Additional operations specific to Type1
	}

	void createAndLoadCommandParameter(LPSTREAM stream, int section_number);

	static std::map<int, int> table_entry_lengths;
	static std::map<int, int> parameter_entry_lengths;
};

}