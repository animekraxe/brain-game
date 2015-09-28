//
//  HudLevelSelection.h
//  Vinezors2.0
//
//  Created by Calvin Phung on 4/17/14.
//
//

#ifndef __Vinezors2_0__HudLevelSelection__
#define __Vinezors2_0__HudLevelSelection__

#include "Hud.h"
#include "Util.h"

class HudLevelSelection : public Hud
{
public:
    HudLevelSelection(Player* player);
    virtual ~HudLevelSelection();
    
    virtual void init();
    virtual void adjust();
    virtual void update(float elapsed);
protected:
    // 2-D grid of level selection buttons
    static const int LEVEL_ITEM_WIDTH = 5;
    static const int LEVEL_ITEM_HEIGHT = 3;
    
    Player* player;
    
    static const int NUM_UNIQUE_BUTTONS = 4;
    enum Buttons {
        BUTTON_BACK, BUTTON_UP, BUTTON_DOWN, BUTTON_ENABLETUTORIALS
        // Note: levelItemBackgrounds are created based on LEVEL_ITEM_WIDTH and LEVEL_ITEM_HEIGHT
        // which are not included in here
    };
    
    // Overlay items for GAME_STATE_MENU
    PanelOverlayElement* levelSelectEntireBackground;
    TextAreaOverlayElement* levelSelectPromptText;
    
    // Container of all the overlay elements to make this level selection icon
    std::vector< std::vector<PanelOverlayElement*> > levelItemBackgrounds;
    std::vector< std::vector<PanelOverlayElement*> > levelItemPlanets;
    std::vector< std::vector<TextAreaOverlayElement*> > levelItemNames;
    std::vector< std::vector<TextAreaOverlayElement*> > levelItemScores;
    
    std::vector<TextAreaOverlayElement*> levelSetNames;
    std::vector<TextAreaOverlayElement*> levelSetStars;
    std::vector<TextAreaOverlayElement*> levelSetScores;
    PanelOverlayElement* levelTotalScoreBackground;
    TextAreaOverlayElement* levelTotalScore;
    PanelOverlayElement* backButtonBackground;
    PanelOverlayElement* buttonGoUpBackground;
    PanelOverlayElement* buttonGoDownBackground;
    
    PanelOverlayElement* buttonEnableTutorialsBackground;
    PanelOverlayElement* checkDisplayEnableTutorialsBackground;
    TextAreaOverlayElement* textEnableTutorialsPrompt;
    
    
    
    void link(Player* player);
    void unlink();
    virtual void alloc();
    virtual void dealloc();
    virtual void initOverlay();
    
    int convertLevelRowToButtonRow() const;
    void updateDisplay();
    void setLevelButton(PanelOverlayElement* levelBackground, PanelOverlayElement* levelItem, TextAreaOverlayElement* levelName, TextAreaOverlayElement* levelScore, int row, int col);
};

#endif /* defined(__Vinezors2_0__HudLevelSelection__) */
