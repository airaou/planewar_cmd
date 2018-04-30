#include "aircraftwar.hpp"

using namespace AircraftWar;

Player::Player(
	CmdConio* cmdConio,
	const Plane_s* plane,
	unsigned life,
	double speed,
	unsigned color,
	double sx,
	double sy,
	double dt
)throw(const char*):
	Aircraft(
		cmdConio,
		plane,
		life,
		speed,
		color,
		sx,
		sy,
		dt
	)
{}

void Player::run(double passTime){
	if(CmdObject::runInCommand(passTime)){
		setSpeed(0,0);
		setAcceleration(0,0);
	}
}

