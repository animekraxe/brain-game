//
//  Hud.cpp
//  Vinezors2.0
//
//  Created by Calvin Phung on 11/24/13.
//
//

#include "Hud.h"
#include "Tunnel.h"
#include "Player.h"

extern Util::ConfigGlobal globals;

Hud::Hud()
{
    // The code snippet below is used to output text
    // create a font resource
    ResourcePtr resourceText = OgreFramework::getSingletonPtr()->m_pFontMgr->create("Arial",ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
    resourceText->setParameter("type", "truetype");
    resourceText->setParameter("source", "C64_User_Mono_v1.0-STYLE.ttf");
    resourceText->setParameter("size", "16");
    resourceText->setParameter("resolution", "96");
    resourceText->load();
    
    panelText = static_cast<OverlayContainer*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "TextInterface"));
    panelText->setMetricsMode(GMM_PIXELS);
    panelText->setPosition(10, 10);
    panelText->setDimensions(10, 10);
    
    healthArea = static_cast<BorderPanelOverlayElement*>(
                                                         OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("BorderPanel", "HealthAreaBorder"));
    
    barHP = static_cast<PanelOverlayElement*>(
                                              OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "HealthBar"));
    indicator = static_cast<PanelOverlayElement*>(
                                                  OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "Indicator"));
    threshold1 = static_cast<PanelOverlayElement*>(
                                                        OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "threshold1"));
    threshold2 = static_cast<PanelOverlayElement*>(
                                                   OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "threshold2"));
    threshold3 = static_cast<PanelOverlayElement*>(
                                                   OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "threshold3"));
    
    // Create text area
    label1 = static_cast<TextAreaOverlayElement*>(
                                                 OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("TextArea", "TextAreaLabel1"));
    label2 = static_cast<TextAreaOverlayElement*>(
                                                 OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("TextArea", "TextAreaLabel2"));
    label3 = static_cast<TextAreaOverlayElement*>(
                                                 OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("TextArea", "TextAreaLabel3"));
    label4 = static_cast<TextAreaOverlayElement*>(
                                                 OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("TextArea", "TextAreaLabel4"));
    label5 = static_cast<TextAreaOverlayElement*>(
                                                 OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("TextArea", "TextAreaLabel5"));
    label6 = static_cast<TextAreaOverlayElement*>(
                                                 OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("TextArea", "TextAreaLabel6"));
    
    // Create an overlay, and add the panel
    Overlay* overlay1 = OgreFramework::getSingletonPtr()->m_pOverlayMgr->create("OverlayHealthArea");
    Overlay* overlay2 = OgreFramework::getSingletonPtr()->m_pOverlayMgr->create("OverlayText");
    overlay1->add2D(healthArea);
    //overlay1->add2D(barHP);
    overlay1->add2D(indicator);
    overlay1->add2D(threshold1);
    overlay1->add2D(threshold2);
    overlay1->add2D(threshold3);
    overlay2->add2D(panelText);
    
    panelText->addChild(label1);
    panelText->addChild(label2);
    panelText->addChild(label3);
    panelText->addChild(label4);
    panelText->addChild(label5);
    panelText->addChild(label6);
    overlays.push_back(overlay1);
    overlays.push_back(overlay2);
}

void Hud::init(Tunnel* tunnel, Player* player)
{
    panelText->setMetricsMode(GMM_PIXELS);
    panelText->setPosition(10, 10);
    panelText->setDimensions(10, 10);
    
    healthArea->setMetricsMode(GMM_RELATIVE);
    healthArea->setPosition(globals.HPBarXRef - 0.01, globals.HPBarYRef - 0.01);
    healthArea->setDimensions(globals.HPBarWidth + 0.02, globals.HPBarHeight + 0.02);
    healthArea->setMaterialName("General/BarColors");
    //healthArea->setMaterialName("General/BaseBlack");
    
    barHP->setMetricsMode(GMM_RELATIVE);
    barHP->setPosition(globals.HPBarXRef, globals.HPBarYRef);
    barHP->setDimensions(0.0, globals.HPBarHeight);
    barHP->setMaterialName("General/BarColors");
    
    indicator->setMetricsMode(GMM_RELATIVE);
    indicator->setPosition(barHP->getLeft(), barHP->getTop() - 0.005);
    indicator->setDimensions(healthArea->getWidth() / 20, globals.HPBarHeight + 0.01);
    indicator->setMaterialName("General/Indicator");
    
    threshold1->setMetricsMode(GMM_RELATIVE);
    threshold1->setDimensions(healthArea->getWidth() / 20, globals.HPBarHeight + 0.01);
    threshold1->setMaterialName("General/StarGray");
    
    threshold2->setMetricsMode(GMM_RELATIVE);
    threshold2->setDimensions(healthArea->getWidth() / 20, globals.HPBarHeight + 0.01);
    threshold2->setMaterialName("General/StarGray");
    
    threshold3->setMetricsMode(GMM_RELATIVE);
    threshold3->setDimensions(healthArea->getWidth() / 20, globals.HPBarHeight + 0.01);
    threshold3->setMaterialName("General/StarGray");
    
    label1->setMetricsMode(GMM_PIXELS);
    label1->setAlignment(TextAreaOverlayElement::Center);
    label1->setPosition(globals.label1_posX, globals.label1_posY);
    label1->setCharHeight(globals.screenHeight / 30);
    label1->setFontName("Arial");
    label1->setColour(ColourValue::Green);
    
    label2->setMetricsMode(GMM_PIXELS);
    label2->setPosition(globals.label2_posX, globals.label2_posY);
    label2->setCharHeight(globals.screenHeight / 50);
    label2->setColour(ColourValue::White);
    label2->setFontName("Arial");
    
    label3->setMetricsMode(GMM_PIXELS);
    label3->setAlignment(TextAreaOverlayElement::Right);
    label3->setPosition(globals.label3_posX, globals.label3_posY);
    label3->setCharHeight(globals.screenHeight / 50);
    label3->setColour(ColourValue::White);
    label3->setFontName("Arial");
    
    label4->setMetricsMode(GMM_PIXELS);
    label4->setPosition(globals.label4_posX, globals.label4_posY);
    label4->setCharHeight(globals.screenHeight / 50);
    label4->setColour(ColourValue::White);
    label4->setFontName("Arial");
    
    label5->setMetricsMode(GMM_PIXELS);
    label5->setAlignment(TextAreaOverlayElement::Right);
    label5->setPosition(globals.label5_posX, globals.label5_posY);
    label5->setCharHeight(globals.screenHeight / 50);
    label5->setColour(ColourValue::ColourValue(1.0, 1.0, 0.0));
    label5->setFontName("Arial");
    
    label6->setMetricsMode(GMM_PIXELS);
    label6->setAlignment(TextAreaOverlayElement::Center);
    label6->setPosition(globals.label6_posX, globals.label6_posY);
    label6->setCharHeight(globals.screenHeight / 50);
    label6->setColour(ColourValue::ColourValue(1.0, 1.0, 0.0));
    label6->setFontName("Arial");
    
    if (tunnel->getMode() == GAME_TIMED)
    {
        healthArea->hide();
        barHP->hide();
        indicator->hide();
    }
    else if (tunnel->getMode() == GAME_PROFICIENCY)
    {
        healthArea->show();
        barHP->show();
        indicator->show();
    }
    else //if (tunnel->getMode() == GAME_NAVIGATION)
    {
        healthArea->hide();
        barHP->hide();
        indicator->hide();
    }
    
    for(int i = 0; i < overlays.size(); ++i)
        overlays[i]->show();
}

void Hud::update(Tunnel* tunnel, Player* player, double elapsed)
{
    double timeLeft = std::max(globals.stageTime - tunnel->getTotalElapsed(), 0.0);
    
    if (tunnel->getMode() == GAME_TIMED)
        label1->setCaption(Util::toStringInt(timeLeft));
    else
        label1->setCaption("");
    if (tunnel->getMode() == GAME_TIMED)
        //label2->setCaption("Distance: " + Util::toStringInt(player->getScore()));
        label2->setCaption("");
    else if (tunnel->getMode() == GAME_PROFICIENCY)
    {
        label2->setCaption("Time: " + Util::toStringInt(timeLeft));
        // Not for the 20th deadline
        //if (player->getNumCorrectBonus() <= 0)
        //    label2->setCaption("Points: " + Util::toStringInt(player->getPoints()));
        //else
        //    label2->setCaption("Points: " + Util::toStringInt(player->getPoints()) + " + " + Util::toStringInt(player->getNumCorrectBonus()));
    }
    if (tunnel->getMode() != GAME_NAVIGATION)
        label3->setCaption(Util::toStringInt(tunnel->getNBack()) + "-Back");
    else
        label3->setCaption("Gathered: " + Util::toStringInt(player->getNumCorrectTotal()) + "/" + Util::toStringInt(tunnel->getNumTargets()));
    label4->setCaption("Speed: " + Util::toStringInt(player->getCamSpeed()));
    if (tunnel->getMode() != GAME_NAVIGATION)
        label5->setCaption("Signals: " + Util::toStringInt(tunnel->getSignalsLeft()));
    else
        label5->setCaption("");
    label6->setCaption(globals.message);
    
    double indicatorRange = barHP->getWidth();
    double barWidth = globals.HPBarWidth;
    // As accuracy bar
    if (tunnel->getMode() == GAME_PROFICIENCY)
    {
        barHP->setDimensions(barWidth, globals.HPBarHeight);
        double progress = player->getProgress(tunnel);
        indicator->setPosition(barHP->getLeft() + barWidth * progress, indicator->getTop());
        
        threshold1->setPosition(barHP->getLeft() + globals.HPBarWidth * globals.stageProficiencyThreshold1, barHP->getTop() - 0.005);
        threshold2->setPosition(barHP->getLeft() + globals.HPBarWidth * globals.stageProficiencyThreshold2, barHP->getTop() - 0.005);
        threshold3->setPosition(barHP->getLeft() + globals.HPBarWidth * globals.stageProficiencyThreshold3, barHP->getTop() - 0.005);
        if (progress >= globals.stageProficiencyThreshold1)
            threshold1->setMaterialName("General/StarGold");
        else
            threshold1->setMaterialName("General/StarGray");
        if (progress >= globals.stageProficiencyThreshold2)
            threshold2->setMaterialName("General/StarGold");
        else
            threshold2->setMaterialName("General/StarGray");
        if (progress >= globals.stageProficiencyThreshold3)
            threshold3->setMaterialName("General/StarGold");
        else
            threshold3->setMaterialName("General/StarGray");
            
    }
    else if (tunnel->getMode() == GAME_TIMED)
    {
        threshold1->setPosition(0.45, 0.15);
        threshold2->setPosition(0.50, 0.15);
        threshold3->setPosition(0.55, 0.15);
        
        label1->setCaption(Util::toStringInt(timeLeft));
        if (timeLeft >= globals.stageTimeThreshold1)
            threshold1->setMaterialName("General/StarGold");
        else
            threshold1->setMaterialName("General/StarGray");
        if (timeLeft >= globals.stageTimeThreshold2)
            threshold2->setMaterialName("General/StarGold");
        else
            threshold2->setMaterialName("General/StarGray");
        if (timeLeft >= globals.stageTimeThreshold3)
            threshold3->setMaterialName("General/StarGold");
        else
            threshold3->setMaterialName("General/StarGray");
    }
    else //if (tunnel->getMode() == GAME_NAVIGATION)
    {
        threshold1->setPosition(0.45, 0.15);
        threshold2->setPosition(0.50, 0.15);
        threshold3->setPosition(0.55, 0.15);
        
        if (player->getHP() + 0.01 >= globals.stageNavigationThreshold1)
            threshold1->setMaterialName("General/StarGold");
        else
            threshold1->setMaterialName("General/StarGray");
        if (player->getHP() + 0.01 >=globals.stageNavigationThreshold2)
            threshold2->setMaterialName("General/StarGold");
        else
            threshold2->setMaterialName("General/StarGray");
        if (player->getHP() + 0.01 >= globals.stageNavigationThreshold3)
            threshold3->setMaterialName("General/StarGold");
        else
            threshold3->setMaterialName("General/StarGray");
    }
    /*
    // As HP counter
    if (tunnel->getMode() != GAME_TIMED)
    {
        barHP->setDimensions(barWidth, globals.HPBarHeight);
        int hpRange = globals.HPPositiveLimit - globals.HPNegativeLimit;
        indicatorRange *= (player->getHP() - globals.HPNegativeLimit) / (double)(hpRange);
        indicator->setPosition(barHP->getLeft() + indicatorRange, indicator->getTop());
    }
     */
}

void Hud::hideOverlays()
{
    for (int i = 0; i < overlays.size(); ++i)
        overlays[i]->hide();
}

void Hud::showOverlays()
{
    for (int i = 0; i < overlays.size(); ++i)
        overlays[i]->show();
}

Hud::~Hud()
{
    //***** Need to properly deallocate
}
