//
//  Powerup.cpp
//  Vinezors2.0
//
//  Created by Emmanuel Flores on 4/15/14.
//
//

#include "Powerup.h"

Powerup::Powerup(float timeout)
    : available(false), active(false), mainTimer(0.0f), mainTimeout(timeout) {}

TractorBeam::TractorBeam()
    : Powerup(0.0f), targetPod(NULL), tractorBeamEffect(NULL), tractorBeamPulseNode(NULL), tractorBeamPulseEffect(NULL), pulseTimer(0.0f), pulseLen(0.0f), tractorBeamNode(NULL), tractorBeamRotatorNode(NULL), expireTimer(0.0f), expireCheckpoint(0.0f), effectModifier(0.0f) {}

void TractorBeam::reset()
{
    available = false;
    active = false;
    
    // should do, but don't need to yet, no tractor beam will be activated while transitioning b/t levels
    if (tractorBeamEffect) {
        tractorBeamPulseNode->detachObject(tractorBeamPulseEffect);
        tractorBeamPulseNode->getCreator()->destroyParticleSystem(tractorBeamPulseEffect);
        tractorBeamPulseEffect = NULL;
        OgreFramework::getSingletonPtr()->m_pSceneMgrMain->getRootSceneNode()->removeAndDestroyChild("PulseNode");
        tractorBeamPulseNode = NULL;
        
        tractorBeamNode->detachObject(tractorBeamEffect);
        tractorBeamNode->getCreator()->destroyParticleSystem(tractorBeamEffect);
        tractorBeamEffect = NULL;
        tractorBeamRotatorNode->removeAndDestroyChild("BeamNode");
        tractorBeamNode = NULL;
        OgreFramework::getSingletonPtr()->m_pSceneMgrMain->getRootSceneNode()->removeAndDestroyChild("BeamRotatorNode");
        tractorBeamRotatorNode = NULL;
    }
    targetPod = NULL;
}

TractorBeam::~TractorBeam()
{
}

TimeWarp::TimeWarp(float timeout, float timebonustimeout, float timebonus, float maxangle)
    : Powerup(timeout), origFov(Degree()), origAngle(0.0f), currentAngle(0.0f), maxAngle(maxangle), angleIncrement(1.0f), slowdown(0.2f), zoomIn(0), origOffset(0.0f), currentOffset(0.0f), offsetIncrement(0.02f), offsetLimit(0.3f), timeBonus(timebonus), timeBonusTimeout(timebonustimeout), currentTimeVal(0), timeVal(20) {}

void TimeWarp::reset()
{
    available = false;
    active = false;
}

TimeWarp::~TimeWarp()
{
}

Shields::Shields()
    : Powerup(0.0), shieldEntity(NULL), shieldNode(NULL), shieldScaleValue(0.0), shieldScaleTarget(0.0) {}

void Shields::reset()
{
    available = false;
    active = false;
    
    if (shieldNode || shieldEntity)
    {
        shieldNode->detachObject(shieldEntity);
        shieldNode->getCreator()->destroyEntity(shieldEntity);
        shieldEntity = NULL;
        shieldNode->getParent()->removeChild(shieldNode);
        shieldNode->getCreator()->destroySceneNode(shieldNode);
        shieldNode = NULL;
    }
}

Shields::~Shields()
{
}