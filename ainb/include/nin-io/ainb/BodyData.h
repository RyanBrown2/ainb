#pragma once

class BodyData
{
public:
	BodyData(int address);
	~BodyData();
private:
	int m_address;
	int m_size;
};