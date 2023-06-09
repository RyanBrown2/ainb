#pragma once
#include <iostream>
#include <fstream>
#include "nin-io/util/util.h"
#include "BaseCommand.h"

class A_Command : public BaseCommand
{
public:
	A_Command();
	~A_Command();
	void load(std::fstream& file);

	friend std::ostream& operator<<(std::ostream& os, const A_Command block);

private:
	int m_unknown2;
};