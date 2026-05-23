#include "EventPackage.h"

namespace EventP {
#pragma region Observer Pattern

	void EventSource::SubscribeObserver(Observer* observer) {
		_subscribers.emplace_back(observer);
	}
	void EventSource::UnSubscribeObserver(Observer* observer) {
		_subscribers.remove(observer);
	}

	void EventSource::Invoke(Event event) {
		auto end = _subscribers.end();
		
		for (auto it = _subscribers.begin(); it != end;) {
			auto current = *it;
			it++;

			current->OnNotify(event);
		}

	}



#pragma endregion
#pragma region EventQueue Pattern
	using ListenerID = EventQueue::ListenerID;
	using Listener = EventQueue::Listener;
	
	ListenerID EventQueue::GetNextOpenListenerSlot() {
		if (_vacantListenerSlots.empty())
			return _slotCount++;

		auto a = _vacantListenerSlots.front();
		_vacantListenerSlots.pop_front();
		return a;

	}

	ListenerID EventQueue::RegisterListener(Listener listener) {
		if (!listener)
			return -1;
		
		auto id = GetNextOpenListenerSlot();

		_listeners[id] = listener;
		return id;
	}

	void EventQueue::DeRegisterListener(ListenerID listenerID) {
		if (!_listeners[listenerID])
			return;

		_listeners[listenerID] = nullptr;
		_vacantListenerSlots.emplace_back(listenerID);
	}

	EventQueue::~EventQueue() {
		ClearEvents(_eventQueue.begin(), _eventQueue.end());
	}

	void  EventQueue::ClearEvents(std::list<Event>::iterator start, std::list<Event>::iterator end) {
		for (auto it = start; it != end;) {
			Event current = *it;
			it++;
			delete current;
		}

		_eventQueue.erase(start, end);
	}




	void EventQueue::EnqueueEvent(EventQueue::Event event) {
		_eventQueue.emplace_back(event);
	}

	void EventQueue::InvokeEvents() {
		if (_eventQueue.empty())
			return;
		
		auto end = _eventQueue.end();

		for (auto it = _eventQueue.begin(); it != end;) {
			Event current = *it;
			it++;
			InvokeEvent(current);
		}

		ClearEvents(_eventQueue.begin(), end);
	}

	void EventQueue::InvokeEvent(Event event) {
		for (int i = 0; i < _slotCount; i++) {
			if (!_listeners[i])
				continue;
			_listeners[i](event);
		}

	}



#pragma endregion
}