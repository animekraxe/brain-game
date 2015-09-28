//
//  LevelScheduler.cpp
//  Vinezors2.0
//
//  Created by Darrin Lin on 8/4/14.
//
//
//________________________________________________________________________________________

#include <iostream>
#include "LevelScheduler.h"

extern Util::ConfigGlobal globals;



using namespace std;

#define SCHEDULE_SIZE 5     // Number of levels in a schedule
//________________________________________________________________________________________

/**
 Creates a new scheduler
 */
LevelScheduler::LevelScheduler( double nBackLevelA, double nBackLevelB, double nBackLevelC, double nBackLevelD, double nBackLevelE )
: tutorialLevels(), scheduleHistoryA(), scheduleHistoryB(), scheduleHistoryC(), scheduleHistoryD(), scheduleHistoryE(), binA(NULL), binB(NULL), binC(NULL), binD(NULL), binE(NULL), totalMarbles(0), sessionFinished(false), sessionFinishedAcknowledged(false)
{
    this->nBackLevelA = nBackLevelA;
    this->nBackLevelB = nBackLevelB;
    this->nBackLevelC = nBackLevelC;
    this->nBackLevelD = nBackLevelD;
    this->nBackLevelE = nBackLevelE;
    this->scoreCurr = 0.0;
    this->speedA = 15.0f;
    this->speedB = 15.0f;
    this->speedC = 15.0f;
    this->speedD = 15.0f;
    this->speedE = 15.0f;
    this->firstTimeA = true;
    this->firstTimeB = true;
    this->firstTimeC = true;
    this->firstTimeD = true;
    this->firstTimeE = true;
    this->playCount = 0;
    this->holdoutOffsetA = 0.0f;
    this->holdoutOffsetB = 0.0f;
    this->holdoutOffsetD = 0.0f;
    this->holdoutLevelA = 0;
    this->holdoutLevelB = 0;
    this->holdoutLevelD = 0;
    
    if(globals.holdoutEnabled && globals.holdoutdelayEnabled)
    {
        //Goes here everytime that the app opens up from an exit.
        this->holdoutOffsetA = -(globals.holdoutdelayNumber-1);
        this->holdoutOffsetB = -(globals.holdoutdelayNumber-1);
        this->holdoutOffsetD = -(globals.holdoutdelayNumber-1);
    }
    initTutorialLevels(); // Also called after loading the scheduler
}
//________________________________________________________________________________________


// Initializes tutorials which will be played when the player has just started the scheduler
// and hasn't seen instruction on how to navigate and how to play
void LevelScheduler::initTutorialLevels()
{
    StageRequest level;
    std::pair<StageRequest, PlayerProgress> ret;
    
    // Tutorial levels have a stageNo of -1 to indicate they do not count towards level score
    
    // A recess for all navigation
    level = StageRequest();
    level.stageNo = -1;
    level.nback = 0;
    level.stageTime = 100.0;
    level.navLevels.push_back(NavigationLevel(0, 1, 0));
    level.navLevels.push_back(NavigationLevel(0, 2, 0));
    level.navLevels.push_back(NavigationLevel(0, 3, 0));
    level.navLevels.push_back(NavigationLevel(0, 4, 0));
    level.navLevels.push_back(NavigationLevel(0, 4, 0));
    level.navLevels.push_back(NavigationLevel(0, 3, 0));
    level.navLevels.push_back(NavigationLevel(0, 2, 0));
    level.navLevels.push_back(NavigationLevel(0, 1, 0));
    level.nameTunnelTile = "General/WallBindingG";
    level.nameSkybox = "General/BlankStarrySkyPlane";
    level.nameMusic = "Music4";
    level.tunnelSectionsPerNavLevel = 10;
    level.initCamSpeed = 10;
    level.minCamSpeed = 10;
    level.maxCamSpeed = 40;
    level.phaseX = PHASE_COLLECT;
    level.difficultyX = DIFFICULTY_EASY;
    level.durationX = DURATION_NORMAL;
    ret = std::pair<StageRequest, PlayerProgress>();
    ret.first = level;
    ret.second.nBackSkill = nBackLevelE;
    tutorialLevels.push_back(ret);
    
    // All-signal 1-Back
    level = StageRequest();
    level.stageNo = -1;
    level.nback = 1;
    level.stageTime = 100.0;
    level.navLevels.push_back(NavigationLevel(0, 1, 0));
    level.navLevels.push_back(NavigationLevel(0, 1, 0));
    level.navLevels.push_back(NavigationLevel(0, 1, 0));
    level.navLevels.push_back(NavigationLevel(0, 1, 0));
    for (int i = 0; i < 8; ++i)
        level.collectionCriteria.push_back(CollectionCriteria(1));
    level.nameTunnelTile = "General/WallBindingD";
    level.nameSkybox = "General/BlankStarrySkyPlane";
    level.nameMusic = "Music3";
    level.tunnelSectionsPerNavLevel = 10;
    level.initCamSpeed = 10;
    level.minCamSpeed = 10;
    level.maxCamSpeed = 40;
    level.phaseX = PHASE_ALL_SIGNAL;
    level.difficultyX = DIFFICULTY_EASY;
    level.durationX = DURATION_NORMAL;
    ret = std::pair<StageRequest, PlayerProgress>();
    ret.first = level;
    ret.second.nBackSkill = nBackLevelD;
    tutorialLevels.push_back(ret);
}
//________________________________________________________________________________________

void setHoldoutToCandidates(std::vector<Bin*> & candidates, int num)
{
    while (num > 0 && candidates.size() > 0)
    {
        int rindex = rand() % candidates.size();
        candidates[rindex]->holdout = true;
        candidates[rindex] = candidates[candidates.size() - 1];
        candidates.pop_back();
        num--;
    }
}

void addToHoldoutCandidates(std::vector<Bin*> & candidates, std::list<Bin>* v)
{
    for (std::list<Bin>::iterator it = v->begin(); it != v->end(); ++it)
    {
        if (it->durationX != DURATION_SHORT) {
            candidates.push_back(&(*it));
        }
    }
}

void LevelScheduler::populateBins()
{
    // Modifiers for each difficulty nBack
    const double N_BACK_EASY = 0.7;
    const double N_BACK_NORMAL = 0.0;
    const double N_BACK_HARD = -0.7;
    
    const int NUM_DIFFICULTIES = 3;
    
    if(binA) delete binA;
    binA = new std::list<Bin>();
    if(binB) delete binB;
    binB = new std::list<Bin>();
    if(binC) delete binC;
    binC = new std::list<Bin>();
    if (binD) delete binD;
    binD = new std::list<Bin>();
    if(binE) binE = new std::list<Bin>();
    binE = new std::list<Bin>();
    
    /////////////////////
    // Easy Difficulty //
    /////////////////////
    binA->push_back(Bin(PHASE_COLOR_SOUND, DIFFICULTY_EASY, DURATION_SHORT, false, N_BACK_EASY));
    binB->push_back(Bin(PHASE_SHAPE_SOUND, DIFFICULTY_EASY, DURATION_SHORT, false, N_BACK_EASY));
    binC->push_back(Bin(PHASE_SOUND_ONLY, DIFFICULTY_EASY, DURATION_SHORT, false, N_BACK_EASY));
    binD->push_back(Bin(PHASE_ALL_SIGNAL, DIFFICULTY_EASY, DURATION_SHORT, false, N_BACK_EASY));
    
    ///////////////////////
    // Normal Difficulty //
    ///////////////////////
    // COLOR SOUND:
    binA->push_back(Bin(PHASE_COLOR_SOUND, DIFFICULTY_NORMAL, DURATION_NORMAL, false, N_BACK_NORMAL));     // normal           normal          N
    binA->push_back(Bin(PHASE_COLOR_SOUND, DIFFICULTY_EASY, DURATION_NORMAL, false, N_BACK_NORMAL));      // normal           normal          Y
    binA->push_back(Bin(PHASE_COLOR_SOUND, DIFFICULTY_EASY, DURATION_LONG, false, N_BACK_NORMAL));       // long             normal          N
    
    // SHAPE SOUND:
    binB->push_back(Bin(PHASE_SHAPE_SOUND, DIFFICULTY_NORMAL, DURATION_NORMAL, false, N_BACK_NORMAL));     // normal           normal          N
    binB->push_back(Bin(PHASE_SHAPE_SOUND, DIFFICULTY_EASY, DURATION_NORMAL, false, N_BACK_NORMAL));      // normal           normal          Y
    binB->push_back(Bin(PHASE_SHAPE_SOUND, DIFFICULTY_EASY, DURATION_LONG, false, N_BACK_NORMAL));       // long             normal          N
    
    // SOUND ONLY:
    binC->push_back(Bin(PHASE_SOUND_ONLY, DIFFICULTY_NORMAL, DURATION_NORMAL, false, N_BACK_NORMAL));      // normal           normal          N
    binC->push_back(Bin(PHASE_SOUND_ONLY, DIFFICULTY_EASY, DURATION_NORMAL, false, N_BACK_NORMAL));          // long             easy            N
    binC->push_back(Bin(PHASE_SOUND_ONLY, DIFFICULTY_EASY, DURATION_LONG, false, N_BACK_NORMAL));        // long             normal          N
    
    // ALL SIGNAL:
    binD->push_back(Bin(PHASE_ALL_SIGNAL, DIFFICULTY_NORMAL, DURATION_NORMAL, false, N_BACK_NORMAL));         // normal           normal          N
    binD->push_back(Bin(PHASE_ALL_SIGNAL, DIFFICULTY_EASY, DURATION_NORMAL, false, N_BACK_NORMAL));          // normal           normal          Y
    binD->push_back(Bin(PHASE_ALL_SIGNAL, DIFFICULTY_EASY, DURATION_LONG, false, N_BACK_NORMAL));           // long             normal          N
    
    /////////////////////
    // Hard Difficulty //
     /////////////////////
    binA->push_back(Bin(PHASE_COLOR_SOUND, DIFFICULTY_HARD, DURATION_LONG, false, N_BACK_HARD));
    binB->push_back(Bin(PHASE_SHAPE_SOUND, DIFFICULTY_HARD, DURATION_LONG, false, N_BACK_HARD));
    binC->push_back(Bin(PHASE_SOUND_ONLY, DIFFICULTY_HARD, DURATION_LONG, false, N_BACK_HARD));
    binD->push_back(Bin(PHASE_ALL_SIGNAL, DIFFICULTY_HARD, DURATION_LONG, false, N_BACK_HARD));
    
    
    std::vector<Bin*> holdoutCandidatesA;
    std::vector<Bin*> holdoutCandidatesB;
    std::vector<Bin*> holdoutCandidatesD;
    addToHoldoutCandidates(holdoutCandidatesA, binA);
    addToHoldoutCandidates(holdoutCandidatesB, binB);
    addToHoldoutCandidates(holdoutCandidatesD, binD);
    
    setHoldoutToCandidates(holdoutCandidatesA, holdoutCandidatesA.size() / 2);
    setHoldoutToCandidates(holdoutCandidatesB, holdoutCandidatesB.size() / 2);
    setHoldoutToCandidates(holdoutCandidatesD, holdoutCandidatesD.size() / 2);
    
    //setHoldout(binA);
    //setHoldout(binB);
    // setHoldout(binC); // no holdout for sound only
    //setHoldout(binD);
    // setHoldout(binE); // no holdout for recess (collection level)
}
//________________________________________________________________________________________


void LevelScheduler::removeBin(LevelPhase phaseX, StageDifficulty difficultyX, StageDuration durationX, bool hasHoldout)
{
    switch (phaseX) {
        case PHASE_COLLECT:
            binE->remove(Bin(phaseX, difficultyX, durationX, hasHoldout));
            break;
        
        case PHASE_COLOR_SOUND:
            binA->remove(Bin(phaseX, difficultyX, durationX, hasHoldout));
            break;
            
        case PHASE_SHAPE_SOUND:
            binB->remove(Bin(phaseX, difficultyX, durationX, hasHoldout));
            break;
            
        case PHASE_SOUND_ONLY:
            binC->remove(Bin(phaseX, difficultyX, durationX, hasHoldout));
            break;
            
        case PHASE_ALL_SIGNAL:
            binD->remove(Bin(phaseX, difficultyX, durationX, hasHoldout));
            break;
            
        default:
            break;
    }
}
//________________________________________________________________________________________


/**
 Picks a random bin and returns it. If the bin is empty, it will recursively pick another. (Poor implementation but works for now)
 
 @return - A bin that has at least 1 element.
 */
std::list<Bin>* LevelScheduler::pickRandomBin(bool soundOnlyLevelsEnabled)
{
    
    enum binNums { binNumA, binNumB, binNumC, binNumD, binNumE };


    std::vector <int> possibleBins;
    if(soundOnlyLevelsEnabled)
    {
        possibleBins.push_back(binNumA);
        possibleBins.push_back(binNumB);
        possibleBins.push_back(binNumC);
        possibleBins.push_back(binNumD);
        possibleBins.push_back(binNumE);
    }
    else
    {
        possibleBins.push_back(binNumA);
        possibleBins.push_back(binNumB);
        possibleBins.push_back(binNumD);
        possibleBins.push_back(binNumE);
    }
    
    int MIN_BIN = 0, MAX_BIN = possibleBins.size()-1;
    int binNum = rand_num(MIN_BIN, MAX_BIN);
    binNum = possibleBins[binNum];
    
    // Keep track of the total number of elements in the bins.
    // If there are no elements left, populate the bin with more.
    if(binA && binB && binC && binD && binE) {
        totalMarbles = binA->size() + binB->size() + binC->size() + binD->size() + binE->size();
    }
    if (totalMarbles < 3) {
        std::cout << "Repopulating " << std::endl;
        populateBins();
        totalMarbles = binA->size() + binB->size() + binC->size() + binD->size() + binE->size();
    }
    
    // =========================================================================
    // Debug output - should be commented out on final release
        cout << "\n------------------------------------------------" << endl;
        cout << "Total Bin Marbles: " << totalMarbles << endl;
        cout << "------------------------------------------------" << endl;
    // =========================================================================
    
    switch (binNum) {
        case binNumA:
            if(binA->empty()) return pickRandomBin(soundOnlyLevelsEnabled);
            return binA;
            break;
        case binNumB:
            if(binB->empty()) return pickRandomBin(soundOnlyLevelsEnabled);
            return binB;
            break;
        case binNumC:
            if(binC->empty()) return pickRandomBin(soundOnlyLevelsEnabled);
            return binC;
            break;
        case binNumD:
            if(binD->empty()) return pickRandomBin(soundOnlyLevelsEnabled);
            return binD;
            break;
        case binNumE:
            if(binE->empty()) return pickRandomBin(soundOnlyLevelsEnabled);
            return binE;
            break;
        default:
            break;
    }
}
//________________________________________________________________________________________


// Update holdout values inside the bins (40% of the marbles/bin is holdout)
void LevelScheduler::setHoldout( std::list<Bin>* b )
{
    int holdoutCounter = 0;
    while ( holdoutCounter != 2 ) {
        for (std::list<Bin>::iterator it = b->begin(); (it != b->end()) && (holdoutCounter != 2); ++it) {
            if ( (it->durationX != DURATION_SHORT) && rand_num(0, 1) && !it->holdout ) {
                it->holdout = true;
                holdoutCounter++;
            }
            cout << "holdout counter: " << holdoutCounter << endl;
        }
    }
}
//________________________________________________________________________________________


// can keep a linear list of marbles to randomly pick from instead
void LevelScheduler::pickRandomMarble( std::vector<Bin>& choices, bool soundOnlyLevelsEnabled )
{
    std::list<Bin>& binRef = *pickRandomBin(soundOnlyLevelsEnabled);
    std::list<Bin>::iterator binIt = binRef.begin();
    int binIndex = rand_num(0, binRef.size() - 1);
    for ( int i = 0; i < binIndex; ++i, ++binIt );
    
    for ( int i = 0; i < choices.size(); ++i )
    {
        if ( *binIt == choices[i] )
        {
            // =========================================================================
            // Debug output - should be commented out on final release
                cout << "Need to pick extra bin & marble due to duplicate" << endl;
            // =========================================================================
            
            pickRandomMarble(choices,soundOnlyLevelsEnabled);
            return;
        }
    }
    choices.push_back(*binIt);
    
    // =========================================================================
    // Debug output - should be commented out on final release
        for(int i = 0; i < choices.size(); ++i)
        {
            cout << "Choice " << i << ": " << choices[i].phaseX << ", " << choices[i].difficultyX << endl;
        }
    // =========================================================================
    
}
//________________________________________________________________________________________


std::vector< std::pair<StageRequest, PlayerProgress> > LevelScheduler::generateChoices(bool holdoutEnabled, bool newNavEnabled, bool indRecessEnabled, double indRecessNBackLevel,bool holdoutDelayEnabled, float holdoutDelayNumber, bool manRecess, bool indRecessFixedEnabled, bool soundOnlyLevelsEnabled)
{
    
    /* For debugging purposes */
    cout <<  "/--------------------------------\\" << endl
         <<  "|       Current nBack Levels     |" << endl
         << "\\--------------------------------/" << endl
         << "A: " << nBackLevelA << endl
         << "B: " << nBackLevelB << endl
         << "C: " << nBackLevelC << endl
         << "D: " << nBackLevelD << endl
         << "E: " << nBackLevelE << endl
         << "IND_DRECESS: " << indRecessNBackLevel << endl
         <<  "__________________________________" << endl
         <<  "/--------------------------------\\" << endl
         <<  "|  Current nBackHoldout Levels   |" << endl
         << "\\--------------------------------/" << endl
         << "holdoutOffsetA: " << holdoutOffsetA << endl
         << "holdoutOffsetB: " << holdoutOffsetB << endl
         << "holdoutOffsetD: " << holdoutOffsetD << endl
         << "holdoutLevelA: " << holdoutLevelA << endl
         << "holdoutLevelB: " << holdoutLevelB << endl
         << "holdoutLevelD: " << holdoutLevelD << endl
         << "Man Recess: " << manRecess <<endl
         <<  "__________________________________" << endl;
    // */
    
    std::vector<Bin> choices;
    std::vector< std::pair<StageRequest, PlayerProgress> > result;
    std::pair<StageRequest, PlayerProgress> node;
    LevelPhase phase;
    StageDifficulty difficulty;
    StageDuration duration;
    bool holdout;
    double shift;
    double playerSkill;
    double playerOffset;
    int nBackRounded;
    
    // If the player has played enough stages, provide one
    // of the three choices to be a recess overriding
    // the randomly selected marble
    
    //Only choose a marble to be recess if manRecess is not enabled
    int recessIndex = -1;
    if (playCount >= 5)
    {
        
        playCount = 0;
        if(!manRecess)
        {
            recessIndex = rand() % 3;
        }
    }
    
    for(int i = 0; i < 3; ++i)
    {
        pickRandomMarble( choices, soundOnlyLevelsEnabled );
        if (recessIndex == i)
        {
            phase = PHASE_COLLECT;
            difficulty = DIFFICULTY_HARD;
            duration = DURATION_NORMAL;
            holdout = false;
            shift = 0.0;
        }
        else
        {
            phase = choices[i].phaseX;
            difficulty = choices[i].difficultyX;
            duration = choices[i].durationX;
            holdout = choices[i].holdout;
            shift = choices[i].nbackShift;
        }
        
//        cout << "\n\n================================\n\nPhase: " << phase << endl;
//        cout << "Difficulty: " << difficulty << endl;
        bool readyForHoldout = true;
        bool holdColor = true;
        bool holdShape = true;
        bool holdSound = true;
        int holdoutLevel = 0;   // Holdout intensity
        
        switch ( phase ) {
            case PHASE_COLLECT:
                playerSkill = nBackLevelE;
                readyForHoldout = false;
                playerOffset = 0.0;
                break;
            case PHASE_COLOR_SOUND:
                playerSkill = nBackLevelA;
                if((playerSkill < holdoutDelayNumber) && holdoutDelayEnabled)
                {
                    readyForHoldout = false;
                }
                // Tutorial For Holdout on Color/Sound
                if (holdoutLevelA <= 0)
                {
                    holdColor = false;
                    holdShape = false;
                    holdoutLevel = 999; // highest holdout intensity
                }
                else if (holdoutLevelA == 1)
                {
                    holdSound = false;
                    holdShape = false;
                    holdoutLevel = 999; // highest holdout intensity
                }
                else if (holdoutLevelA == 2)
                {
                    // normal 1-back holdout
                    holdoutLevel = 999; // highest holdout intensity
                }
                else
                {
                    holdoutLevel = std::max(holdoutLevelA - HOLDOUT_CHECKPOINTA, 0);
                }
                playerOffset = holdoutOffsetA;
                break;
            case PHASE_SHAPE_SOUND:
                playerSkill = nBackLevelB;
                if((playerSkill < holdoutDelayNumber) && holdoutDelayEnabled)
                {
                    readyForHoldout = false;
                }
                // Tutorial For Holdout on Shape/Sound
                if (holdoutLevelB <= 0)
                {
                    holdColor = false;
                    holdShape = false;
                    holdoutLevel = 999; // highest holdout intensity
                }
                else if (holdoutLevelB == 1)
                {
                    holdColor = false;
                    holdSound = false;
                    holdoutLevel = 999; // highest holdout intensity
                }
                else if (holdoutLevelB == 2)
                {
                    // normal 1-back holdout
                    holdoutLevel = 999; // highest holdout intensity
                }
                else
                {
                    holdoutLevel = std::max(holdoutLevelB - HOLDOUT_CHECKPOINTB, 0);
                }
                playerOffset = holdoutOffsetB;
                break;
            case PHASE_SOUND_ONLY:
                playerSkill = nBackLevelC;
                readyForHoldout = false;
                playerOffset = 0.0;
                break;
            case PHASE_ALL_SIGNAL:
                playerSkill = nBackLevelD;
                if((playerSkill < holdoutDelayNumber) && holdoutDelayEnabled)
                {
                    readyForHoldout = false;
                }
                if (holdoutLevelD <= 0)
                {
                    holdColor = false;
                    holdShape = false;
                    holdoutLevel = 999; // highest holdout intensity
                }
                else if (holdoutLevelD == 1)
                {
                    holdColor = false;
                    holdSound = false;
                    holdoutLevel = 999; // highest holdout intensity
                }
                else if (holdoutLevelD == 2)
                {
                    holdShape = false;
                    holdSound = false;
                    holdoutLevel = 999; // highest holdout intensity
                }
                else if (holdoutLevelD == 3)
                {
                    // normal 1-back holdout
                    holdoutLevel = 999; // highest holdout intensity
                }
                else
                {
                    holdoutLevel = std::max(holdoutLevelD - HOLDOUT_CHECKPOINTD, 0);
                }
                playerOffset = holdoutOffsetD;
                break;
            default:
                playerSkill = 1.0;
                playerOffset = 0.0;
                break;
        }
        //std::cout << "PHASE: " << phase << std::endl;
        //std::cout << "SKILL: " << playerSkill << std::endl;
        //std::cout << "SHIFT: " << shift << std::endl;
        
        //Only if holdout is Enabled!
        
        int currentUNL = (int)round(nBackLevelE);
        
        //If Recess and indRecess Enabled, we use another UNL!
        if((phase == PHASE_COLLECT) && indRecessEnabled)
        {
            playerSkill = indRecessNBackLevel;
            currentUNL = (int)round(indRecessNBackLevel);
            std::cout << "IND RECESS: " << indRecessNBackLevel << std::endl;

        }
        if(holdoutEnabled)
        {
            if (holdout && readyForHoldout) //Problem2
                nBackRounded = (int)round(playerSkill + playerOffset + shift);
            else
                nBackRounded = (int)round(playerSkill + shift);
        }
        else
        {
            nBackRounded = (int)round(playerSkill + shift);
        }
        

        
        if(nBackRounded < 1) nBackRounded = 1;
        
        if(holdoutEnabled)
        {
            if (holdout)
            {
                if(readyForHoldout)
                {
                    std::cout << "HOLDOUT FLAGS IN LEVEL: " << holdColor << " " << holdShape << " " << holdSound << std::endl;
                    node.first.generateStageRequest(nBackRounded, phase, difficulty, duration, 100.0, holdColor, holdShape, holdSound, holdoutLevel, currentUNL,newNavEnabled, indRecessEnabled, indRecessFixedEnabled);
                }
                else{
                    node.first.generateStageRequest(nBackRounded, phase, difficulty, duration, 0.0, false, false, false, 0, currentUNL,newNavEnabled, indRecessEnabled, indRecessFixedEnabled);
                }
            }
            else
            {
                node.first.generateStageRequest(nBackRounded, phase, difficulty, duration, 0.0, false, false, false, 0, currentUNL,newNavEnabled, indRecessEnabled, indRecessFixedEnabled);
            }
        }
        else
        {
            node.first.generateStageRequest(nBackRounded, phase, difficulty, duration, 0.0, false, false, false, 0, currentUNL,newNavEnabled, indRecessEnabled, indRecessFixedEnabled);
        }
        
        

        node.second.nBackSkill = playerSkill;
        node.second.nBackOffset = playerOffset;
        // binRef.remove(*binIt); // can't remove here... until they pick
        result.push_back(node);
    }
    
    //Push Recess Level
    holdout = false;
    shift = 0.0;
    int currentUNL = (int)round(nBackLevelE);
    //If Recess and indRecess Enabled, we use another UNL!
    if(indRecessEnabled)
    {
        playerSkill = indRecessNBackLevel;
        currentUNL = (int)round(indRecessNBackLevel);
        
    }
    nBackRounded = (int)round(playerSkill + shift);
    
    std::pair<StageRequest, PlayerProgress> Recessnode;
    Recessnode.first.generateStageRequest(nBackRounded, PHASE_COLLECT, DIFFICULTY_HARD, DURATION_NORMAL, 0.0, false, false, false, 0, currentUNL,newNavEnabled, indRecessEnabled, indRecessFixedEnabled);
    result.push_back(Recessnode);
    
    return result;
}
//________________________________________________________________________________________

/**
 Simple random number generator that returns a random number between lower and upper
 
 @param lower - lower bound
 @param upper - upper bound
 @return int - the random number generated
 */
int LevelScheduler::rand_num(int lower, int upper)
{
    return rand() % (upper - lower + 1) + lower;
}

// Returns an average of all columns that have already been played, otherwise it returns default from study settings
int LevelScheduler::predictAverageStartingSpeed(int initVel)
{
    int cnt = 0; // number of first times
    double total = 0.0;
    if (!firstTimeA) {
        total += speedA;
        cnt++;
    }
    if (!firstTimeB) {
        total += speedB;
        cnt++;
    }
    if (!firstTimeC) {
        total += speedC;
        cnt++;
    }
    if (!firstTimeD) {
        total += speedD;
        cnt++;
    }
    if (!firstTimeE) {
        total += speedE;
        cnt++;
    }
    return cnt > 0 ? total / cnt : initVel;
}

//________________________________________________________________________________________


void LevelScheduler::saveScheduler1_1(std::ostream& out)
{
    std::cout << "Saving Scheduler 1.1\n";
    out << nBackLevelA << " "
        << nBackLevelB << " "
        << nBackLevelC << " "
        << nBackLevelD << " "
        << nBackLevelE << " "
        << scoreCurr << " "
        << holdoutOffsetA << " "
        << holdoutOffsetB << " "
        << holdoutOffsetD << " "
        << holdoutLevelA << " "
        << holdoutLevelB << " "
        << holdoutLevelD << " "
        << speedA << " "
        << speedB << " "
        << speedC << " "
        << speedD << " "
        << speedE << " "
        << firstTimeA << " "
        << firstTimeB << " "
        << firstTimeC << " "
        << firstTimeD << " "
        << firstTimeE << " "
        << playCount << " ";
    
    std::cout << binA->size() << " "
    << binB->size() << " "
    << binC->size() << " "
    << binD->size() << " "
    << binE->size() << "\n";
    
    // save binA
    if( binA )  // if bin is null don't save
    {
        out << binA->size() << " ";
        for (std::list<Bin>::iterator it = binA->begin(); it != binA->end(); ++it) {
            out << it->phaseX << " "
                << it->difficultyX << " "
                << it->durationX << " "
                << it->holdout << " "
                << it->nbackShift << " ";
        }
    }
    else
        out << 0 << " ";
    
    // save binB
    if( binB )  // if bin is null don't save
    {
        out << binB->size() << " ";
        for (std::list<Bin>::iterator it = binB->begin(); it != binB->end(); ++it) {
            out << it->phaseX << " "
                << it->difficultyX << " "
                << it->durationX << " "
                << it->holdout << " "
                << it->nbackShift << " ";
        }
    }
    else
        out << 0 << " ";
    
    // save binC
    if( binC )  // if bin is null don't save
    {
        out << binC->size() << " ";
        for (std::list<Bin>::iterator it = binC->begin(); it != binC->end(); ++it) {
            out << it->phaseX << " "
                << it->difficultyX << " "
                << it->durationX << " "
                << it->holdout << " "
                << it->nbackShift << " ";
        }
    }
    else
        out << 0 << " ";
    
    // save binD
    if( binD )  // if bin is null don't save
    {
        out << binD->size() << " ";
        for (std::list<Bin>::iterator it = binD->begin(); it != binD->end(); ++it) {
            out << it->phaseX << " "
                << it->difficultyX << " "
                << it->durationX << " "
                << it->holdout << " "
                << it->nbackShift << " ";
        }
    }
    else
        out << 0 << " ";
    
    // save binE
    if( binE )  // if bin is null don't save
    {
        out << binE->size() << " ";
        for (std::list<Bin>::iterator it = binE->begin(); it != binE->end(); ++it) {
            out << it->phaseX << " "
                << it->difficultyX << " "
                << it->durationX << " "
                << it->holdout << " "
                << it->nbackShift << " ";
        }
    }
    else
        out << 0 << " ";
    
    if (scheduleHistoryA.size() > 0)
    {
        out << 1 << " ";
        out << scheduleHistoryA.back().first << " " << scheduleHistoryA.back().second << " ";
    }
    else
        out << 0 << " ";
    
    if (scheduleHistoryB.size() > 0)
    {
        out << 1 << " ";
        out << scheduleHistoryB.back().first << " " << scheduleHistoryB.back().second << " ";
    }
    else
        out << 0 << " ";
    
    if (scheduleHistoryC.size() > 0)
    {
        out << 1 << " ";
        out << scheduleHistoryC.back().first << " " << scheduleHistoryC.back().second << " ";
    }
    else
        out << 0 << " ";
    
    if (scheduleHistoryD.size() > 0)
    {
        out << 1 << " ";
        out << scheduleHistoryD.back().first << " " << scheduleHistoryD.back().second << " ";
    }
    else
        out << 0 << " ";
    
    if (scheduleHistoryE.size() > 0)
    {
        out << 1 << " ";
        out << scheduleHistoryE.back().first << " " << scheduleHistoryE.back().second << " ";
    }
    else
        out << 0 << " ";
}
//________________________________________________________________________________________

void LevelScheduler::loadScheduler1_0(std::istream& in)
{
    float currentHoldout; // obsolete now
    std::cout << "Loading Scheduler 1.0\n";
    in  >> nBackLevelA
        >> nBackLevelB
        >> nBackLevelC
        >> nBackLevelD
        >> nBackLevelE
        >> scoreCurr
        >> currentHoldout
        >> holdoutOffsetA
        >> holdoutOffsetB
        >> holdoutOffsetD
        >> speedA
        >> speedB
        >> speedC
        >> speedD
        >> speedE
        >> firstTimeA
        >> firstTimeB
        >> firstTimeC
        >> firstTimeD
        >> firstTimeE
        >> playCount;
    initTutorialLevels();
    
    int size;
    int tmpPhase;
    int tmpDifficulty;
    int tmpDuration;
    bool tmpHoldout;
    double tmpnbackShift;
    
    // read binA
    in >> size;
    if (binA) delete binA;
    binA = new std::list<Bin>();
    for (int i = 0; i < size; ++i)
    {
        in >> tmpPhase >> tmpDifficulty >> tmpDuration >> tmpHoldout >> tmpnbackShift;
        Bin tmpBin = Bin((LevelPhase)tmpPhase, (StageDifficulty)tmpDifficulty, (StageDuration)tmpDuration, tmpHoldout, tmpnbackShift);
        binA->push_back(tmpBin);
        cout << "Marble A: " << tmpPhase << " " << tmpDifficulty << " " << tmpDuration << " " << tmpHoldout << " " << tmpnbackShift << std::endl;
    }
    
    // read binB
    in >> size;
    if (binB) delete binB;
    binB = new std::list<Bin>();
    for (int i = 0; i < size; ++i)
    {
        in >> tmpPhase >> tmpDifficulty >> tmpDuration >> tmpHoldout >> tmpnbackShift;
        Bin tmpBin = Bin((LevelPhase)tmpPhase, (StageDifficulty)tmpDifficulty, (StageDuration)tmpDuration, tmpHoldout, tmpnbackShift);
        binB->push_back(tmpBin);
        cout << "Marble B: " << tmpPhase << " " << tmpDifficulty << " " << tmpDuration << " " << tmpHoldout << " " << tmpnbackShift << std::endl;
    }
    
    // read binC
    in >> size;
    if (binC) delete binC;
    binC = new std::list<Bin>();
    for (int i = 0; i < size; ++i)
    {
        in >> tmpPhase >> tmpDifficulty >> tmpDuration >> tmpHoldout >> tmpnbackShift;
        Bin tmpBin = Bin((LevelPhase)tmpPhase, (StageDifficulty)tmpDifficulty, (StageDuration)tmpDuration, tmpHoldout, tmpnbackShift);
        binC->push_back(tmpBin);
        cout << "Marble C: " << tmpPhase << " " << tmpDifficulty << " " << tmpDuration << " " << tmpHoldout << " " << tmpnbackShift << std::endl;
    }
    
    // read binD
    in >> size;
    if (binD) delete binD;
    binD = new std::list<Bin>();
    for (int i = 0; i < size; ++i)
    {
        in >> tmpPhase >> tmpDifficulty >> tmpDuration >> tmpHoldout >> tmpnbackShift;
        Bin tmpBin = Bin((LevelPhase)tmpPhase, (StageDifficulty)tmpDifficulty, (StageDuration)tmpDuration, tmpHoldout, tmpnbackShift);
        binD->push_back(tmpBin);
        cout << "Marble D: " << tmpPhase << " " << tmpDifficulty << " " << tmpDuration << " " << tmpHoldout << " " << tmpnbackShift << std::endl;
    }
    
    // read binE
    in >> size;
    if (binE) delete binE;
    binE = new std::list<Bin>();
    for (int i = 0; i < size; ++i)
    {
        in >> tmpPhase >> tmpDifficulty >> tmpDuration >> tmpHoldout >> tmpnbackShift;
        Bin tmpBin = Bin((LevelPhase)tmpPhase, (StageDifficulty)tmpDifficulty, (StageDuration)tmpDuration, tmpHoldout, tmpnbackShift);
        binE->push_back(tmpBin);
        cout << "Marble E: " << tmpPhase << " " << tmpDifficulty << " " << tmpDuration << " " << tmpHoldout << " " << tmpnbackShift << std::endl;
    }
    
    in >> size;
    for (int i = 0; i < size; ++i)
    {
        std::pair<StageRequest, PlayerProgress> level;
        in >> level.first >> level.second;
        scheduleHistoryA.push_back(level);
    }
    
    in >> size;
    for (int i = 0; i < size; ++i)
    {
        std::pair<StageRequest, PlayerProgress> level;
        in >> level.first >> level.second;
        scheduleHistoryB.push_back(level);
    }
    
    in >> size;
    for (int i = 0; i < size; ++i)
    {
        std::pair<StageRequest, PlayerProgress> level;
        in >> level.first >> level.second;
        scheduleHistoryC.push_back(level);
    }
    
    in >> size;
    for (int i = 0; i < size; ++i)
    {
        std::pair<StageRequest, PlayerProgress> level;
        in >> level.first >> level.second;
        scheduleHistoryD.push_back(level);
    }
    
    in >> size;
    for (int i = 0; i < size; ++i)
    {
        std::pair<StageRequest, PlayerProgress> level;
        in >> level.first >> level.second;
        scheduleHistoryE.push_back(level);
    }
}

void LevelScheduler::loadScheduler1_1(std::istream& in)
{
    std::cout << "Loading Scheduler 1.1\n";
    in  >> nBackLevelA
    >> nBackLevelB
    >> nBackLevelC
    >> nBackLevelD
    >> nBackLevelE
    >> scoreCurr
    >> holdoutOffsetA
    >> holdoutOffsetB
    >> holdoutOffsetD
    >> holdoutLevelA
    >> holdoutLevelB
    >> holdoutLevelD
    >> speedA
    >> speedB
    >> speedC
    >> speedD
    >> speedE
    >> firstTimeA
    >> firstTimeB
    >> firstTimeC
    >> firstTimeD
    >> firstTimeE
    >> playCount;
    initTutorialLevels();
    
    int size;
    int tmpPhase;
    int tmpDifficulty;
    int tmpDuration;
    bool tmpHoldout;
    double tmpnbackShift;
    
    // read binA
    in >> size;
    if (binA) delete binA;
    binA = new std::list<Bin>();
    for (int i = 0; i < size; ++i)
    {
        in >> tmpPhase >> tmpDifficulty >> tmpDuration >> tmpHoldout >> tmpnbackShift;
        Bin tmpBin = Bin((LevelPhase)tmpPhase, (StageDifficulty)tmpDifficulty, (StageDuration)tmpDuration, tmpHoldout, tmpnbackShift);
        binA->push_back(tmpBin);
        cout << "Marble A: " << tmpPhase << " " << tmpDifficulty << " " << tmpDuration << " " << tmpHoldout << " " << tmpnbackShift << std::endl;
    }
    
    // read binB
    in >> size;
    if (binB) delete binB;
    binB = new std::list<Bin>();
    for (int i = 0; i < size; ++i)
    {
        in >> tmpPhase >> tmpDifficulty >> tmpDuration >> tmpHoldout >> tmpnbackShift;
        Bin tmpBin = Bin((LevelPhase)tmpPhase, (StageDifficulty)tmpDifficulty, (StageDuration)tmpDuration, tmpHoldout, tmpnbackShift);
        binB->push_back(tmpBin);
        cout << "Marble B: " << tmpPhase << " " << tmpDifficulty << " " << tmpDuration << " " << tmpHoldout << " " << tmpnbackShift << std::endl;
    }
    
    // read binC
    in >> size;
    if (binC) delete binC;
    binC = new std::list<Bin>();
    for (int i = 0; i < size; ++i)
    {
        in >> tmpPhase >> tmpDifficulty >> tmpDuration >> tmpHoldout >> tmpnbackShift;
        Bin tmpBin = Bin((LevelPhase)tmpPhase, (StageDifficulty)tmpDifficulty, (StageDuration)tmpDuration, tmpHoldout, tmpnbackShift);
        binC->push_back(tmpBin);
        cout << "Marble C: " << tmpPhase << " " << tmpDifficulty << " " << tmpDuration << " " << tmpHoldout << " " << tmpnbackShift << std::endl;
    }
    
    // read binD
    in >> size;
    if (binD) delete binD;
    binD = new std::list<Bin>();
    for (int i = 0; i < size; ++i)
    {
        in >> tmpPhase >> tmpDifficulty >> tmpDuration >> tmpHoldout >> tmpnbackShift;
        Bin tmpBin = Bin((LevelPhase)tmpPhase, (StageDifficulty)tmpDifficulty, (StageDuration)tmpDuration, tmpHoldout, tmpnbackShift);
        binD->push_back(tmpBin);
        cout << "Marble D: " << tmpPhase << " " << tmpDifficulty << " " << tmpDuration << " " << tmpHoldout << " " << tmpnbackShift << std::endl;
    }
    
    // read binE
    in >> size;
    if (binE) delete binE;
    binE = new std::list<Bin>();
    for (int i = 0; i < size; ++i)
    {
        in >> tmpPhase >> tmpDifficulty >> tmpDuration >> tmpHoldout >> tmpnbackShift;
        Bin tmpBin = Bin((LevelPhase)tmpPhase, (StageDifficulty)tmpDifficulty, (StageDuration)tmpDuration, tmpHoldout, tmpnbackShift);
        binE->push_back(tmpBin);
        cout << "Marble E: " << tmpPhase << " " << tmpDifficulty << " " << tmpDuration << " " << tmpHoldout << " " << tmpnbackShift << std::endl;
    }
    
    in >> size;
    for (int i = 0; i < size; ++i)
    {
        std::pair<StageRequest, PlayerProgress> level;
        in >> level.first >> level.second;
        scheduleHistoryA.push_back(level);
    }
    
    in >> size;
    for (int i = 0; i < size; ++i)
    {
        std::pair<StageRequest, PlayerProgress> level;
        in >> level.first >> level.second;
        scheduleHistoryB.push_back(level);
    }
    
    in >> size;
    for (int i = 0; i < size; ++i)
    {
        std::pair<StageRequest, PlayerProgress> level;
        in >> level.first >> level.second;
        scheduleHistoryC.push_back(level);
    }
    
    in >> size;
    for (int i = 0; i < size; ++i)
    {
        std::pair<StageRequest, PlayerProgress> level;
        in >> level.first >> level.second;
        scheduleHistoryD.push_back(level);
    }
    
    in >> size;
    for (int i = 0; i < size; ++i)
    {
        std::pair<StageRequest, PlayerProgress> level;
        in >> level.first >> level.second;
        scheduleHistoryE.push_back(level);
    }
}

//________________________________________________________________________________________
