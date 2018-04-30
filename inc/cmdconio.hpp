#ifndef __CMDCONIO_HPP__
#define __CMDCONIO_HPP__

#include "myevent.hpp"

class CmdColor {
	enum Color {
		TXT_BLUE      = 0x01,
		TXT_GREEN     = 0x02,
		TXT_RED       = 0x04,
		TXT_INTENSITY = 0x08,
		BG_BLUE       = 0x40,
		BG_GREEN      = 0x20,
		BG_RED        = 0x10,
		BG_INTENSITY  = 0x80,
	};
};

class CmdConio : public CmdColor {
public:
	virtual bool setCursorPosition(int x,int y);
	virtual void setColor(int color);
	virtual void setDefaultColor(int color);
	virtual int getDefaultColor();
	virtual void setCommandSize(int x,int y);
	virtual void getCommandSize(int&x,int&y);
	virtual bool isInCommand(int x,int y);
	virtual bool putchar(int c);
	virtual void printf(const char*format,...);
	virtual void log(const char*format,...);
	virtual void ClearScreen();
	virtual int kbhit();
	virtual int getch();
	virtual void delay_ms(unsigned t);
	virtual ~CmdConio();
};

class Windows_Cmd : public CmdConio {
public:
	Windows_Cmd();
};

class Ubuntu_Terminal : public CmdConio {
	
};

class EventKbhit : public myEvent::Event {
public:
	int key1;
	int key2;
};

extern EventKbhit* onKbhit;

#endif

