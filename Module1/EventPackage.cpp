#include "EventPackage.h"


#pragma region EventQueue Pattern
using ListenerID = EventP::EventQueue::ListenerID;

ListenerID EventP::EventQueue::GetNextOpenListenerSlot() {
	if (_vacantListenerSlots.size() == 0)
		return _slotCount++;
	
	auto a = _vacantListenerSlots.top();
	_vacantListenerSlots.pop();
	return a;

}

#pragma endregion