#include "myevent.hpp"
#include "aircraftwar.hpp"

using namespace AircraftWar;

int lftoi(double);

Gun::Gun(
	CmdConio* cmd,
	const Aircraft* source,
	const Bullet_s* bulletTemplate,
	double coolTime
)throw(const char*):
	cmd(cmd),
	source(source),
	bulletTemplate(bulletTemplate),
	coolTime(coolTime),
	cooling(0)
{
	if(!source){
		throw "持枪者不存在"; 
	}
	if(!bulletTemplate){
		throw "缺少子弹模版";
	}
	myEvent::onLoop->add(Gun_cooling,this);
}

Gun::~Gun(){
	myEvent::onLoop->remove(Gun_cooling,this);
}

void Gun::setCoolTime(double coolTime){
	this->coolTime=coolTime;
}

void Gun::setBulletTemplate(const Bullet_s* bulletTemplate){
	this->bulletTemplate=bulletTemplate;
}

void Gun::fire(){
	cmd->log("<gun fire>\n");
	if(this->cooling>0)return;
	unsigned fireX=lftoi(this->source->sx)+this->source->firePointX;
	unsigned fireY=lftoi(this->source->sy)+this->source->firePointY;
	if(cmd->isInCommand(fireX,fireY)){
		Bullet* b=new Bullet(
			cmd,
			this->bulletTemplate,
			this->source->bulletDirectionRedian,
			this->source->color&0x0f,
			this->source
		);
		
		b->display();
		cmd->log("<onLoop:%d,b=%p>\n",myEvent::onLoop->getQuantity(),b);
		
		this->cooling=this->coolTime;
	}
}

void AircraftWar::Gun_cooling(void* sender,myEvent::Event* e){
	Gun* gun=(Gun*)sender;
	myEvent::EventLoop* f=(myEvent::EventLoop*)e;
	if(gun->cooling<=f->passTime){
		gun->cooling=0;
	}else{
		gun->cooling-=f->passTime;
	}
}
