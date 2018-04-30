#include <stdio.h>
#include <windows.h>
#include <conio.h>
#include "cmdconio.hpp"

EventKbhit __onKbhit;
EventKbhit* onKbhit=&__onKbhit;

static HANDLE hout;
int defaultColor=0x07;
int cursorX=0;
int cursorY=0;
int cmdSizeX=80;
int cmdSizeY=25;

void check_kbhit(void* sender,myEvent::Event* e){
	if(kbhit()){
		onKbhit->key1=getch();
		onKbhit->key2=kbhit()?getch():0;
		onKbhit->happen();
	}
}

void cmdConio_init(void* sender,myEvent::Event* e){
	myEvent::onLoop->add(check_kbhit);
}

Windows_Cmd::Windows_Cmd(){
	hout=GetStdHandle(STD_OUTPUT_HANDLE);
}

bool Windows_Cmd::CmdConio::setCursorPosition(int x,int y){
	if(isInCommand(x,y)){
		COORD pos={cursorX=x,cursorY=y};
		SetConsoleCursorPosition(hout,pos);
		return 0;
	}else{
		cursorX=-1;
		cursorY=-1;
		return 1;
	}
}

void Windows_Cmd::CmdConio::setColor(int color){
	SetConsoleTextAttribute(hout,color);
}

void Windows_Cmd::CmdConio::setDefaultColor(int color){
	defaultColor=color;
}

int Windows_Cmd::CmdConio::getDefaultColor(){
	return defaultColor;
}

void Windows_Cmd::CmdConio::setCommandSize(int x,int y){
	char str[30];
	cmdSizeX=x;
	cmdSizeY=y;
	sprintf(str,"mode con cols=%d lines=%d",x,y);
	system(str);
}

void Windows_Cmd::CmdConio::getCommandSize(int&x,int&y){
	x=cmdSizeX;
	y=cmdSizeY;
}

bool Windows_Cmd::CmdConio::isInCommand(int x,int y){
	return (x<cmdSizeX)&&(y<cmdSizeY)&&(x>=0)&&(y>=0)&&(!(x==cmdSizeX-1&&y==cmdSizeY-1));
}

bool Windows_Cmd::CmdConio::putchar(int c){
	if(cursorX==-1||cursorY==-1){
		return 1;
	}
	if(cursorX>=cmdSizeX){
		cursorX=-1;
	}
	if(cursorX==cmdSizeX-1&&cursorY==cmdSizeY-1){
		cursorX=-1;
		cursorY=-1;
		return 1;
	}
	cursorX++;
	::putchar(c);
	return 0;
}

void Windows_Cmd::CmdConio::printf(const char*format,...){
	va_list vp;
	va_start(vp,format);
	cursorX+=vprintf(format,vp);
	cursorY+=cursorX/cmdSizeX;
	cursorX%=cmdSizeX;
	va_end(vp);
}

static FILE* logfile=0;

void Windows_Cmd::CmdConio::log(const char*format,...){
	if(!logfile){
		logfile=fopen("log.txt","w");
	}
	va_list vp;
	va_start(vp,format);
	vfprintf(logfile,format,vp);
	va_end(vp);
}

void Windows_Cmd::CmdConio::ClearScreen(){
	setColor(defaultColor);
	system("cls");
}

int Windows_Cmd::CmdConio::kbhit(){
	return ::kbhit();
}

int Windows_Cmd::CmdConio::getch(){
	if(cursorX==cmdSizeX-1&&cursorY==cmdSizeY-1){
		return 0;
	}else{
		cursorX++;
		return ::getch();
	}
}

void Windows_Cmd::CmdConio::delay_ms(unsigned time){
	Sleep(time);
}

Windows_Cmd::CmdConio::~CmdConio(){
	if(logfile){
		fclose(logfile);
		logfile=0;
	}
}

