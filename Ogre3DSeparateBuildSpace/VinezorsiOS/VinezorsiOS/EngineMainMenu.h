//
//  EngineMainMenu.h
//  Vinezors2.0
//
//  Created by Calvin Phung on 4/18/14.
//
//

#ifndef __Vinezors2_0__EngineMainMenu__
#define __Vinezors2_0__EngineMainMenu__

#include "Util.h"

#include "Engine.h"
#include "HudMainMenu.h"

class EngineMainMenu : public Engine
{
public:
    EngineMainMenu(EngineStateManager* engineStateMgr, Player* player);
    virtual ~EngineMainMenu();
    
    virtual void enter();
    virtual void exit();
    virtual void update(float elapsed);
    virtual EngineState getEngineType() const {
        return ENGINE_MAIN_MENU;
    }
    
    virtual void activatePerformSingleTap(float x, float y);
    
#if OGRE_PLATFORM != OGRE_PLATFORM_APPLE_IOS
    virtual void mouseMoved(const OIS::MouseEvent &evt);
	virtual void mousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID id);
	virtual void mouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID id);
    
    virtual void keyPressed(const OIS::KeyEvent &keyEventRef);
	virtual void keyReleased(const OIS::KeyEvent &keyEventRef);
#endif
    
    
    virtual void requestResize();
    
    
protected:
	Player* player;
    HudMainMenu* hud;
    
    bool levelsPressed = false;
    bool settingsPressed = false;

    
    void alloc();
    void dealloc();
};

#endif /* defined(__Vinezors2_0__EngineMainMenu__) */
