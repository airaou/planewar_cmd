#include <cmath>
#include "cmdobject.hpp"
#include "myevent.hpp"
#include "aircraftwar.hpp"

using namespace AircraftWar;

EventBulletHit __onBulletHit;
EventBulletHit*  AircraftWar::onBulletHit=&__onBulletHit;

int lftoi(double lf);

Bullet::Bullet(
	CmdConio* cmd,
	const Bullet_s* bullet,
	double redian,
	unsigned color,
	const Aircraft* source
)throw(const char*):
	CmdObject(
		cmd,
		bullet->width,
		bullet->height,
		bullet->pic,
		color,
		lftoi(source->sx)+source->firePointX-bullet->firePointX,
		lftoi(source->sy)+source->firePointY-bullet->firePointY,
		source->dt
	),
	fireLevel(bullet->power),
	source(source)
{
	CmdObject::setSpeed(cos(redian)*bullet->speed,sin(redian)*bullet->speed);
}

Bullet::~Bullet(){
	cmdConio->log("<bullet delete addr=%p>\n",this);
}

void Bullet::setSpeed(double speed){
	double oldSpeed=sqrt(this->vx*this->vx+this->vy*this->vy);
	if(oldSpeed<0.0001){
		oldSpeed=1;
	}
	CmdObject::setSpeed(this->vx*speed/oldSpeed,this->vy*speed/oldSpeed);
}

void Bullet::setDirectionRedian(double redian){
	double speed=sqrt(this->vx*this->vx+this->vy*this->vy);
	CmdObject::setSpeed(cos(redian)*speed,sin(redian)*speed);
}

void Bullet::setDirectionDegree(double degree){
	setDirectionRedian(degree*M_PI/180);
}

void Bullet::run(double passTime){
	if(this->runInCommand(passTime)){
		delete this;
	}else{
		onBulletHit->bullet=this;
		onBulletHit->happen();
	}
}

