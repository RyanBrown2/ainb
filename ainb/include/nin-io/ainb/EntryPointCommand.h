#pragma once
#include <iostream>
#include <fstream>
#include "nin-io/util/util.h"
#include "BaseCommand.h"

class EntryPointCommand : public BaseCommand
{
public:
	EntryPointCommand();
	~EntryPointCommand();
	void load(std::fstream& file);

	friend std::ostream& operator<<(std::ostream& os, const EntryPointCommand block);

private:
	int m_unknown2;
};