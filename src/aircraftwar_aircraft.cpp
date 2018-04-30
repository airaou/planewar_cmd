#include <cmath.>
#include "myevent.hpp"
#include "cmdobject.hpp"
#include "aircraftwar.hpp"

using namespace AircraftWar;

int lftoi(double lf);

void AircraftWar::Aircraft_hit(void*sender,myEvent::Event* e){
	Aircraft* aircraft=(Aircraft*)sender;
	EventBulletHit* f=(EventBulletHit*)e;
	if(!f->bullet)return;
	if(!aircraft->visible)return;
	if(f->bullet->source==aircraft)return;
	if(f->bullet->isOverlap(aircraft)){
		aircraft->hit(f->bullet->fireLevel);
		aircraft->cmdConio->log("<bullet hit aircraft>\n");
		delete f->bullet;
		f->bullet=0;
	}
}

Aircraft::Aircraft(
	CmdConio* cmdConio,
	const Plane_s* plane,
	unsigned life,
	double speed,
	unsigned color,
	double sx,
	double sy,
	double dt
)throw(const char*):
	CmdObject(
		cmdConio,
		plane->width,
		plane->height,
		plane->pic,
		color,
		sx,
		sy,
		dt
	),
	life(life),
	bulletDirectionRedian(plane->bulletDirectionRedian),
	firePointX(plane->firePointX),
	firePointY(plane->firePointY),
	speed(speed),
	score(0),
	gun(0)
{
	onBulletHit->add(Aircraft_hit,this);
}

Aircraft::~Aircraft(){
	if(gun)delete gun;
	onBulletHit->remove(Aircraft_hit,this);
}

void Aircraft::setSpeedByStr(const char*ctrlStr){
	double vx=0;
	double vy=0;
	for(;*ctrlStr;ctrlStr++){
		switch(*ctrlStr){
		case 'w':
			vy-=this->speed;
			break;
		case 'a':
			vx-=this->speed;
			break;
		case 's':
			vy+=this->speed;
			break;
		case 'd':
			vx+=this->speed;
			break;
		}
	}
	this->setSpeed(vx,vy);
}

void Aircraft::setGun(const Bullet_s* bulletTemple){
	if(gun){
		delete gun;
	}
	gun=new Gun(cmdConio,this,bulletTemple,0.1);
}

void Aircraft::stop(){
	this->setSpeed(0,0);
	this->setAcceleration(0,0);
}

void Aircraft::run(double passTime){
	CmdObject::run(passTime);
}

void Aircraft::hit(unsigned injury){
	if(this->life>injury){
		this->life-=injury;
	}else{
		this->life=0;
		this->hide();
	}
}

void Aircraft::fire(){
	if(this->visible&&gun){
		gun->fire();
	}
}

