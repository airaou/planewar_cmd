#ifndef __AIRCRAFTWAR_HPP__
#define __AIRCRAFTWAR_HPP__

#include <vector>
#include "myevent.hpp"
#include "cmdobject.hpp"
#include "cmdconio.hpp"

namespace AircraftWar {
	
	class Mateial;
	class Bullet;
	class Gun;
	class Aircraft;
	class Player;
	class Enemy;
	class Aircraftwar;
	class EventBulletHit;
	
	class Mateial {
	protected:
		struct Plane_s {
			unsigned width;
			unsigned height;
			unsigned firePointX;
			unsigned firePointY;
			unsigned fireLevel;
			double bulletDirectionRedian;
			double bulletSpeed;
			const char* pic;
		};
		struct Bullet_s {
			unsigned width;
			unsigned height;
			unsigned power;
			unsigned firePointX;
			unsigned firePointY;
			unsigned speed;
			const char* pic;
		};
	public:
		static Plane_s planes[];
		static Bullet_s bullets[];
	};
	
	class Bullet : public CmdObject, public Mateial {
	private:
		
	public:
		Bullet(
			CmdConio* cmd,
			const Bullet_s* bullet,
			double redian,
			unsigned color,
			const Aircraft* source
		)throw(const char*);
		~Bullet();
		unsigned fireLevel;
		const CmdObject* source;
		void setSource(CmdObject* source);
		const CmdObject* getSource();
		void setSpeed(double speed);
		void setDirectionRedian(double redian);
		void setDirectionDegree(double degree);
		void run(double passTime);
	};

	class Gun : public Mateial {
	private:
		CmdConio* cmd;
		const Aircraft* source;
		const Bullet_s* bulletTemplate;
		double cooling;
		double coolTime;
	public:
		Gun(
			CmdConio* cmd,
			const Aircraft* source,
			const Bullet_s* bulletTemple,
			double coolTime
		)throw(const char*);
		~Gun();
		void setCoolTime(double coolTime);
		void setBulletTemplate(const Bullet_s* bulletTemplate);
		void fire();
		friend void Gun_cooling(void* sender,myEvent::Event* e);
	};
	void Gun_cooling(void* sender,myEvent::Event* e);

	class Aircraft : public CmdObject, public Mateial {
	private:
		Gun* gun;
	public:
		Aircraft(
			CmdConio* cmdConio,
			const Plane_s* plane,
			unsigned life,
			double speed=1,
			unsigned color=0x07,
			double sx=0,
			double sy=0,
			double dt=0.01
		)throw(const char*);
		~Aircraft();
		
		unsigned life;
		double bulletDirectionRedian;
		double speed;
		unsigned firePointX;
		unsigned firePointY;
		unsigned score;
	
		void setSpeedByStr(const char* ctrlStr);
		void setGun(const Bullet_s* bulletTemple);
		void clearGun();
		void stop();
		void run(double passTime);
		void hit(unsigned injury);
		friend void Aircraft_run(void*sender,myEvent::Event* e);
		void fire();
	};
	void Aircraft_hit(void*sender,myEvent::Event* e);

	class Player : public Aircraft {
	public:
		Player(
			CmdConio* cmdConio,
			const Plane_s* plane,
			unsigned life,
			double speed=1,
			unsigned color=0x07,
			double sx=0,
			double sy=0,
			double dt=0.01
		)throw(const char*);
		void run(double passTime);
	};
	
	class Enemy : public Aircraft {
	protected:
		int(*animator)(Enemy* self,unsigned time);
		unsigned animeTime;
	public:
		Enemy(
			CmdConio* cmdConio,
			const Plane_s* plane,
			unsigned life,
			double speed=1,
			unsigned color=0x07,
			double sx=0,
			double sy=0,
			double dt=0.01
		)throw(const char*);
		void setAnimator(int(*animator)(Enemy*,unsigned),unsigned animeTime=0);
		void run(double passTime);
	};
	
	class Aircraftwar {
	private:
		CmdConio* cmd;
		std::vector<Player*> playerList;
	public:
		Aircraftwar(CmdConio* cmd)throw(const char*);
		~Aircraftwar()throw(const char*);
		
		unsigned getPlayerQuantity();
		
		friend void Kbhit(void* sender,myEvent::Event* e);
		friend void addEnemy(void*sender,myEvent::Event*e);
		friend void over(void* sender,myEvent::Event* e);
		
		run();
	};
	
	class EventBulletHit : public myEvent::Event {
	public:
		Bullet* bullet;
	};
	
	extern EventBulletHit* onBulletHit;
	extern myEvent::Event* onOver;
	
	void Kbhit(void* sender,myEvent::Event* e);
	void addEnemy(void*sender,myEvent::Event*e);
	void over(void*sender,myEvent::Event*e);
	
}

#endif

