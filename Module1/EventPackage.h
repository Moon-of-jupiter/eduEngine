
#ifndef EventPackage_hpp
#define EventPackage_hpp
#pragma once

#include <entt/entt.hpp>

#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <functional>
#include <map>
#include <algorithm>
#include <queue>
#include <stack>



namespace EventP {
	
	class EventArgs {
	public:
		std::string _event;
		entt::entity _sender;

	};


#pragma region Observer Pattern
	

	
	class Observer {
	public:
		using Sender = entt::entity;
		using Event = EventArgs;

		virtual ~Observer() {};
		virtual void OnNotify(const Event& event) = 0;
	};


	class EventSource {
		using Sender = entt::entity;
		using Event = EventArgs;
	
	private:
		

		std::list<Observer*> _subscribers;

	public:
		void SubscribeObserver(Observer* observer);
		void UnSubscribeObserver(Observer* observer);
		
		void Invoke(Event event);

		


	};

#pragma endregion
#pragma region EventQueue Pattern

	


	class EventQueue {
	public:
		using Sender = entt::entity;
		using Event = EventArgs*;
		using Listener = std::function<void(Event)>;
		using ListenerID = int;

	private:
		std::map<ListenerID, Listener> _listeners;
		std::list<ListenerID> _vacantListenerSlots;
		ListenerID _slotCount = 0;

		std::list<Event> _eventQueue;


		ListenerID GetNextOpenListenerSlot();


		void InvokeEvent(Event event);

		void ClearEvents(std::list<Event>::iterator start, std::list<Event>::iterator end);

	public:
		~EventQueue();

		ListenerID RegisterListener(Listener listener);

		void DeRegisterListener(ListenerID listenerID);

		void EnqueueEvent(Event event);

		void InvokeEvents();

	};

	
	


#pragma endregion


}


#endif;