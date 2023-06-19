#include "ainb_wrapper.h"

using namespace std;

AINB_FILE::AINB_FILE()
{
	m_ainb = new AINB();
	m_name = nullptr;
}

AINB_FILE::~AINB_FILE()
{
}

void AINB_FILE::loadFile(string file_dir)
{
	fstream file;

	file.open(file_dir, fstream::in | fstream::out | std::ios::binary);

	if (file.fail()) {
		std::cout << "Failed to open file" << std::endl;
		return;
	}


	m_ainb->load(file);

	CComBSTR temp(m_ainb->getName().c_str());
	m_name = temp.Detach();

	//m_name = SysAllocString((m_ainb->getName().c_str()));

	file.close();
}

BSTR AINB_FILE::getName()
{
	return m_name;
}
