#include "aircraftwar.hpp"

using namespace AircraftWar;

Enemy::Enemy(
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
	),
	animeTime(0),
	animator(0)
{}

void Enemy::setAnimator(int(*animator)(Enemy*,unsigned),unsigned animeTime){
	this->animator=animator;
	animeTime=animeTime;
	run(0);
}

void Enemy::run(double passTime){
	if(this->animator){
		if(animator(this,animeTime)){
			return;
		}
		animeTime++;
	}
	Aircraft::run(passTime);
}

