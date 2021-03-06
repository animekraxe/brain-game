//
//  EngineStateManager.cpp
//  Vinezors2.0
//
//  Created by Calvin Phung on 4/17/14.
//
//

#include "EngineStateManager.h"
#include "EngineStage.h"
#include "EngineLevelSelection.h"
#include "EngineMainMenu.h"
#include "EngineCredits.h"
#include <iostream>

EngineStateManager::EngineStateManager()
: gameEngineStack(), cleanup()
{
}

void EngineStateManager::update(float elapsed)
{
    for (int i = 0; i < cleanup.size(); ++i)
        delete cleanup[i];
    cleanup.clear();
}

Engine* EngineStateManager::getActiveEngine() const
{
    return gameEngineStack.size() > 0 ? gameEngineStack[gameEngineStack.size() - 1] : NULL;
}

void EngineStateManager::requestPushEngine(EngineState engineType, Player* player)
{
    switch (engineType)
    {
        case ENGINE_STAGE:
        {
            Engine* top = getActiveEngine();
            if (top) top->exit();
            gameEngineStack.push_back(new EngineStage(this, player));
            break;
        }
        case ENGINE_LEVEL_SELECTION:
        {
            Engine* top = getActiveEngine();
            if (top) top->exit();
            gameEngineStack.push_back(new EngineLevelSelection(this, player));
            break;
        }
        case ENGINE_MAIN_MENU:
        {
            Engine* top = getActiveEngine();
            if (top) top->exit();
            gameEngineStack.push_back(new EngineMainMenu(this, player));
            break;
        }
        case ENGINE_CREDITS:
        {
            Engine* top = getActiveEngine();
            if (top) top->exit();
            gameEngineStack.push_back(new EngineCredits(this, player));
            break;
        }
        default:
            std::cout << "WARNING: Unknown engine type push request!\n";
            break;
    }
}

void EngineStateManager::requestPopEngine()
{
    if (gameEngineStack.size() > 0)
    {
        Engine* top = getActiveEngine();
        if (top)
        {
            top->exit();
            cleanup.push_back(top);
            gameEngineStack.pop_back();
            top = getActiveEngine();
            if (top) top->enter();
        }
    }
    else
        std::cout << "WARNING: No more engines to pop!\n";
}
