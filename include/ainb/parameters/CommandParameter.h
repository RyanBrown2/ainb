#pragma once

#include <fstream>
#include <string>
#include <map>

#include "ParameterBase.h"

namespace ainb
{

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

	class CommandParameterBase : public ParameterBase
	{
	public:
		virtual ~CommandParameterBase() = default;

		virtual void load(std::fstream& stream, StringList* string_list, bool is_input) {};
		virtual void write(std::fstream& stream, StringList* string_list, bool is_input) {};
		virtual std::map<std::string, std::string>* getExtras() { return new std::map<std::string, std::string>(); }; // any data specific to the parameter type

		void setAddress(int address) { m_address = address; }
		int getAddress() { return m_address; }

		void setIsInput(bool is_input) { m_is_input = is_input; }
		bool getIsInput() { return m_is_input; }

	protected:
		int m_address = -1;
		bool m_is_input = false;

		// todo: once command loading gets finished, need to make this a command pointer instead
		// also will need to implement a linking pass to link the command pointers to the command
		int m_command_ref = -1; 

		int m_type_num = -1;
	};

	template <ParameterType T>
	class CommandParameter : public CommandParameterBase
	{
	public:
		CommandParameter();
		~CommandParameter();

		void load(std::fstream& stream, StringList* string_list, bool is_input) override;
		void write(std::fstream& stream, StringList* string_list, bool is_input) override;
		std::map<std::string, std::string>* getExtras() override;
	private:
		ParameterType m_type = T;
	};

	template <>
	class CommandParameter<ParameterType::UDT> : public CommandParameterBase
	{
	public:
		CommandParameter();

		void load(std::fstream& stream, StringList* string_list, bool is_input) override;
		void write(std::fstream& stream, StringList* string_list, bool is_input) override;
		std::map<std::string, std::string>* getExtras() override;
	private:
		std::string m_type_name = "";
	};
}