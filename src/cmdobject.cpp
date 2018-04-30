#include "cmdobject.hpp"
#include <cmath>

EventHide __onHide;
EventHide* onHide=&__onHide;

int lftoi(double lf){
	lf=(int)(lf*100)/100.0;
	if(lf>=0||(int)lf-lf==0)return (int)lf;
	return (int)(lf-1);
}

CmdObject::CmdObject(
	CmdConio* cmdConio,
	unsigned width,
	unsigned height,
	const char*pic,
	unsigned color,
	double sx,
	double sy,
	double dt)
	throw(const char*):
	visible(0),
	cmdConio(cmdConio),
	width(width),
	height(height),
	sx(sx),
	sy(sy),
	vx(0),
	vy(0),
	ax(0),
	ay(0),
	color(color),
	dt(dt)
{
	if(!cmdConio){
		throw "缺少命令行对象"; 
	}
	if(dt<=0){
		throw "时间间隔为负或零"; 
	}
	if(!pic){
		throw "贴图异常"; 
	}
	if((color&0x0f)==((color>>4)&0x0f)){
		throw "不可见"; 
	}
	this->pic=new char[width*height];
	for(unsigned i=0;i<width*height;i++){
		this->pic[i]=pic[i];
	}
	onHide->add(CmdObject_refresh,this);
	myEvent::onLoop->add(CmdObject_run,this);
}

CmdObject::~CmdObject()throw(const char*){
	if(!pic){
		throw "pic内存泄漏"; 
	}
	if(this->visible){
		this->hide();
	}
	delete[] this->pic;
	onHide->remove(CmdObject_refresh,this);
	myEvent::onLoop->remove(CmdObject_run,this);
}

void CmdObject::setWidth(unsigned width,const char*newpic,unsigned newwidth){
	bool visible;
	if(visible=this->visible){
		this->hide();
	}
	char*oldpic=this->pic;
	char oldWidth=this->width;
	this->width=width;
	this->pic=new char[this->width*this->height];
	if(newpic){
		if(newwidth==0){
			newwidth=oldWidth;
		}
		for(unsigned x=0;x<this->width;x++){
			for(unsigned y=0;y<this->height;y++){
				if(x<newwidth){
					this->pic[y*this->width+x]=newpic[y*newwidth+x];
				}else{
					this->pic[y*this->width+x]=' ';
				}
			}
		}
	}else{
		for(unsigned x=0;x<this->width;x++){
			for(unsigned y=0;y<this->height;y++){
				if(x<oldWidth){
					this->pic[y*this->width+x]=oldpic[y*oldWidth+x];
				}else{
					this->pic[y*this->width+x]=' ';
				}
			}
		}
	}
	delete[] oldpic;
	if(visible){
		this->display();
	}
}

void CmdObject::setHeight(unsigned height,const char*newpic,unsigned newheight){
	
}

void CmdObject::setPosition(double sx,double sy){
	bool visible;
	if(visible=this->visible){
		this->hide();
	}
	this->sx=sx;
	this->sy=sy;
	if(visible){
		this->display();
	}
}

void CmdObject::setSpeed(double vx,double vy){
	this->vx=vx;
	this->vy=vy;
}

void CmdObject::setAcceleration(double ax,double ay){
	this->ax=ax;
	this->ay=ay;
}

void CmdObject::setPic(const char*pic,unsigned newwidth,unsigned newheight){
	bool visible;
	char* oldpic=this->pic;
	if(visible=this->visible){
		this->hide();
	}
	if(newwidth!=0){
		this->width=newwidth;
	}
	if(newheight!=0){
		this->height=newheight;
	}
	
	this->pic=new char[this->width*this->height];
	for(unsigned i=0;i<this->width*this->height;i++){
		this->pic[i]=pic[i];
	}
	
	delete[] oldpic;
	if(visible){
		this->display();
	}
}

unsigned CmdObject::getWidth()const{
	return this->width;
}

unsigned CmdObject::getHeight()const{
	return this->height;
}

void CmdObject::getPosition(double &sx,double &sy)const{
	sx=this->sx;
	sy=this->sy;
}

void CmdObject::getPositionOfCommand(int& posx,int& posy)const{
	posx=lftoi(this->sx);
	posy=lftoi(this->sy);
}

void CmdObject::getSpeed(double &vx,double &vy)const{
	vx=this->vx;
	vy=this->vy;
}

void CmdObject::getAcceleration(double &ax,double &ay)const{
	ax=this->ax;
	ay=this->ay;
}

char* CmdObject::getPic()const{
	return this->pic;
}

void CmdObject::hide(){
	bool havePutchar=0;
	this->visible=0;
	cmdConio->setColor(cmdConio->getDefaultColor());
	int baseX;
	int baseY;
	getPositionOfCommand(baseX,baseY);
	for(unsigned y=0;y<this->height;y++){
		havePutchar=0;
		if((y+baseY)<0)continue;
		for(unsigned x=0;x<this->width;x++){
			if(isBody(this->pic[x+y*this->width])){
				if(havePutchar==0){
					if(this->cmdConio->setCursorPosition(baseX+x,baseY+y)){
						//光标定位失败
						havePutchar=0;
						continue;
					}
				}
				this->cmdConio->putchar(' ');
				havePutchar=1;
			}else{
				havePutchar=0;
			}
		}
	}
	onHide->height=this->height;
	onHide->width=this->width;
	onHide->left=this->sx;
	onHide->top=this->sy;
	onHide->pic=this->pic;
	onHide->happen();
}

void CmdObject::display(){
	bool havePutchar=0;
	this->visible=1;
	cmdConio->setColor(color);
	int baseX;
	int baseY;
	getPositionOfCommand(baseX,baseY);
	for(unsigned y=0;y<this->height;y++){
		havePutchar=0;
		if((baseY+y)<0)continue;
		for(unsigned x=0;x<this->width;x++){
			int c;
			if(isBody(c=this->pic[x+y*this->width])){
				if(havePutchar==0){
					if(this->cmdConio->setCursorPosition(baseX+x,baseY+y)){
						//定位光标失败
						havePutchar=0;
						continue; 
					}
				}
				this->cmdConio->putchar(c);
				havePutchar=1;
			}else{
				havePutchar=0;
			}
		}
	}
}

void CmdObject::run(double passTime){
	double oldX;
	double oldY;
	this->getPosition(oldX,oldY);
	for(unsigned times=0;passTime>(times*this->dt);times++){
		this->vx+=this->ax*this->dt;
		this->vy+=this->ay*this->dt;
		this->sx+=this->vx*this->dt;
		this->sy+=this->vy*this->dt;
	}
	if(this->visible){
		if(lftoi(oldX)!=lftoi(this->sx)||lftoi(oldY)!=lftoi(this->sy)){
			double newX,newY;
			this->getPosition(newX,newY);
			this->sx=oldX;
			this->sy=oldY;
			this->setPosition(newX,newY);
		}
	}
}

int CmdObject::runInCommand(double passTime){
	double oldX;
	double oldY;
	double oldvx;
	double oldvy;
	double oldax;
	double olday;
	this->getPosition(oldX,oldY);
	this->getSpeed(oldvx,oldvy);
	this->getAcceleration(oldax,olday);
	for(unsigned times=0;passTime>(times*this->dt);times++){
		this->vx+=this->ax*this->dt;
		this->vy+=this->ay*this->dt;
		this->sx+=this->vx*this->dt;
		this->sy+=this->vy*this->dt;
	}
	if(
		cmdConio->isInCommand(lftoi(this->sx),lftoi(this->sy))&&
		cmdConio->isInCommand(lftoi(this->sx)+this->width-1,lftoi(this->sy)+this->height-1)
	){
		if(this->sx<0)this->sx=0;
		if(this->sy<0)this->sy=0;
		if(this->visible){
			if(lftoi(oldX)!=lftoi(this->sx)||lftoi(oldY)!=lftoi(this->sy)){
				double newX,newY;
				this->getPosition(newX,newY);
				this->sx=oldX;
				this->sy=oldY;
				this->setPosition(newX,newY);
			}
		}
		return 0;
	}else{
		this->ax=oldax;
		this->ay=olday;
		this->vx=oldvx;
		this->vy=oldvy;
		this->sx=oldX;
		this->sy=oldY;
		return 1;
	}
}

void CmdObject_run(void*sender,myEvent::Event* e){
	CmdObject* self=(CmdObject*)sender;
	myEvent::EventLoop* f=(myEvent::EventLoop*)e;
	self->run(f->passTime);
}

void CmdObject_refresh(void* sender,myEvent::Event* e){
	CmdObject* obj=(CmdObject*)sender;
	EventHide* f=(EventHide*)e;
	if(obj->pic==f->pic)return;
	if(obj->visible){
		if(obj->isOverlap(f->pic,f->left,f->top,f->width,f->height)){
			obj->display();
		}
	}	
}

bool CmdObject::isInCommandFully(){
	return cmdConio->isInCommand(lftoi(this->sx),lftoi(this->sy))&&
	cmdConio->isInCommand(lftoi(this->sx)+this->width-1,lftoi(this->sy)+this->height-1);
}

bool CmdObject::isSimpleOverlap(const char*pic,unsigned left,unsigned top,unsigned width,unsigned height){
	int tx=this->sx;
	int ty=this->sy;
	return
		tx  +this->width >left&&
		left+width       >tx  &&
		ty  +this->height>top &&
		top +height      >ty;
}

bool CmdObject::isSimpleOverlap(CmdObject* other){
	int tx=this->sx;
	int ty=this->sy;
	int ox=other->sx;
	int oy=other->sy;
	return
		tx+this ->width >ox&&
		ox+other->width >sx&&
		ty+this ->height>oy&&
		oy+other->height>sy;
}

static int min(int a,int b){
	if(a>b)return b;
	return a;
}

static int max(int a,int b){
	if(a>b)return a;
	return b;
}

bool CmdObject::isOverlap(const char*pic,unsigned left,unsigned top,unsigned width,unsigned height){
	if(!isSimpleOverlap(pic,left,top,width,height)){
		return 0;
	}
	int ox=left-(int)this->sx;
	int oy=top -(int)this->sy;
	int mx=width+ox;
	int my=height+oy;
	for(unsigned y=max(oy,0);y<min(my,this->height);y++){
		for(unsigned x=max(ox,0);x<min(mx,this->width);x++){
			//cmdConio->log("<(%d,%d)%c,%c>\n",x,y,this->pic[x+y*this->width],pic[(x-ox)+(y-oy)*width]);
			if(isBody(this->pic[x+y*this->width])){
				if(isBody(pic[(x-ox)+(y-oy)*width])){
					//cmdConio->log("<1>\n");
					return 1;
				}
			}
		}
	}
	return 0;
}

bool CmdObject::isOverlap(CmdObject* other){
	if(!isSimpleOverlap(other)){
		return 0;
	}
	int ox=((int)other->sx)-(int)this->sx;
	int oy=((int)other->sy)-(int)this->sy;
	int mx=other->width+ox;
	int my=other->height+oy;
	cmdConio->log("<isOverlap: '%s' '%s'>\n",this->pic,other->pic);
	for(unsigned y=max(oy,0);y<min(my,this->height);y++){
		for(unsigned x=max(ox,0);x<min(mx,this->width);x++){
			if(isBody(this->pic[x+y*this->width])){
				if(isBody(other->pic[(x-ox)+(y-oy)*other->width])){
					cmdConio->log("<res:1>\n");
					return 1;
				}
			}
		}
	}
	cmdConio->log("<res:0>\n");
	return 0;
}

bool CmdObject::isBody(int ch){
	bool isgraph[]={
		 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		 0,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,
		 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,16,16,16,16,16,16,
		16, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,16,16,16,16,16,
		16, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
		 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,16,16,16,16, 0,
	};
	if(ch>=128){
		return 0;
	}else{
		return isgraph[ch];
	}
}

