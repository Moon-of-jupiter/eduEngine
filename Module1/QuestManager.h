#ifndef QuestManager_hpp
#define QuestManager_hpp
#pragma once
#include "EventPackage.h"

class QuestManager {
    enum QuestLine
    {
        GetGrass,
        GoToHorse,
        FeedHorse,
        Done,
    };

    QuestLine questLineState = GetGrass;





};




#endif