
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
	
#pragma region Observer Pattern
	

	
	class Observer {
		using Sender = entt::entity&;
		using Event = std::string;

	public:
		virtual ~Observer() {};
		virtual void OnNotify(Sender sender, Event event) = 0;
	};


	class EventSource {
		using Sender = entt::entity&;
		using Event = std::string;
	
	private:
		using func = Observer*;

		std::list<func> _subscribers;

	public:
		void SubscribeObserver(Observer* observer);
		void UnSubscribeObserver(Observer* observer);
		
		void Invoke(Sender sender, Event event);

		


	};

#pragma endregion
#pragma region EventQueue Pattern


	class EventQueue {
	public:
		using args = int;
		using Listener = std::function<void(args)>;
		using ListenerID = int;

	private:
		std::map<ListenerID, Listener> _listeners;
		std::stack<ListenerID> _vacantListenerSlots;
		ListenerID _slotCount = 0;

		


		ListenerID GetNextOpenListenerSlot();


	public:
		ListenerID RegisterListener(Listener listener);

		void DeRegisterListener(ListenerID listenerID);


	};


#pragma endregion


}


#endif;