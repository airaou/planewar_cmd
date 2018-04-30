#include <cmath>
#include "aircraftwar.hpp"

using namespace AircraftWar;

Mateial::Plane_s Mateial::planes[]={{
		5,5,2,0,1,M_PI*3/2,7,
		"  *  "
		" *** "
		"*****"
		"  *  "
		" /|\\ "
	},{
		5,5,2,5,1,M_PI*1/2,7,
		" \\|/ "
		"  *  "
		"*****"
		" *** "
		"  *  "
	}
};

Mateial::Bullet_s Mateial::bullets[]={
	1,1,1,0,0,5,
	"*",
	5,2,3,2,0,3,
	" /-\\ "
	"|   |",
};

