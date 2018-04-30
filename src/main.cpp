#include "myevent.hpp"
#include "myevent_onloadfunclist.hpp"
#include "cmdobject.hpp"
#include "aircraftwar.hpp"

int main(int argc,char** argv) {
	CmdConio* cmd=new Windows_Cmd();
	AircraftWar::Aircraftwar*aw;
	try{
		aw=new AircraftWar::Aircraftwar(cmd);
		aw->run();
	}catch(const char*str){
		cmd->printf(str);
	}
	delete aw;
	delete cmd;
	return 0;
}
