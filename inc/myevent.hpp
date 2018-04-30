#ifndef __MYEVENT_HPP__
#define __MYEVENT_HPP__

#include <vector>

namespace myEvent {

	class Event;
	
	typedef void(*EventFunc)(void* sender,Event* e);
	
	class Event {
	private:
		struct EventUnit{
			EventFunc func;
			void* sender;
		};
		std::vector<EventUnit> funcList;
	public:
		Event();
		~Event();
		void add(EventFunc,void*sender=0);
		void add(EventFunc*functions,unsigned length,void*senders[]=0);
		void happen();
		void clear();
		void remove(EventFunc,void*sender=0);
		unsigned getQuantity()const;
		int have(EventFunc func);
	};
	
	class EventLoop : public Event {
	public:
		double passTime;
	};
	
	class EventExit : public Event {
	public:
		int exitNum;
	};
	
	extern Event* onLoad;
	extern EventLoop* onLoop;
	extern EventExit* onExit;
	
	int EventModel(EventFunc* onLoadFuncList,unsigned length);
	
}

#endif

