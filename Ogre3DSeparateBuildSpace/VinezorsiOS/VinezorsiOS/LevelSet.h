//
//  LevelSet.h
//  Vinezors2.0
//
//  Created by Calvin Phung on 4/25/14.
//
//

#ifndef __Vinezors2_0__LevelSet__
#define __Vinezors2_0__LevelSet__

#include "Util.h"

// Number of Columns and Rows
#define NUM_TASKS 6
#define NUM_LEVELS 8

struct CollectionCriteria
{
    int nback;
    bool collected;
    
    CollectionCriteria(int n) : nback(n), collected(false) {}
};

struct StageRequest
{
    int nback;
    float stageTime;
    std::vector<NavigationLevel> navLevels;
    std::vector<CollectionCriteria> collectionCriteria;
    std::vector<PowerupType> powerups;
    std::string nameTunnelTile;
    std::string nameSkybox;
    std::string nameMusic;
    int tunnelSectionsPerNavLevel;
    char phase;
    bool hasHoldout;
    int holdoutFrequency;
    float initCamSpeed;
    float minCamSpeed;
    float maxCamSpeed;
    
    StageRequest() : nback(0), stageTime(120.0), navLevels(), collectionCriteria(), powerups(), nameTunnelTile(""), nameSkybox(""), nameMusic(""), tunnelSectionsPerNavLevel(10), phase('A'), hasHoldout(false), holdoutFrequency(4), initCamSpeed(20.0), minCamSpeed(15.0), maxCamSpeed(25.0) {}
};

class LevelSet
{
private:
    std::vector<std::vector<StageRequest> > stageList;
public:
    LevelSet() : stageList() {}
    
    // Returns true whether the level exists.
    // Refer to player.h isLevelAvailable to determine whether the player reqs meet playability
    bool hasLevel(int levelSelect) const;
    bool hasLevel(int row, int col) const;
    bool hasLevelRow(int row) const;
    
    StageRequest retrieveLevel(int levelSelect) const;
    StageRequest retrieveLevel(int row, int col) const;
    int getLevelNo(int row, int col) const; // Level Selection (row, col) -> (n-back, task)
    int getLevelRow(int level) const;
    int getLevelCol(int level) const;
    int getNumLevels() const;
    int getTotalRowRequirement(int row) const; // Star requirement to unlock next row
    void initializeLevelSet();
};

#endif /* defined(__Vinezors2_0__LevelSet__) */
