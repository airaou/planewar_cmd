#include <cmath>
#include <cstdlib>
#include <vector>
#include "myevent_onloadfunclist.hpp"
#include "myevent.hpp"
#include "cmdobject.hpp"
#include "cmdconio.hpp"
#include "aircraftwar.hpp"

using namespace AircraftWar;


myEvent::Event __onOver;
myEvent::Event* AircraftWar::onOver=&__onOver;

int testAnime1(Enemy* obj,unsigned time){
	if(time>2000){
		delete obj;
		return 1;
	}else if(time==100){
		obj->setSpeed(0,3);
	}else if(time==0){
	}
	if(time%200==0)obj->fire();
	return 0;
}

int testAnime2(Enemy* obj,unsigned time){
	if(time==0){
		obj->setSpeed(-4,3);
	}
	if(time>2000){
		delete obj;
		return 1;
	}else if(time%500==0){
		if(time/500%2){
			obj->setAcceleration(-2,0);
		}else{
			obj->setAcceleration(2,0);
		}
	}
	if(time%200==0)obj->fire();
	return 0;
}

void AircraftWar::Kbhit(void* sender,myEvent::Event* e){
	Aircraftwar* obj=(Aircraftwar*)sender;
	EventKbhit* f=(EventKbhit*)e;
	char ctrlStr[]=" ";
	switch(f->key1){
	case 'w':
	case 'a':
	case 's':
	case 'd':
		ctrlStr[0]=f->key1;
		obj->playerList[0]->setSpeedByStr(ctrlStr);
		break;
	case 'z':
		obj->playerList[0]->stop();
		break;
	case 'q':
		obj->playerList[0]->fire();
		break;
	case '8':
	case '4':
	case '5':
	case '6':
		ctrlStr[0]="asd w"[f->key1-'4'];
		obj->playerList[1]->setSpeedByStr(ctrlStr);
		break;
	case '1':
		obj->playerList[1]->stop();
		break;
	case '7':
		obj->playerList[1]->fire();
		break;
	}
}

void AircraftWar::addEnemy(void* sender,myEvent::Event* e){
	Aircraftwar* aw=(Aircraftwar*)sender;
	static unsigned time;
	if(time%300==0){
		
		if(aw->getPlayerQuantity()==0){
			myEvent::onLoop->remove(addEnemy);
			onOver->happen();
		}
		
		static unsigned i;
		Enemy* em;
		em=new Enemy(aw->cmd,&Aircraft::planes[1],6,5,0x47,rand()%70+5,-(rand()%10));
		em->bulletDirectionRedian=M_PI/2;
		em->setGun(&Bullet::bullets[0]);
		int (*const animes[])(Enemy*,unsigned)={
			testAnime1,
			testAnime2,
		};
		em->setAnimator(animes[i%2]);
		em->display();
		i++;
	}
	time++;
}

void AircraftWar::over(void*sender,myEvent::Event*e){
	Aircraftwar*aw=(Aircraftwar*)sender;
	int scrWidth;
	int scrHeight;
	aw->cmd->getCommandSize(scrWidth,scrHeight);
	aw->cmd->log("<over>\n");
	CmdObject* obj=new CmdObject(aw->cmd,9,1,"Game_Over",0x3f,scrWidth/2-9/2,scrHeight/2-1/2);
	obj->display();
}

Aircraftwar::Aircraftwar(CmdConio* cmd)throw(const char*):
	playerList(0)
{
	this->cmd=cmd;
	if(!cmd){
		throw "控制台指针不应为空";
	}
	this->cmd->setCommandSize(80,50);
	onKbhit->add(Kbhit,this);
	for(unsigned i=0;i<1;i++){
		Player* ac;
		ac=new Player(cmd,&Aircraft::planes[i],3,5,"\x3f\x0e"[i],10,10+7*i);
		ac->setGun(&Bullet::bullets[1]);
		ac->display();
		playerList.push_back(ac);
	}
	myEvent::onLoop->add(Kbhit,this);
	myEvent::onLoop->add(addEnemy,this);
	onOver->add(over,this);
}

Aircraftwar::~Aircraftwar()throw(const char*){
	for(std::vector<Player*>::iterator i=playerList.begin();i!=playerList.end();i++){
		if(*i){
			delete *i;
		}else{
			throw "对象内存泄漏"; 
		}
	}
}

unsigned Aircraftwar::getPlayerQuantity(){
	unsigned quan=0;
	for(std::vector<Player*>::iterator i=playerList.begin();i!=playerList.end();i++){
		if((*i)->visible){
			quan++;
		}
	}
	return quan;
}

#define ONLOADFUNC(__func) void __func (void*,myEvent::Event* e);
ONLOADFUNCS
#undef ONLOADFUNC
Aircraftwar::run(){
	myEvent::EventFunc funcArr[]={
#define ONLOADFUNC(__func) __func,
		ONLOADFUNCS
#undef ONLOADFUNC
	};
	myEvent::EventModel(funcArr,sizeof(funcArr)/sizeof(myEvent::EventFunc));
}

