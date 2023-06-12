#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "nin-io/util/util.h"
#include "StringList.h"
#include "ParameterHandler.h"
#include "EntryPointCommand.h"
#include "ExecutionCommand.h"
#include "CommandBody.h"

class CommandSequence
{
public:
	CommandSequence();
	~CommandSequence();

	

private:
	EntryPointCommand* m_entry_point_command = nullptr;
	

};