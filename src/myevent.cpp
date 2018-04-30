#include <cstdlib>
#include <csetjmp>
#include <csignal>
#include <windows.h>
#include "myevent_onloadfunclist.hpp"
#include "myevent.hpp"

using namespace myEvent;

jmp_buf exitjmp;

Event __onLoad;
Event* myEvent::onLoad=&__onLoad;
EventLoop __onLoop;
EventLoop* myEvent::onLoop=&__onLoop;
EventExit __onExit;
EventExit* myEvent::onExit=&__onExit;

Event::Event():funcList(0){
	
}

Event::~Event(){
	
}

void Event::add(EventFunc func,void*sender){
	EventUnit eu={func,sender};
	funcList.push_back(eu);
}

void Event::add(EventFunc* funcarr,unsigned length,void*senders[]){
	if(!funcarr)return;
	for(unsigned i=0;i<length;i++){
		EventUnit eu={funcarr[i],senders?senders[i]:0};
		funcList.push_back(eu);
	}
}

void Event::happen(){
	for(unsigned i=0;i<funcList.size();i++){
		if(funcList[i].func){
			funcList[i].func(funcList[i].sender,this);
		}
	}
	if(this==onExit){
		longjmp(exitjmp,onExit->exitNum);
	}
}

void Event::clear(){
	funcList.clear();
}

void Event::remove(EventFunc func,void*sender){
	for(unsigned i=0;i<funcList.size();i++){
		if(funcList[i].func==func){
			if(sender){
				if(sender==funcList[i].sender){
					funcList.erase(funcList.begin()+i);
				}
			}else{
				funcList.erase(funcList.begin()+i);
			}
		}
	}
}

unsigned Event::getQuantity()const {
	return funcList.size();
}

int Event::have(EventFunc func){
	unsigned quantity=0;
	for(unsigned i=0;i<funcList.size();i++){
		if(funcList[i].func==func){
			quantity++;
		}
	}
	return quantity;
}

int myEvent::EventModel(EventFunc* onLoadFuncList,unsigned length){
	int rtnnum;
	if((rtnnum=setjmp(exitjmp))==0){
		onLoad->add(onLoadFuncList,length);
		onLoad->happen();
		while(1){
			onLoop->passTime=0.001;
			onLoop->happen();
			Sleep(1);
		}
	}else{
		return rtnnum;
	}
}

