#include "PotPattern.h"

#include <algorithm>
#include <cstdlib>

bool comparePotX(Pot* lhs, Pot* rhs)
{
    return lhs->getPosition().x < rhs->getPosition().x;
}

PotPattern::PotPattern(Stage* stage, Player* player)
: PopzorsPattern(stage, player), signaled(false), signalStart(SIGNAL_START), signalLength(SIGNAL_LENGTH), timer(0), stages(0), stageIndex(0), potsPerStage(0), potsLeft(), poppyAssignmentList(), playerNumAnswers(0), blinkPotIndex(0), spawnPoppyTimer(0), usefulPotIndex(0), selected(NULL)
{
}

void PotPattern::setup()
{
    stage->ground = new Ground(Vector3(0, 0, 0), GROUND_COLOR, GROUND_COLOR, SIGNAL_LENGTH);
    
    stages = 1;
    potsPerStage = player->level + 2;
    int numDistractors = player->level;
    player->totalProblems = stages * potsPerStage;
    
    for (int i = 0; i < potsPerStage * stages; ++i)
    {
        Pot* pot = new Pot(Vector3(randRangeDouble(-4, 4), POT_HEIGHT / 2, randRangeDouble(-2, 0)), POT_RADIUS / 2,
                           BASE_COLOR, BLAND_COLOR, 1, getSoundAccordingToColor(BLAND_COLOR));
        pot->setId(i);
        stage->pots.push_back(pot);
        ColourValue col = getRandomPotColor();
        stage->pots[i]->setBlinkColor(col);
        stage->pots[i]->setSound(getSoundAccordingToColor(col));
    }
    
    poppyAssignmentList;
    for (int i = 0; i < stage->pots.size(); ++i)
        poppyAssignmentList.push_back(stage->pots[i]->getBlinkColor());
    for (int i = 0; i < numDistractors; ++i)
        poppyAssignmentList.push_back(getRandomPotColor());
}

void PotPattern::reset()
{
    PopzorsPattern::reset();
    signaled = false;
    signalStart = SIGNAL_START;
    signalLength = SIGNAL_LENGTH;
    stages = 0;
    stageIndex = 0;
    timer = 0;
    potsLeft.clear();
    playerNumAnswers = 0;
    blinkPotIndex = 0;
    spawnPoppyTimer = 0;
    usefulPotIndex = 0;
    selected = NULL;
    
}

void PotPattern::setPattern()
{
    if (stageIndex <= 0) {
        reset();
        setup();
    }
    
    potsLeft.clear();
    for (int i = stageIndex * potsPerStage; i < stage->pots.size(); ++i)
        potsLeft.push_back(stage->pots[i]);
    for (int i = 0; i < potsLeft.size(); ++i)
        potsLeft[i]->setTimeBlinkLength(signalLength * (potsLeft.size() - i + 1));
}

bool PotPattern::isFinished() const
{
    return playerNumAnswers >= player->totalProblems;
}

void PotPattern::processSelect(ClickedResult res)
{
    /*
    if (res.poppy && res.eventCode == InputEvent::EVENT_MOUSEDOWN) {
        
        if (res.poppy->isSelectable())
        {
            //deselect
            if ( (selected = res.poppy) && (selected != NULL) )
                selected = NULL;
            res.poppy->setColor(SELECT_COLOR);
            selected = res.poppy;
        }
    }
    
    if (res.pot && res.eventCode == InputEvent::EVENT_MOUSEDOWN) {
        
        if (res.pot->isSelectable())
        {
            if (selected && selected != res.pot && isReady())
                if (selected->getType() == Selectable::TYPE_POPPY)
                {
                    Poppy * old = (Poppy*)selected;
                    Vector3 potpos = res.pot->getPosition();
                    old->setPosition(potpos.x, potpos.y, potpos.z);
                    old->setSelectable(false);
                    old->deactivateJump();
                    old->setMoving(false);
                    res.pot->setSelectable(false);
                    selected = NULL;
                    
                    updatePlayerChoice(old, res.pot);
                }
        }
    }
     */
}

void PotPattern::update(double elapsed)
{
    player->totalElapsed += elapsed;
    if (ready && poppyAssignmentList.size() > 0)
    {
        spawnPoppyTimer += elapsed;
        const double SPAWN_RATE = 1.0;
        if (spawnPoppyTimer >= SPAWN_RATE) {
            addPoppy();
            spawnPoppyTimer = 0;
        }
    }
    updatePotBlinks(elapsed);
    
    stage->update(elapsed);
    for (int i = 0; i < stage->poppies.size(); ++i)
    {
        // If poppy is at destination, deactivate jump animation
        Vector3 dist = stage->poppies[i]->getDest() - stage->poppies[i]->getPosition();
        if (dist.length() <= 0.1)
        {
            stage->poppies[i]->setMoving(false);
            stage->poppies[i]->deactivateJump();
        }
    }
    
    stage->handleBothCollisions(elapsed);
    
    if (isFinished() && !stage->ground->isBlinking())
    {
        // Done with all stages, reset to new level
        stageIndex = 0;
        updateLevel();
        setPattern();
    }
    
    double barWidth = Util::HP_BAR_WIDTH;
    if (player->numConsecutiveSuccess > 0) {
        barWidth *= player->numConsecutiveSuccess / (double)(player->levelUpCeiling);
        
        stage->barHP->setDimensions(barWidth, Util::HP_BAR_HEIGHT);
        if (player->numConsecutiveSuccess >= Player::levelUpCeiling)
            stage->barHP->setMaterialName("General/BaseBlue");
        else
            stage->barHP->setMaterialName("General/BaseGreen");
    } else {
        if (stage->ground->getBlinkColor() == FEEDBACK_COLOR_BAD)
            stage->barHP->setDimensions(barWidth, Util::HP_BAR_HEIGHT);
        else
            stage->barHP->setDimensions(0.0, Util::HP_BAR_HEIGHT);
        stage->barHP->setMaterialName("General/BaseRed");
    }
    stage->label1->setCaption("Time: " + toStringInt(player->totalElapsed));
    stage->label2->setCaption("Score: " + toStringInt(player->score));
}

void PotPattern::updateLevel()
{
    if (player->numCorrect >= player->totalProblems)
    {
        player->updateLevel(PLAYER_SUCCESS);
    }
    else
    {
        player->updateLevel(PLAYER_FAILURE);
    }
}

void PotPattern::updatePlayerChoice(Poppy* poppy, Pot* pot)
{
    if (pot->getBlinkColor() != BLAND_COLOR && pot->getBlinkColor() == poppy->getBlinkColor())
    {
        player->numCorrect++;
    }
    printf("%d %d\n", player->numCorrect, player->totalProblems);
    playerNumAnswers++;
    if (playerNumAnswers % potsPerStage == 0)
    {
        // Reorganize the pots in the right order to refer to the next problem set
        std::vector<Pot*> temp;
        for (int i = 0; i < stage->pots.size(); ++i)
            if (!stage->pots[i]->isSelectable()) {
                stage->pots[i]->setBaseColor(stage->pots[i]->getBlinkColor());
                temp.push_back(stage->pots[i]);
            }
        for (int i = 0; i < stage->pots.size(); ++i)
            if (stage->pots[i]->isSelectable() && stage->pots[i]->getBlinkColor() != BLAND_COLOR)
                temp.push_back(stage->pots[i]);
        for (int i = 0; i < stage->pots.size(); ++i)
            if (stage->pots[i]->isSelectable() && stage->pots[i]->getBlinkColor() == BLAND_COLOR)
                temp.push_back(stage->pots[i]);
        stage->pots = temp;
        
        if (isFinished())
        {
            if (player->numCorrect >= player->totalProblems)
            {
                stage->ground->setBlinkColor(FEEDBACK_COLOR_GOOD);
                player->updateSuccess(PLAYER_SUCCESS);
                player->score += player->level;
            }
            else
            {
                stage->ground->setBlinkColor(FEEDBACK_COLOR_BAD);
                player->updateSuccess(PLAYER_FAILURE);
            }
            stage->ground->activateBlink();
        }
        else
        {
            ready = false;
            signaled = false;
            timer = 0;
            potsLeft.clear();
            blinkPotIndex = 0;
            spawnPoppyTimer = 0;
            ++stageIndex;
            usefulPotIndex = 0;
            setPattern();
        }
    }
}

void PotPattern::updatePotBlinks(double elapsed)
{
    timer += elapsed;
	if (!signaled && timer > signalStart)
    {
        signaled = true;
        potsLeft[blinkPotIndex]->activateBlink();
        potsLeft[blinkPotIndex]->playSound();
        blinkPotIndex++;
        timer = 0;
    }
	if (signaled && timer >= signalLength)
    {
        if (blinkPotIndex >= 0 && blinkPotIndex < potsLeft.size())
        {
            potsLeft[blinkPotIndex]->activateBlink();
            potsLeft[blinkPotIndex]->playSound();
            blinkPotIndex++;
        }
        else ready = true;
        timer = 0;
	}
}

void PotPattern::addPoppy()
{
    if (poppyAssignmentList.size() <= 0)
        return;
    
    ColourValue potColor;
    int r = rand() % poppyAssignmentList.size();
    potColor = poppyAssignmentList[r];
    poppyAssignmentList[r] = poppyAssignmentList[poppyAssignmentList.size() - 1];
    poppyAssignmentList.pop_back();
    
    Poppy* poppy = new Poppy(Vector3(randRangeDouble(-6, 6),POPPY_RADIUS,randRangeDouble(4, 4)), potColor, potColor, 1.0, POPPY_RADIUS);
    poppy->setId(stage->poppies.size());
    poppy->setMoving(true);
    poppy->setDest(Vector3(randRangeDouble(-3, 3),POPPY_RADIUS,randRangeDouble(1, 3)));
    poppy->activateJump();
    stage->poppies.push_back(poppy);
}

bool PotPattern::mouseMoved(const OIS::MouseEvent &evt)
{
    Ogre::Ray ray = OgreFramework::getSingletonPtr()->getCursorRay();
    Ogre::RaySceneQuery* query = OgreFramework::getSingletonPtr()->m_pSceneMgr->createRayQuery(ray);
    
    query->setSortByDistance(true);
    Ogre::RaySceneQueryResult result = query->execute();
    
    if (stage->selected && stage->selected->getType() == Selectable::TYPE_POPPY)
        for (int i = 0; i < result.size(); ++i)
            if (stage->ground->hasEntity( (Entity*)result[i].movable )) {
                Poppy* toMove = (Poppy*)stage->selected;
                Vector3 hoverPos = ray * result[i].distance;
                hoverPos.y = POPPY_RADIUS;
                toMove->setPosition(hoverPos);
            }
    return true;
}

bool PotPattern::mousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
    Ogre::Ray ray = OgreFramework::getSingletonPtr()->getCursorRay();
    Ogre::RaySceneQuery* query = OgreFramework::getSingletonPtr()->m_pSceneMgr->createRayQuery(ray);
    
    query->setSortByDistance(true);
    Ogre::RaySceneQueryResult result = query->execute();
    
    //Handle Selection
    if (stage->selected == NULL) {
        if (result.size() > 0 && result[0].movable != NULL) {
            for (int i = 0; i < stage->poppies.size(); ++i)
                if (stage->poppies[i]->isSelectable() &&
                    stage->poppies[i]->hasEntity( (Ogre::Entity*)result[0].movable )) {
                    stage->poppies[i]->setColor(getRandomPotColor());
                    stage->selected = stage->poppies[i];
                    stage->poppies[i]->setMoving(false);
                    stage->poppies[i]->deactivateJump();
                    //soundPickUp->play();
                }
            
            for (int i = 0; i < stage->pots.size(); ++i)
                if (stage->pots[i]->hasEntity( (Ogre::Entity*)result[0].movable) )
                    stage->pots[i]->setColor(getRandomPotColor());
        }
    }
    
    return true;
}

// *** TODO: Needs to check isReady() before placing in pot pot
bool PotPattern::mouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
    Ogre::Ray ray = OgreFramework::getSingletonPtr()->getCursorRay();
    Ogre::RaySceneQuery* query = OgreFramework::getSingletonPtr()->m_pSceneMgr->createRayQuery(ray);
    
    query->setSortByDistance(true);
    Ogre::RaySceneQueryResult result = query->execute();
    
    if (stage->selected && stage->selected->getType() == Selectable::TYPE_POPPY && result.size() > 0) {
        Poppy* old = (Poppy*)stage->selected;
        stage->selected = NULL;
        Vector3 placeDest = ray * result[0].distance;
        placeDest.y = POPPY_RADIUS;
        old->setPosition(placeDest);
        old->activateJump();
    
        // Is it within a pot?
        for (int i = 0; i < stage->pots.size(); ++i)
        {
            if (stage->pots[i]->isSelectable() &&
                (stage->pots[i]->getPosition() - old->getPosition()).length() <= POT_RADIUS / 1.5)
            {
                old->setSelectable(false);
                old->deactivateJump();
                old->setMoving(false);
                old->setVelocity(Vector3::ZERO);
                old->setPosition(stage->pots[i]->getPosition());
                stage->pots[i]->setSelectable(false);
                updatePlayerChoice(old, stage->pots[i]);
                break;
            }
        }
    }
    
    return true;
}