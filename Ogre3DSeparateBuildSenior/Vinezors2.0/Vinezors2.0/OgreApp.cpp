#include "OgreApp.h"

#include "Tunnel.h"
#include "Player.h"
#include "Hud.h"
#include "LevelManager.h"
#include <fstream>
#include <ctime>
#include <cstdlib>

Util::ConfigGlobal globals;

#define NETWORKING 1

OgreApp::OgreApp()
{
}

//|||||||||||||||||||||||||||||||||||||||||||||||

OgreApp::~OgreApp()
{
#ifdef USE_RTSHADER_SYSTEM
    mShaderGenerator->removeSceneManager(OgreFramework::getSingletonPtr()->m_pSceneMgrMain);
    
    finalizeRTShaderSystem();
#endif
    delete OgreFramework::getSingletonPtr();
}

//|||||||||||||||||||||||||||||||||||||||||||||||

#ifdef USE_RTSHADER_SYSTEM

/*-----------------------------------------------------------------------------
 | Initialize the RT Shader system.
 -----------------------------------------------------------------------------*/
bool OgreApp::initializeRTShaderSystem(SceneManager* sceneMgr)
{
    if (RTShader::ShaderGenerator::initialize())
    {
        mShaderGenerator = RTShader::ShaderGenerator::getSingletonPtr();
        
        mShaderGenerator->addSceneManager(sceneMgr);
        
        // Setup core libraries and shader cache path.
        StringVector groupVector = ResourceGroupManager::getSingleton().getResourceGroups();
        StringVector::iterator itGroup = groupVector.begin();
        StringVector::iterator itGroupEnd = groupVector.end();
        String shaderCoreLibsPath;
        String shaderCachePath;
        
        for (; itGroup != itGroupEnd; ++itGroup)
        {
            ResourceGroupManager::LocationList resLocationsList = ResourceGroupManager::getSingleton().getResourceLocationList(*itGroup);
            ResourceGroupManager::LocationList::iterator it = resLocationsList.begin();
            ResourceGroupManager::LocationList::iterator itEnd = resLocationsList.end();
            bool coreLibsFound = false;
            
            // Try to find the location of the core shader lib functions and use it
            // as shader cache path as well - this will reduce the number of generated files
            // when running from different directories.
            for (; it != itEnd; ++it)
            {
                if ((*it)->archive->getName().find("RTShaderLib") != String::npos)
                {
                    shaderCoreLibsPath = (*it)->archive->getName() + "/";
                    shaderCachePath = shaderCoreLibsPath;
                    coreLibsFound = true;
                    break;
                }
            }
            // Core libs path found in the current group.
            if (coreLibsFound)
                break;
        }
        
        // Core shader libs not found -> shader generating will fail.
        if (shaderCoreLibsPath.empty())
            return false;
        
        // Create and register the material manager listener.
        mMaterialMgrListener = new ShaderGeneratorTechniqueResolverListener(mShaderGenerator);
        MaterialManager::getSingleton().addListener(mMaterialMgrListener);
    }
    
    return true;
}

/*-----------------------------------------------------------------------------
 | Finalize the RT Shader system.
 -----------------------------------------------------------------------------*/
void OgreApp::finalizeRTShaderSystem()
{
    // Restore default scheme.
    MaterialManager::getSingleton().setActiveScheme(MaterialManager::DEFAULT_SCHEME_NAME);
    
    // Unregister the material manager listener.
    if (mMaterialMgrListener != NULL)
    {
        MaterialManager::getSingleton().removeListener(mMaterialMgrListener);
        delete mMaterialMgrListener;
        mMaterialMgrListener = NULL;
    }
    
    // Finalize RTShader system.
    if (mShaderGenerator != NULL)
    {
        RTShader::ShaderGenerator::finalize();
        mShaderGenerator = NULL;
    }
}
#endif // USE_RTSHADER_SYSTEM

#if !defined(OGRE_IS_IOS)
void OgreApp::startDemo(const char* name, MusicMode musica)
#else
void OgreApp::startDemo(void* uiWindow, void* uiView, unsigned int width, unsigned int height, const char* name, MusicMode musica)
#endif
{
    globals.playerName = name;
    gameState = STATE_PLAY;
    musicMode = musica;
    
	new OgreFramework();
#if !defined(OGRE_IS_IOS)
    if (!OgreFramework::getSingletonPtr()->initOgre(this, this, this))
        return;
#else
	if (!OgreFramework::getSingletonPtr()->initOgre(uiWindow, uiView, width, height, this))
		return;
#endif
    
    
	m_bShutdown = false;
    
	OgreFramework::getSingletonPtr()->m_pLog->logMessage("Demo initialized!");
	
#ifdef USE_RTSHADER_SYSTEM
    initializeRTShaderSystem(OgreFramework::getSingletonPtr()->m_pSceneMgrMain);
    MaterialPtr baseWhite = MaterialManager::getSingleton().getByName("BaseWhite", ResourceGroupManager::INTERNAL_RESOURCE_GROUP_NAME);
    baseWhite->setLightingEnabled(false);
    mShaderGenerator->createShaderBasedTechnique(
                                                 "BaseWhite",
                                                 MaterialManager::DEFAULT_SCHEME_NAME,
                                                 RTShader::ShaderGenerator::DEFAULT_SCHEME_NAME);
    mShaderGenerator->validateMaterial(RTShader::ShaderGenerator::DEFAULT_SCHEME_NAME,
                                       "BaseWhite");
    baseWhite->getTechnique(0)->getPass(0)->setVertexProgram(
                                                             baseWhite->getTechnique(1)->getPass(0)->getVertexProgram()->getName());
    baseWhite->getTechnique(0)->getPass(0)->setFragmentProgram(
                                                               baseWhite->getTechnique(1)->getPass(0)->getFragmentProgram()->getName());
    
    // creates shaders for base material BaseWhiteNoLighting using the RTSS
    mShaderGenerator->createShaderBasedTechnique(
                                                 "BaseWhiteNoLighting",
                                                 MaterialManager::DEFAULT_SCHEME_NAME,
                                                 RTShader::ShaderGenerator::DEFAULT_SCHEME_NAME);
    mShaderGenerator->validateMaterial(RTShader::ShaderGenerator::DEFAULT_SCHEME_NAME,
                                       "BaseWhiteNoLighting");
    MaterialPtr baseWhiteNoLighting = MaterialManager::getSingleton().getByName("BaseWhiteNoLighting", ResourceGroupManager::INTERNAL_RESOURCE_GROUP_NAME);
    baseWhiteNoLighting->getTechnique(0)->getPass(0)->setVertexProgram(
                                                                       baseWhiteNoLighting->getTechnique(1)->getPass(0)->getVertexProgram()->getName());
    baseWhiteNoLighting->getTechnique(0)->getPass(0)->setFragmentProgram(
                                                                         baseWhiteNoLighting->getTechnique(1)->getPass(0)->getFragmentProgram()->getName());
    
    
#endif
    
	setupDemoScene();
#if !((OGRE_PLATFORM == OGRE_PLATFORM_APPLE) && __LP64__)
	runDemo();
#endif
}

//|||||||||||||||||||||||||||||||||||||||||||||||

void OgreApp::setupDemoScene()
{
    globals.initPaths();
#if defined(NETWORKING)
#if defined(OGRE_IS_IOS)
    syncConfig();
#endif
#endif
    
    seed = time(0);
    
    srand(seed);
    sessionOver = false;
    
    Util::generateMaterials();
    
    Util::createSphere(OgreFramework::getSingletonPtr()->m_pSceneMgrMain, "sphereMesh", 1.0, 8, 8);
    Util::createCylinder(OgreFramework::getSingletonPtr()->m_pSceneMgrMain, "cylinderMesh", 1.0, 1.0, 8);
    Util::createDiamond(OgreFramework::getSingletonPtr()->m_pSceneMgrMain, "diamondMesh", 1.0, 1.0);
    Util::createBox(OgreFramework::getSingletonPtr()->m_pSceneMgrMain, "boxMesh", 1.0, 1.0, 1.0);
    Util::createPlane(OgreFramework::getSingletonPtr()->m_pSceneMgrMain, "planeMesh", 1.0, 1.0);
    Util::createDefaultSegments(OgreFramework::getSingletonPtr()->m_pSceneMgrMain);
    
	origin = Vector3(0, 0, 0);
    
	OgreFramework::getSingletonPtr()->m_pCameraMain->setPosition(Vector3(origin.x, origin.y, origin.z + globals.tunnelSegmentDepth / 2));
	OgreFramework::getSingletonPtr()->m_pCameraMain->lookAt(origin);
    
    globals.initPaths();
    if (!configStageType(globals.configPath, globals.configBackup, "globalConfig"))
        globals.setMessage("WARNING: Failed to read configuration", MESSAGE_ERROR);
    
	player = new Player(
                        globals.playerName,
                        OgreFramework::getSingletonPtr()->m_pCameraMain->getPosition(),
                        OgreFramework::getSingletonPtr()->m_pCameraMain->getOrientation(),
                        globals.initCamSpeed,
                        globals.vineTOffset,
                        SPEED_CONTROL_AUTO,
                        seed,
                        "vinezors" + Util::toStringInt(seed) + ".csv");
	player->addVine(new Vine(OgreFramework::getSingletonPtr()->m_pSceneMgrMain->getRootSceneNode(), player->getCamPos(), globals.vineRadius));
    player->setSounds(true);
    player->setRunningSpeed(globals.set1StartingSpeed, globals.set2StartingSpeed, globals.set3StartingSpeed, globals.trialStartingSpeed, globals.startingNav);
    if (!player->loadProgress(globals.savePath))
        std::cout << "WARNING: Save File could not be loaded correctly" << std::endl;
    globals.initLogs(player->getSkillLevel().sessionID);
    
    tunnel = NULL;
    hud = new Hud();
    
    // Determine length of time
    globals.sessionTime = globals.sessionTimeMin;
    if (player->getSkillLevel().sessionID >= 1)
        globals.sessionTime += ((globals.sessionTimeMax - globals.sessionTimeMin) / globals.expectedNumSessions) * (player->getSkillLevel().sessionID - 1);
    std::cout << "Session Length: " << globals.sessionTime << std::endl;
    
    levelMgr = new LevelManager(player, globals.scheduleMain, globals.scheduleRepeat, globals.scheduleRepeatRandomPool);
    
    setLevel(EVEN);
    
    Light* lightMain = OgreFramework::getSingletonPtr()->m_pSceneMgrMain->createLight("Light");
    lightMain->setDiffuseColour(1.0, 1.0, 1.0);
    lightMain->setSpecularColour(1.0, 1.0, 1.0);
    //lightMain->setAttenuation(3250, 1.0, 0.0014, 0.000007);
    lightNodeMain = OgreFramework::getSingletonPtr()->m_pSceneMgrMain->getRootSceneNode()->createChildSceneNode("lightNode");
    lightNodeMain->attachObject(lightMain);
    lightNodeMain->setPosition(OgreFramework::getSingletonPtr()->m_pCameraMain->getPosition());
    
#if defined(NETWORKING)
#if defined(OGRE_IS_IOS)
    syncLogs();
#endif
#endif
}

void OgreApp::update(float elapsed)
{
    OgreFramework::getSingletonPtr()->m_pSoundMgr->update(elapsed);
    
    switch (gameState)
    {
        case STATE_PLAY:
        {
            // Update the game state
            if (!pause) {
                player->update(elapsed);
                if (tunnel->needsCleaning())
                {
                    endLevel();
                    //// Generate a new tunnel because we are done with current one
                    if (gameState == STATE_PLAY)
                        setLevel();
                }
            } else {
#if !defined(OGRE_IS_IOS)
                // Navigation Debug Keys
                if (player->getKeyUp())
                    player->move(Vector3(player->getCamForward() * globals.initCamSpeed * elapsed));
                if (player->getKeyDown())
                    player->move(Vector3(player->getCamForward() * -globals.initCamSpeed * elapsed));
                if (player->getKeyLeft())
                    player->move(Vector3(player->getCamRight() * -globals.initCamSpeed * elapsed));
                if (player->getKeyRight())
                    player->move(Vector3(player->getCamRight() * globals.initCamSpeed * elapsed));
#endif
            }
            hud->update(elapsed);
            
            // Graphical view changes
            lightNodeMain->setPosition(OgreFramework::getSingletonPtr()->m_pCameraMain->getPosition());
            
            Quaternion camRot = player->getCombinedRotAndRoll();
            OgreFramework::getSingletonPtr()->m_pCameraMain->setPosition(player->getCamPos());
            OgreFramework::getSingletonPtr()->m_pCameraMain->setOrientation(camRot);
            
            OgreFramework::getSingletonPtr()->m_pSceneMgrMain->getSkyPlaneNode()->setOrientation(player->getCombinedRotAndRoll());
            break;
        }
        case STATE_PROMPT:
        {
            break;
        }
    }
}

void OgreApp::endLevel(Evaluation forced)
{
    globals.clearMessage();
    
    if (tunnel)
    {
        tunnel->setCleaning(true);
        if (levelMgr->levelFinished(tunnel, forced))
        {
            gameState = STATE_PROMPT;
            globals.setMessage("Play Again?\n\n\n(Yes / No)\n\n\n<--- Swipe --->", MESSAGE_NORMAL);
        }
        else
            gameState = STATE_PLAY;
    }
}

void OgreApp::setLevel(Evaluation forced, bool forward)
{
    globals.clearMessage();
    
    std::vector<NavigationLevel> navLevels;
    if (tunnel)
    {
        levelMgr->updatePlayerSkill(tunnel, forced);
        if (forward)
            levelMgr->incrementSchedIndex();
        else
            levelMgr->decrementSchedIndex();
        navLevels = tunnel->getNavLevels();
        player->unlink();
        tunnel->unlink();
        hud->unlink();
    }
    
    // Don't gen a new tunnel if our schedule is over
    if (!predictSessionOver())
    {
        tunnel = levelMgr->getNextLevel(tunnel);
        if (navLevels.size() <= 0) // First stage
        {
            globals.appendMessage("\n\nSession No: " + Util::toStringInt(player->getSkillLevel().sessionID), MESSAGE_NORMAL);
        }
    }
    // Link/Relink Pointers
    tunnel->link(player, hud);
    player->link(tunnel, hud);
    hud->init(tunnel, player);
    
    if (!predictSessionOver())
    {
        // If repeated, repeat the same navigation level
        if (levelMgr->isRepeatingPhase())
            tunnel->setNavigationLevels(navLevels);
        else
            tunnel->setNavigationLevels();
        tunnel->constructTunnel(globals.tunnelSections);
        // If nback is same then panels are changing, keep speed same
        player->setCamPos(tunnel->getStart() + tunnel->getCurrent()->getForward() * globals.tunnelSegmentDepth);
        player->setCamRot(tunnel->getCurrent()->getQuaternion());
        player->setDesireRot(player->getCamRot());
        player->newTunnel(musicMode == MUSIC_ENABLED);
        
        Plane plane;
        switch (globals.setSkyBox)
        {
            case 0:
                plane.d = 3000;
                plane.normal = Ogre::Vector3(0, 0, 1);
                OgreFramework::getSingletonPtr()->m_pSceneMgrMain->setSkyPlane(true, plane, "General/SpaceSkyPlane", 80, 4, true);
                OgreFramework::getSingletonPtr()->m_pSceneMgrMain->setFog(Ogre::FOG_LINEAR, Ogre::ColourValue::ZERO, 0.0, 300.0, 600.0);
                OgreFramework::getSingletonPtr()->m_pViewportMain->setBackgroundColour(ColourValue(0.0f, 0.0f, 0.0f, 1.0f));
                OgreFramework::getSingletonPtr()->m_pSceneMgrMain->getSkyPlaneNode()->resetToInitialState();
                break;
            case 1:
                plane.d = 160;
                plane.normal = Ogre::Vector3(0, 0, 1);
                OgreFramework::getSingletonPtr()->m_pSceneMgrMain->setSkyPlane(true, plane, "General/TestSkyPlane0", 1.9, 1, true);
                OgreFramework::getSingletonPtr()->m_pSceneMgrMain->setFog(Ogre::FOG_LINEAR, Ogre::ColourValue(1.0, 0.95, 0.3, 0.0), 0.0, 500.0,  800.0);
                OgreFramework::getSingletonPtr()->m_pViewportMain->setBackgroundColour(ColourValue(0.0f, 0.0f, 0.0f, 1.0f));
                OgreFramework::getSingletonPtr()->m_pSceneMgrMain->getSkyPlaneNode()->resetToInitialState();
                break;
            default:
                plane.d = 3000;
                plane.normal = Ogre::Vector3(0, 0, 1);
                OgreFramework::getSingletonPtr()->m_pSceneMgrMain->setSkyPlane(true, plane, "General/SpaceSkyPlane", 80, 4, true);
                OgreFramework::getSingletonPtr()->m_pSceneMgrMain->setFog(Ogre::FOG_LINEAR, Ogre::ColourValue::ZERO, 0.0, 300.0, 600.0);
                OgreFramework::getSingletonPtr()->m_pViewportMain->setBackgroundColour(ColourValue(0.0f, 0.0f, 0.0f, 1.0f));
                OgreFramework::getSingletonPtr()->m_pSceneMgrMain->getSkyPlaneNode()->resetToInitialState();
                break;
        }
    }
    
    // Stop the game
    setPause(true);
}

float OgreApp::getTotalElapsed() const
{
    return OgreFramework::getSingletonPtr()->totalElapsed;
}

void OgreApp::setPause(bool value)
{
    if (value)
    {
        pause = value;
        player->pause();
        OgreFramework::getSingletonPtr()->m_pSoundMgr->pauseAllSounds();
        if (predictSessionOver())
        {
            if (player->getName() == "subject999")
                globals.setMessage("Training is complete!\nPlease check in.\n\nDouble tap to exit.", MESSAGE_FINAL);
            else
                globals.setMessage("That's it for Today!\nPlease check in before you leave.\n\nYou completed: " + Util::toStringInt(player->getNumStagesWon()) + " stages!\n\nDouble tap to exit.", MESSAGE_FINAL);
            endGame();
        }
        else
        {
            globals.appendMessage("\n\nSwipe to Continue", MESSAGE_NORMAL);
#ifdef DEBUG_MODE
            globals.appendMessage("\n\n" + player->getSkillLevel().getCurrentStats(), MESSAGE_NORMAL);
            globals.appendMessage("\n\n" + Util::toStringInt(globals.sessionTime - getTotalElapsed()), MESSAGE_NORMAL);
#endif
        }
        Ogre::ControllerManager::getSingleton().setTimeFactor(0);
    }
    else if (!sessionOver)
    {
        pause = value;
        player->unpause();
        OgreFramework::getSingletonPtr()->m_pSoundMgr->resumeAllPausedSounds();
        
        globals.clearMessage();
        Ogre::ControllerManager::getSingleton().setTimeFactor(1);
    }
}

bool OgreApp::isPaused() const
{
    return pause;
}

bool OgreApp::predictSessionOver() const
{
    return sessionOver ||
    ((!tunnel || (tunnel->getTotalElapsed() <= 0.0 || tunnel->needsCleaning())) && // At the beginning or end of stages only
    ((getTotalElapsed() > globals.sessionTime && // Ran out of time, done with main schedule
     levelMgr->isDoneWithMainSchedule() &&  // , and not retrying a level
     !levelMgr->isRepeatingPhase()) ||
    (player->getSkillLevel().sessionID <= 0 && // First session exits early
     levelMgr->isDoneWithMainSchedule())));
}

void OgreApp::endGame()
{
    if (!sessionOver)
    {
        player->saveStage(globals.logPath);
        player->saveProgress(globals.savePath, levelMgr->isDoneWithMainSchedule());
        sessionOver = true;
#if defined(NETWORKING)
#if defined(OGRE_IS_IOS)
        syncLogs();
#endif
#endif
    }
}

void OgreApp::requestResize()
{
    if (hud) hud->setOverlay();
}

//|||||||||||||||||||||||||||||||||||||||||||||||

void OgreApp::runDemo()
{
	OgreFramework::getSingletonPtr()->m_pLog->logMessage("Start main loop...");
    
	float timeSinceLastFrame = 0;
	float startTime = 0;
    
    OgreFramework::getSingletonPtr()->m_pRenderWnd->resetStatistics();
    
#if (!defined(OGRE_IS_IOS)) && !((OGRE_PLATFORM == OGRE_PLATFORM_APPLE) && __LP64__)
	while(!m_bShutdown && !OgreFramework::getSingletonPtr()->isOgreToBeShutDown())
	{
		if(OgreFramework::getSingletonPtr()->m_pRenderWnd->isClosed())m_bShutdown = true;
        
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32 || OGRE_PLATFORM == OGRE_PLATFORM_LINUX || OGRE_PLATFORM == OGRE_PLATFORM_APPLE
		WindowEventUtilities::messagePump();
#endif
		if(OgreFramework::getSingletonPtr()->m_pRenderWnd->isActive())
		{
			startTime = OgreFramework::getSingletonPtr()->m_pTimer->getMillisecondsCPU();
            
#if !OGRE_IS_IOS
			OgreFramework::getSingletonPtr()->m_pKeyboard->capture();
#endif
			OgreFramework::getSingletonPtr()->m_pMouse->capture();
            
			OgreFramework::getSingletonPtr()->updateOgre(timeSinceLastFrame);
			OgreFramework::getSingletonPtr()->m_pRoot->renderOneFrame();
            
			timeSinceLastFrame = OgreFramework::getSingletonPtr()->m_pTimer->getMillisecondsCPU() - startTime;
		}
		else
		{
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
            Sleep(1000);
#else
            sleep(1);
#endif
		}
	}
#endif
    
#if !defined(OGRE_IS_IOS)
	OgreFramework::getSingletonPtr()->m_pLog->logMessage("Main loop quit");
	OgreFramework::getSingletonPtr()->m_pLog->logMessage("Shutdown OGRE...");
#endif
}

void OgreApp::activatePerformLeftMove()
{
    player->addAction(ACTION_SWIPE_LEFT);
    switch (gameState)
    {
        case STATE_PLAY:
        {
            if (pause)
            {
                setPause(false);
            }
            else
            {
                if (player->setVineDirRequest(Util::rightOf(player->getVineDest())) && !tunnel->isDone())
                {
                    float val = player->getDesireRoll();
                    player->setDesireRoll(val + 45);
                }
                globals.moveLeft = true;
            }
            break;
        }
        case STATE_PROMPT:
        {
            gameState = STATE_PLAY;
            levelMgr->repeatPreviousPhase();
            setLevel();
            break;
        }
    }
}

void OgreApp::activatePerformRightMove()
{
    player->addAction(ACTION_SWIPE_RIGHT);
    switch (gameState)
    {
        case STATE_PLAY:
        {
            if (pause)
            {
                setPause(false);
            }
            else
            {
                if (player->setVineDirRequest(Util::leftOf(player->getVineDest())) && !tunnel->isDone())
                {
                    float val = player->getDesireRoll();
                    player->setDesireRoll(val - 45);
                }
                globals.moveRight = true;
            }
            break;
        }
        case STATE_PROMPT:
        {
            gameState = STATE_PLAY;
            setLevel(FAIL);
            break;
        }
    }
}

void OgreApp::activatePerformDoubleTap(float x, float y)
{
    //player->addAction(ACTION_DOUBLE_TAP);
    //if (gameState == STATE_PLAY && !pause) player->performShockwave();
    if (sessionOver)
        OgreFramework::getSingletonPtr()->requestOgreShutdown();
}

void OgreApp::activatePerformSingleTap(float x, float y)
{
    player->addAction(ACTION_SINGLE_TAP);
    if (x >= globals.pauseButton_posX && x <= globals.pauseButton_posX + globals.pauseButton_width &&
        y >= globals.pauseButton_posY && y <= globals.pauseButton_posY + globals.pauseButton_posY) {
        if (!pause) setPause(true);
    }
#ifdef DEBUG_MODE
    else if (y <= 100 && x <= globals.screenWidth / 2) {
        setLevel(EVEN, false);
    } else if (y <= 100 && x > globals.screenWidth / 2) {
        setLevel(EVEN);
    }
#endif
}

void OgreApp::activatePerformPinch()
{
    //player->addAction(ACTION_PINCH);
    //setPause(!pause);
}

void OgreApp::activatePerformBeginLongPress()
{
    player->addAction(ACTION_TAP_HOLD);
    if (gameState == STATE_PLAY && !pause && globals.boostEnabled) player->performBoost();
}

void OgreApp::activatePerformEndLongPress()
{
}

#if OGRE_PLATFORM != OGRE_PLATFORM_APPLE_IOS
bool OgreApp::mouseMoved(const OIS::MouseEvent &evt)
{
    if (pause) {
        // Move the camera when paused
        Vector2 dmove = Vector2(evt.state.X.rel, evt.state.Y.rel);
        
        Vector3 right = player->getCamRight(true);
        Vector3 up = player->getCamUpward(true);
        Quaternion yawRot;
        Quaternion pitchRot;
        yawRot.FromAngleAxis(Degree(-dmove.x) / 4, up);
        pitchRot.FromAngleAxis(Degree(-dmove.y) / 4, right);
        
        Quaternion curRot = player->getCamRot();
        curRot = pitchRot * yawRot * curRot;
        player->setCamRot(curRot);
        curRot = player->getCombinedRotAndRoll();
        OgreFramework::getSingletonPtr()->m_pCameraMain->setOrientation(curRot);
        
        player->setMousePos(Vector2(evt.state.X.abs, evt.state.Y.abs));
    }
    else
    {
        //if (player->getMouseLeft())
        {
            player->checkCursorMove(evt.state.X.rel, evt.state.Y.rel);
            if (player->checkPerformLeftMove(false))
            {
                activatePerformLeftMove();
                player->setCursorMoved();
            }
            else if (player->checkPerformRightMove(false))
            {
                activatePerformRightMove();
                player->setCursorMoved();
            }
        }
    }
    return true;
}

bool OgreApp::mousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
    switch (id)
    {
        case OIS::MB_Left:
            player->setMouseLeft(true);
            break;
        default:
            break;
    }
    return true;
}

bool OgreApp::mouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
    switch (id)
    {
        case OIS::MB_Left:
            player->setMouseLeft(false);
            player->resetCursorMoved();
            break;
        default:
            break;
    }
    return true;
}
#endif

//|||||||||||||||||||||||||||||||||||||||||||||||

bool OgreApp::keyPressed(const OIS::KeyEvent &keyEventRef)
{
#if !defined(OGRE_IS_IOS)
	OgreFramework::getSingletonPtr()->keyPressed(keyEventRef);
	
    switch (keyEventRef.key)
    {
        case OIS::KC_LEFT:
        {
            player->setKeyLeft(true);
            if (player->checkPerformLeftMove(true))
                activatePerformLeftMove();
            break;
        }
        case OIS::KC_RIGHT:
        {
            player->setKeyRight(true);
            player->resetCursorMoved();
            if (player->checkPerformRightMove(true))
                activatePerformRightMove();
            break;
        }
        case OIS::KC_UP:
        {
            player->setKeyUp(true);
            if (player->getSpeedControl() == SPEED_CONTROL_FLEXIBLE)
            {
                int s = player->getBaseSpeed();
                if (s + 1 <= globals.maxCamSpeed)
                    s++;
                player->setBaseSpeed(s);
            }
            break;
        }
        case OIS::KC_DOWN:
        {
            player->setKeyDown(true);
            if (player->getSpeedControl() == SPEED_CONTROL_FLEXIBLE)
            {
                int s = player->getBaseSpeed();
                if (s - 1 >= globals.minCamSpeed)
                    s--;
                player->setBaseSpeed(s);
            }
            break;
        }
        case OIS::KC_P:
        {
            setPause(!pause);
            break;
        }
        case OIS::KC_MINUS:
        {
            endLevel(FAIL);
            setLevel(FAIL);
            break;
        }
        case OIS::KC_EQUALS:
        {
            endLevel(PASS);
            setLevel(PASS);
            break;
        }
        case OIS::KC_X:
        {
            player->changeMovementMode();
            break;
        }
        case OIS::KC_C:
        {
            player->setShowCombo(player->getShowCombo() ? 0 : 1);
            break;
        }
        case OIS::KC_SPACE:
        {
            player->setKeySpace(true);
            activatePerformDoubleTap(0, 0);
            break;
        }
        case OIS::KC_Z:
        {
            activatePerformBeginLongPress();
            break;
        }
        default:
            break;
    }
#endif
	return true;
}

//|||||||||||||||||||||||||||||||||||||||||||||||

bool OgreApp::keyReleased(const OIS::KeyEvent &keyEventRef)
{
#if !defined(OGRE_IS_IOS)
	OgreFramework::getSingletonPtr()->keyReleased(keyEventRef);
    
    switch (keyEventRef.key)
    {
        case OIS::KC_LEFT:
        {
            player->setKeyLeft(false);
            break;
        }
        case OIS::KC_RIGHT:
        {
            player->setKeyRight(false);
            break;
        }
        case OIS::KC_UP:
        {
            player->setKeyUp(false);
            break;
        }
        case OIS::KC_DOWN:
        {
            player->setKeyDown(false);
            break;
        }
        case OIS::KC_SPACE:
        {
            player->setKeySpace(false);
            break;
        }
        default:
            break;
    }
#endif
    
	return true;
}

//|||||||||||||||||||||||||||||||||||||||||||||||


void OgreApp::preViewportUpdate(const Ogre::RenderTargetViewportEvent & evt)
{
    if (evt.source == OgreFramework::getSingletonPtr()->m_pViewportSide)
    {
        hud->hideOverlays();
    }
}

void OgreApp::postViewportUpdate(const Ogre::RenderTargetViewportEvent & evt)
{
    hud->showOverlays();
}