#include "ainb/ainb.h"

using namespace std;

using namespace ainb;

AINB::AINB(LPSTREAM stream) {
	STATSTG stat_info;
	stream->Stat(&stat_info, STATFLAG_NONAME);

	char buff[4];
	stream->Read(buff, 3, NULL);
	buff[3] = '\0';
	cout << stat_info.cbSize.LowPart << endl;
	cout << buff << endl;
}

AINB::~AINB() {

}

AINB* CreateAINB(LPSTREAM stream) {
	return new AINB(stream);
}

void DestroyAINB(AINB* ainb) {
	delete ainb;
}
