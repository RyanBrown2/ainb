#pragma once
#include <iostream>
#include <fstream>
#include "nin-io/util/util.h"
#include "DataBlock.h"
#include "A_Block.h"
#include "B_Block.h"

class AINB
{
public:
	AINB();
	~AINB();
	void load(std::fstream& file);
private:
};