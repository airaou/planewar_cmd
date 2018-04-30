#ifndef __CMDOBJECT_H__
#define __CMDOBJECT_H__

#include "cmdconio.hpp"
#include "myevent.hpp"

class CmdObject : public CmdColor{
public:
	bool visible;
	
	double sx;
	double sy;
	double vx;
	double vy;
	double ax;
	double ay;
	double dt;
	
	unsigned width;
	unsigned height;
	char*pic;
	unsigned color;
	
	CmdConio* cmdConio;
public:
	CmdObject(
		CmdConio* cmdConio,
		unsigned width=2,
		unsigned height=2,
		const char*pic="****",
		unsigned color=0x07,
		double sx=0.0,
		double sy=0.0,
		double dt=0.01
	)throw(const char*);
	~CmdObject()throw(const char*);
	void setWidth(unsigned width,const char*newpic=0,unsigned newwidth=0);
	void setHeight(unsigned height,const char*newpic=0,unsigned newheight=0);
	void setPosition(double sx,double sy);
	void setSpeed(double vx,double vy);
	void setAcceleration(double ax,double ay);
	void setPic(const char*pic,unsigned width=0,unsigned height=0);
	void setColor(unsigned color);
	
	unsigned getWidth()const;
	unsigned getHeight()const;
	void getPosition(double &sx,double &sy)const;
	void getPositionOfCommand(int& posx,int& posy)const;
	void getSpeed(double &vx,double &vy)const;
	void getAcceleration(double &ax,double &ay)const;
	char* getPic()const;
	
	void hide();
	void display();
	virtual void run(double passTime);
	int runInCommand(double passTime);
	friend void CmdObject_run(void* sender,myEvent::Event* e);
	friend void CmdObject_refresh(void* sender,myEvent::Event* e);
	
	bool isInCommandFully();
	bool isSimpleOverlap(const char*pic,unsigned left,unsigned top,unsigned width,unsigned height);
	bool isSimpleOverlap(CmdObject* other);
	bool isOverlap(const char*pic,unsigned left,unsigned top,unsigned width,unsigned height);
	bool isOverlap(CmdObject* other);
	
	static bool isBody(int ch);
};

void CmdObject_run    (void* sender,myEvent::Event* e);
void CmdObject_refresh(void* sender,myEvent::Event* e);

/**
  * 物体隐藏时发出
  * pic为事件发出者的原贴图,所有与原贴图有相交的对象都要重绘
  * 被调用的事件处理函数不应修改事件的属性
	*/
class EventHide : public myEvent::Event {
public:
	const char* pic;
	unsigned width;
	unsigned height;
	unsigned left;
	unsigned top;
};

extern EventHide* onHide;

#endif

