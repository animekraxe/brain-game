//
//  Util.cpp
//  Testing
//
//  Created by Calvin Phung on 8/27/13.
//
//

#include "Util.h"

#include <vector>
#include <cstdlib>
#include <sstream>
#include <iomanip>

Util::ConfigGlobal::ConfigGlobal()
{
    sessionTime = 3000.00;
    stageTime = 120.0;
    stageTotalSignals = 60;
    stageTotalTargets1 = 20;
    stageTotalTargets2 = 35;
    stageTotalTargets3 = 45;
    stageTotalCollections = 40;
    stageTotalTargetsVariance = 2;
    stageTimeThreshold1 = 25;
    stageTimeThreshold2 = 50;
    stageTimeThreshold3 = 75;
    set1Repetitions = 3;
    set2Repetitions = 3;
    set3Repetitions = 3;
    gameMode = 1;
    revealColor = 1;
    revealSound = 1;
    revealShape = 1;
    tunnelReferenceForward = Vector3(0, 0, -1);
    tunnelReferenceUpward = Vector3(0, 1, 0);
    tunnelReferenceRight = Vector3(1, 0, 0);
    tunnelMinAngleTurn = 0;
    tunnelMaxAngleTurn = 3;
    tunnelSegmentWidth = 25.0;
    tunnelSegmentDepth = 25.0;
    tunnelSegmentBuffer = 25.0;
    tunnelWallLength = tunnelSegmentWidth / (2 * Math::Cos(Ogre::Radian(Math::PI) / 4) + 1);
    tunnelSegmentsPerSection = 5;
    tunnelSegmentsPerPod = 4;
    tunnelSegmentsPerDistractors = 4;
    tunnelSegmentsBeforeRefresh = tunnelSegmentsPerSection * 2;
    tunnelSectionsPerNavigationUpgrade = 3;
    initialSegmentsFirstPod = 1;
    initialSegmentsFirstDistractors = 3;
    initiationSections = 1;
    vineTOffset = 1.0;
    vineRadius = tunnelSegmentWidth / 15.0;
    vineTransitionSpeed = 5.0;
    podAppearance = 2;
    podHeadRadius = tunnelSegmentWidth / 25.0;
    podStemRadius = tunnelSegmentWidth / 100.0;
    podStemLength = tunnelWallLength / 2.0;
    podRotateSpeed = 5.0;
    podCollisionMin = 0.20;
    podCollisionMax = 0.40;
    distractorCollisionMin = 0.25;
    distractorCollisionMax = 0.35;
    podBinSize1 = 10;
    podBinSize2 = 5;
    podBinSize3 = 3;
    podNBackChance = 33;
    stageTotalDistractorsMin = 1;
    stageTotalDistractorsMax = 1;
    seatLength = podHeadRadius * 2;
    tunnelSections = 5;
    nback = 2;
    control = 1;
    historyMode = -1;
    startingHP = 0;
    HPNegativeLimit = -10;
    HPPositiveLimit = 10;
    HPNegativeCorrectAnswer = 2;
    HPNegativeWrongAnswer = -1;
    HPNegativeDistractor = -1;
    HPPositiveCorrectAnswer = 1;
    HPPositiveWrongAnswer = -2;
    HPPositiveDistractor = -1;
    distractorSpeedPenalty = 0.0;
    distractorTimePenalty = 0.0;
    initCamSpeed = 15.0;
    startupCamSpeed = 60.0;
    globalModifierCamSpeed = 5.0;
    boostModifierCamSpeed = 1.25;
    minCamSpeed = 10.0;
    maxCamSpeed = 30.0;
    nlevelSpeedModifier = 0.8;
    numToSpeedUp = 2;
    numToSpeedDown = 2;
    stepsizeSpeedUp = 1.0;
    stepsizeSpeedDown = -1.0;
    HPBarXRef = 0.05;
    HPBarYRef = 0.05;
    HPBarWidth = 0.9;
    HPBarHeight = 0.05;
    screenWidth = 1024;
    screenHeight = 800;
    setSkyBox = 2;
    setWallPanelTexture = 0;
    setVineShip = VINE_RUNNER_SHIP;
    swipeSensitivity = 12.0;
    swipeInverted = 0;
    combo1MinA = 5;
    combo2MinA = 10;
    combo1MinB = 3;
    combo2MinB = 6;
    numNavPhases = 23;
    numTimePhases = 4;
    numSegmentsWithObstacles = 0;
    previousNumSegmentsWithObstacles = 0;
    
    signalTypes = std::vector<std::vector<PodInfo> >(4);
    signalTypes[POD_SIGNAL_1].push_back(PodInfo(POD_SIGNAL_1, POD_FUEL, POD_COLOR_BLUE, POD_SHAPE_CONE, POD_SOUND_1));
    signalTypes[POD_SIGNAL_2].push_back(PodInfo(POD_SIGNAL_2, POD_FUEL, POD_COLOR_GREEN, POD_SHAPE_SPHERE, POD_SOUND_2));
    signalTypes[POD_SIGNAL_3].push_back(PodInfo(POD_SIGNAL_3, POD_FUEL, POD_COLOR_PINK, POD_SHAPE_DIAMOND, POD_SOUND_3));
    signalTypes[POD_SIGNAL_4].push_back(PodInfo(POD_SIGNAL_4, POD_FUEL, POD_COLOR_YELLOW, POD_SHAPE_TRIANGLE, POD_SOUND_4));
    navMap = std::vector<NavigationLevel>(numNavPhases);
    navMap[0] = NavigationLevel(0, 1, 0);
    navMap[1] = NavigationLevel(1, 2, 0);
    navMap[2] = NavigationLevel(2, 2, 1);
    navMap[3] = NavigationLevel(3, 1, 1);
    navMap[4] = NavigationLevel(4, 3, 0);
    navMap[5] = NavigationLevel(5, 4, 0);
    navMap[6] = NavigationLevel(6, 4, 1);
    navMap[7] = NavigationLevel(7, 3, 1);
    navMap[8] = NavigationLevel(8, 2, 2);
    navMap[9] = NavigationLevel(9, 4, 2);
    navMap[10] = NavigationLevel(10, 3, 2);
    navMap[11] = NavigationLevel(11, 1, 2);
    navMap[12] = NavigationLevel(12, 4, 3);
    navMap[13] = NavigationLevel(13, 2, 3);
    navMap[14] = NavigationLevel(14, 3, 3);
    navMap[15] = NavigationLevel(15, 4, 4);
    navMap[16] = NavigationLevel(16, 3, 4);
    navMap[17] = NavigationLevel(17, 2, 4);
    navMap[18] = NavigationLevel(18, 4, 5);
    navMap[19] = NavigationLevel(19, 3, 5);
    navMap[20] = NavigationLevel(20, 4, 6);
    navMap[21] = NavigationLevel(21, 3, 6);
    navMap[22] = NavigationLevel(22, 4, 7);
    navIndex = 0;
}

// Updates variables that depend on other globals, should call this if a game global has changed
void Util::ConfigGlobal::set()
{
    viewportMainWidth_modeRight = screenWidth - 224;
    viewportMainHeight_modeRight = screenHeight;
    viewportSideWidth_modeRight = 224;
    viewportSideHeight_modeRight = screenHeight;
    
    viewportMainWidth_modeBottom = screenWidth;
    viewportMainHeight_modeBottom = screenHeight - 100;
    viewportSideWidth_modeBottom = screenWidth;
    viewportSideHeight_modeBottom = 100;
    
    viewportMainWidth_modeNone = screenWidth;
    viewportMainHeight_modeNone = screenHeight;
    viewportSideWidth_modeNone = 0;
    viewportSideHeight_modeNone = 0;
    
    label1_posX = screenWidth / 2;
    label1_posY = 3 * screenHeight / 40;
    label2_posX = screenWidth / 80;
    label2_posY = 7 * screenHeight / 40;
    label3_posX = screenWidth - screenWidth / 40;
    label3_posY = 5 * screenHeight / 40;
    label4_posX = screenWidth / 80;
    label4_posY = 5 * screenHeight / 40;
    label5_posX = screenWidth - screenWidth / 40;
    label5_posY = 7 * screenHeight / 40;
    label6_posX = screenWidth / 2;
    label6_posY = screenHeight / 2 - screenHeight / 5;
    label7_posX = screenWidth / 7.5;
    label7_posY = screenHeight - screenHeight / 10;
}

void Util::ConfigGlobal::initPaths(const char* name)
{
    playerName = name;
    
#if defined(OGRE_IS_IOS)
    savePath = Util::getIOSDir() + "/" + playerName + "/" + playerName + ".save";
    configPath = Util::getIOSDir() + "/" + playerName + "/" + playerName + ".conf";
    configBackup = Util::getIOSDir() + "/backup.conf";
#else
    savePath = Util::getOSXDir() + "/" + playerName + "/" + playerName + ".save";
    configPath = Util::getOSXDir() + "/" + playerName + "/" + playerName + ".conf";
    configBackup = Util::getOSXDir() + "/backup.conf";
#endif
    
    //Build log path
    logPath = buildLogPath(playerName);
}

void Util::ConfigGlobal::setConfigValue(std::istream& in, std::string paramName)
{
    if (paramName == "stageID")
        in >> stageID;
    else if (paramName == "sessionTime")
        in >> sessionTime;
    else if (paramName == "stageTime")
        in >> stageTime;
    else if (paramName == "stageTotalSignals")
        in >> stageTotalSignals;
    else if (paramName == "stageTotalTargets1")
        in >> stageTotalTargets1;
    else if (paramName == "stageTotalTargets2")
        in >> stageTotalTargets2;
    else if (paramName == "stageTotalTargets3")
        in >> stageTotalTargets3;
    else if (paramName == "stageTotalCollections")
        in >> stageTotalCollections;
    else if (paramName == "stageTotalTargetsVariance")
        in >> stageTotalTargetsVariance;
    else if (paramName == "stageTimeThreshold1")
        in >> stageTimeThreshold1;
    else if (paramName == "stageTimeThreshold2")
        in >> stageTimeThreshold2;
    else if (paramName == "stageTimeThreshold3")
        in >> stageTimeThreshold3;
    else if (paramName == "set1Repetitions")
        in >> set1Repetitions;
    else if (paramName == "set2Repetitions")
        in >> set2Repetitions;
    else if (paramName == "set3Repetitions")
        in >> set3Repetitions;
    else if (paramName == "nback")
        in >> nback;
    else if (paramName == "control")
        in >> control;
    else if (paramName == "gameMode")
        in >> gameMode;
    else if (paramName == "revealColor")
        in >> revealColor;
    else if (paramName == "revealSound")
        in >> revealSound;
    else if (paramName == "revealShape")
        in >> revealShape;
    else if (paramName == "tunnelMinAngleTurn")
        in >> tunnelMinAngleTurn;
    else if (paramName == "tunnelMaxAngleTurn")
        in >> tunnelMaxAngleTurn;
    else if (paramName == "tunnelSegmentsPerSection")
        in >> tunnelSegmentsPerSection;
    else if (paramName == "tunnelSegmentsPerPod")
        in >> tunnelSegmentsPerPod;
    else if (paramName == "tunnelSegmentsPerDistractors")
        in >> tunnelSegmentsPerDistractors;
    else if (paramName == "initialSegmentsFirstPod")
        in >> initialSegmentsFirstPod;
    else if (paramName == "initialSegmentsFirstDistractors")
        in >> initialSegmentsFirstDistractors;
    else if (paramName == "podRotateSpeed")
        in >> podRotateSpeed;
    else if (paramName == "podAppearance")
        in >> podAppearance;
    else if (paramName == "podBinSize1")
        in >> podBinSize1;
    else if (paramName == "podBinSize2")
        in >> podBinSize2;
    else if (paramName == "podBinSize3")
        in >> podBinSize3;
    else if (paramName == "podCollisionMin")
        in >> podCollisionMin;
    else if (paramName == "podCollisionMax")
        in >> podCollisionMax;
    else if (paramName == "distractorCollisionMin")
        in >> distractorCollisionMin;
    else if (paramName == "distractorCollisionMax")
        in >> distractorCollisionMax;
    else if (paramName == "podNBackChance")
        in >> podNBackChance;
    else if (paramName == "stageTotalDistractorsMin")
        in >> stageTotalDistractorsMin;
    else if (paramName == "stageTotalDistractorsMax")
        in >> stageTotalDistractorsMax;
    else if (paramName == "HPNegativeLimit")
        in >> HPNegativeLimit;
    else if (paramName == "HPPositiveLimit")
        in >> HPPositiveLimit;
    else if (paramName == "HPPositiveCorrectAnswer")
        in >> HPPositiveCorrectAnswer;
    else if (paramName == "HPPositiveWrongAnswer")
        in >> HPPositiveWrongAnswer;
    else if (paramName == "HPPositiveDistractor")
        in >> HPPositiveDistractor;
    else if (paramName == "HPNegativeCorrectAnswer")
        in >> HPNegativeCorrectAnswer;
    else if (paramName == "HPNegativeWrongAnswer")
        in >> HPNegativeWrongAnswer;
    else if (paramName == "HPNegativeDistractor")
        in >> HPNegativeDistractor;
    else if (paramName == "distractorSpeedPenalty")
        in >> distractorSpeedPenalty;
    else if (paramName == "distractorTimePenalty")
        in >> distractorTimePenalty;
    else if (paramName == "initCamSpeed")
        in >> initCamSpeed;
    else if (paramName == "globalModifierCamSpeed")
        in >> globalModifierCamSpeed;
    else if (paramName == "boostModifierCamSpeed")
        in >> boostModifierCamSpeed;
    else if (paramName == "minCamSpeed")
        in >> minCamSpeed;
    else if (paramName == "maxCamSpeed")
        in >> maxCamSpeed;
    else if (paramName == "nlevelSpeedModifier")
        in >> nlevelSpeedModifier;
    else if (paramName == "numToSpeedUp")
        in >> numToSpeedUp;
    else if (paramName == "numToSpeedDown")
        in >> numToSpeedDown;
    else if (paramName == "stepsizeSpeedUp")
        in >> stepsizeSpeedUp;
    else if (paramName == "stepsizeSpeedDown")
        in >> stepsizeSpeedDown;
    else if (paramName == "setSkyBox")
        in >> setSkyBox;
    else if (paramName == "setWallPanelTexture")
        in >> setWallPanelTexture;
    else if (paramName == "setVineShip")
    {
        int input;
        in >> input;
        setVineShip = (VineMeshType)input;
    }
    else if (paramName == "swipeSensitivity")
        in >> swipeSensitivity;
    else if (paramName == "swipeInverted")
        in >> swipeInverted;
    else if (paramName == "combo1MinA")
        in >> combo1MinA;
    else if (paramName == "combo2MinA")
        in >> combo2MinA;
    else if (paramName == "combo1MinB")
        in >> combo1MinB;
    else if (paramName == "combo2MinB")
        in >> combo2MinB;
    else if (paramName == "numSignalTypes")
    {
        int input;
        in >> input;
        signalTypes.clear();
        signalTypes = std::vector<std::vector<PodInfo> >(input);
    }
    else if (paramName == "signalType")
    {
        int input1, input2, input3, input4, input5;
        in >> input1 >> input2 >> input3 >> input4 >> input5;
        assert(input1 >= 0 && input1 < signalTypes.size());
        signalTypes[input1].push_back(PodInfo((PodSignal)input1, (PodMeshType)input2, (PodColor)input3, (PodShape)input4, (PodSound)input5));
    }
    else
    {
        std::cout << "WARNING: UNKNOWN PARAMETER... " << paramName << " IGNORED" << std::endl;
        setMessage("WARNING: Unknown config parameter", MESSAGE_NOTIFY);
    }
}

bool Util::ConfigGlobal::loadConfig(int sid)
{
    std::string check, paramName, colon;
    char nextVal;
    
    std::ifstream in (configPath.c_str());
    if (!in.good())
    {
        in.open(configBackup.c_str());
        std::cout << "Loading config: " << configBackup << std::endl;
    }
    else
        std::cout << "Loading config: " << configPath << std::endl;
    if (!in.good()) return false;
    
    int count = 0;
    do {
        in >> check;
        if (check != "{") {
            std::cout << "ERROR: Config file missing \'{\' in "
            << "at least one Stage configuration" << std::endl;
            return false;
        }
        
        in >> paramName;
        while (paramName != "}" && !in.eof()) {
            in >> colon;
            nextVal = in.peek();
            setConfigValue(in, paramName);
            in >> paramName;
            
            if (colon == "{" || nextVal == '{' || paramName == "{" || in.eof()) {
                std::cout << "ERROR: Config file missing \'}\' in "
                << "at least one Stage configuration." << std::endl;
                in.close();
                return false;
            }
        }
        
        // Sanity check, make sure stageID follows standard value 0.. 1.. 2.. 3
        if (stageID != count)
        {
            std::cout << "ERROR: Config file does not follow standard enumerated stageID starting at 0" << std::endl;
            in.close();
            return false;
        }
        ++count;
        
    } while (stageID != sid && !in.eof());
    
    in.close();
    
    if (stageID == sid) {
        std::cout << "Loaded stageID " << stageID << std::endl;
        return true;
    } else {
        std::cout << "Failed to load stageID " << stageID << std::endl;
        return false;
    }
    
    return false;
}

void Util::ConfigGlobal::setMessage(std::string msg, MessageType type, int size)
{
    if (type > messageType)
    {
        message = msg;
        messageType = type;
        messageSize = size;
    }
}

void Util::ConfigGlobal::clearMessage()
{
    message = "";
    messageType = MESSAGE_NONE;
}

bool Util::ConfigGlobal::setName(const char* name)
{
    std::string test = "subject000";
    playerName = name;
    if (playerName.length() != test.length())
        return false;
    
    if (playerName.substr(0, 7) != "subject")
        return false;
    
    int id = atoi(playerName.substr(7, 3).c_str());
    if (!((id >= 100 && id <= 200) || (id >= 900 && id <= 999)))
        return false;
    return true;
}

std::string Util::ConfigGlobal::buildLogPath(std::string playerName)
{
    //Get Date
    time_t raw = time(0);
    struct tm * timeinfo = localtime( &raw );
    char buffer [80];
    strftime(buffer, 80, "%F", timeinfo);
    
#if defined(OGRE_IS_IOS)
    std::string logPath = Util::getIOSDir() + "/" + playerName + "/"
    + playerName + "-" + std::string(buffer);
#else
    std::string logPath = Util::getOSXDir() + "/" + playerName + "/"
    + playerName + "-" + std::string(buffer);
#endif
    
    int i = 1;
    std::ifstream testExist (std::string(logPath + ".log").c_str());
    
    while (testExist) {
        testExist.close();
        logPath = logPath + "_" + Util::toStringInt(i);
        ++i;
        testExist.open(std::string(logPath + ".log").c_str());
    }
    
    logPath = logPath + ".log";
    
    return logPath;
}

float Util::clamp(float val, float min, float max)
{
    if (val < min)
        val = min;
    if (val > max)
        val = max;
    return val;
}

// Returns the degrees from 0-359 for a direction where SOUTH is 0
int Util::getDegrees(Direction dir)
{
	switch (dir)
	{
        case NORTHWEST:
            return 225;
        case NORTH:
            return 180;
        case NORTHEAST:
            return 135;
        case EAST:
            return 90;
        case SOUTHEAST:
            return 45;
        case SOUTH:
            return 0;
        case SOUTHWEST:
            return 270;
        case WEST:
            return 315;
        default:
            return 0;
	}
}

Direction Util::leftOf(Direction dir)
{
	switch (dir)
	{
        case NORTHWEST:
            return WEST;
        case NORTH:
            return NORTHWEST;
        case NORTHEAST:
            return NORTH;
        case EAST:
            return NORTHEAST;
        case SOUTHEAST:
            return EAST;
        case SOUTH:
            return SOUTHEAST;
        case SOUTHWEST:
            return SOUTH;
        case WEST:
            return SOUTHWEST;
        default:
            return NO_DIRECTION;
	}
}

Direction Util::rightOf(Direction dir)
{
	switch (dir)
	{
        case NORTHWEST:
            return NORTH;
        case NORTH:
            return NORTHEAST;
        case NORTHEAST:
            return EAST;
        case EAST:
            return SOUTHEAST;
        case SOUTHEAST:
            return SOUTH;
        case SOUTH:
            return SOUTHWEST;
        case SOUTHWEST:
            return WEST;
        case WEST:
            return NORTHWEST;
        default:
            return NO_DIRECTION;
	}
}

Direction Util::oppositeOf(Direction dir)
{
	switch (dir)
	{
        case NORTHWEST:
            return SOUTHEAST;
        case NORTH:
            return SOUTH;
        case NORTHEAST:
            return SOUTHWEST;
        case EAST:
            return WEST;
        case SOUTHEAST:
            return NORTHWEST;
        case SOUTH:
            return NORTH;
        case SOUTHWEST:
            return NORTHEAST;
        case WEST:
            return EAST;
        default:
            return NO_DIRECTION;
	}
}

bool Util::doSidesMatch(bool sides1[NUM_DIRECTIONS], bool sides2[NUM_DIRECTIONS])
{
    for (int i = 0; i < NUM_DIRECTIONS; ++i)
        if (sides1[i] != sides2[i])
            return false;
    return true;
}

void Util::setSides(bool sides[NUM_DIRECTIONS], int level, Direction dir)
{
    for (int i = 0; i < NUM_DIRECTIONS; ++i)
        sides[i] = false;
    switch (level)
    {
        case 1:
            sides[leftOf(dir)] = true;
            sides[dir] = true;
            sides[rightOf(dir)] = true;
            break;
        case 2:
            sides[leftOf(leftOf(dir))] = true;
            sides[leftOf(dir)] = true;
            sides[dir] = true;
            sides[rightOf(dir)] = true;
            sides[rightOf(rightOf(dir))] = true;
            break;
        case 3:
            sides[leftOf(leftOf(leftOf(dir)))] = true;
            sides[leftOf(leftOf(dir))] = true;
            sides[leftOf(dir)] = true;
            sides[dir] = true;
            sides[rightOf(dir)] = true;
            sides[rightOf(rightOf(dir))] = true;
            sides[rightOf(rightOf(rightOf(dir)))] = true;
            break;
        default:
            sides[NORTHWEST] = true;
            sides[WEST] = true;
            sides[SOUTHWEST] = true;
            sides[SOUTH] = true;
            sides[SOUTHEAST] = true;
            sides[EAST] = true;
            sides[NORTHEAST] = true;
            sides[NORTH] = true;
            break;
    }
    
}

int Util::getNumSides(int level)
{
    switch (level)
    {
        case 1:
            return 3;
        case 2:
            return 5;
        case 3:
            return 7;
        case 4:
            return 8;
        default:
            return 8;
    }
}

int Util::getNumSides(bool sides[NUM_DIRECTIONS])
{
    int count = 0;
    for (int i = 0; i < NUM_DIRECTIONS; ++i)
        if (sides[i]) ++count;
    return count;
}

Direction Util::randDirection()
{
    return (Direction)(rand() % NUM_DIRECTIONS);
}

Direction Util::randDirection(const bool sides[NUM_DIRECTIONS])
{
    std::vector<Direction> dirs;
    if (sides[NORTHWEST])
        dirs.push_back(NORTHWEST);
    if (sides[NORTH])
        dirs.push_back(NORTH);
    if (sides[NORTHEAST])
        dirs.push_back(NORTHEAST);
    if (sides[EAST])
        dirs.push_back(EAST);
    if (sides[SOUTHEAST])
        dirs.push_back(SOUTHEAST);
    if (sides[SOUTH])
        dirs.push_back(SOUTH);
    if (sides[SOUTHWEST])
        dirs.push_back(SOUTHWEST);
    if (sides[WEST])
        dirs.push_back(WEST);
    
    int randDirIndex = rand() % dirs.size();
    
    return dirs[randDirIndex];
}

Vector3 Util::randVector3()
{
    Vector3 temp(0, 1, 0);
    return temp.randomDeviant(Degree(randRangeFloat(0.0, 180.0)));
}


int Util::randRangeInt(int min, int max)
{
    return min + rand () % (max - min + 1);
}

float Util::randRangeFloat(float min, float max)
{
    return min + static_cast<float>(rand()) / RAND_MAX * (max - min);
}

std::string Util::toStringInt(int value)
{
    std::stringstream ss;
	ss << value;
	return ss.str();
}

std::string Util::toStringFloat(float value)
{
    std::stringstream ss;
    ss << std::setprecision(3) << std::fixed;
	ss << value;
	return ss.str();
}

std::string Util::getOSXDir()
{
    char * dir = getenv("HOME");
    std::string result = "";
    if (dir)
        result = std::string(dir) + "/braingame/Documents";
    else
        return "";
    
    mkdir(result.c_str(), 0777);
    return result;
}

std::string Util::getIOSDir()
{
    const char* dir = OgreFramework::getSingletonPtr()->getMacBundlePath().c_str();
    std::string result = "";
    if (dir)
        result = std::string(dir) + "/../Documents";
    else
        return "";
    
    mkdir(result.c_str(), 0777);
    return result;
}

void Util::drawRect(ManualObject* obj, float x, float y, float width, float height, const ColourValue & col, bool filled)
{
    float left = x;
    float right = x + width;
    float top = -y;
    float bottom = -(y + height);
    
    obj->clear();
    // Range is from x [-1, 1], y [-1, 1]
    obj->setUseIdentityProjection(true);
    obj->setUseIdentityView(true);
    obj->setQueryFlags(0);
    if (filled)
    {
        obj->begin("BaseWhiteNoLighting", Ogre::RenderOperation::OT_TRIANGLE_LIST);
        obj->position(left, top, -1);
        obj->colour(col);
        obj->position(right, top, -1);
        obj->colour(col);
        obj->position(right, bottom, -1);
        obj->colour(col);
        obj->position(left, bottom, -1);
        obj->colour(col);
        obj->quad(3, 2, 1, 0);
        obj->end();
    }
    else
    {
        obj->begin("BaseWhiteNoLighting", Ogre::RenderOperation::OT_LINE_STRIP);
        obj->position(left, top, -1);
        obj->colour(col);
        obj->position(right, top, -1);
        obj->colour(col);
        obj->position(right, bottom, -1);
        obj->colour(col);
        obj->position(left, bottom, -1);
        obj->colour(col);
        obj->position(left, top, -1);
        obj->colour(col);
        obj->end();
    }
    obj->setBoundingBox(Ogre::AxisAlignedBox::BOX_INFINITE);
}

// http://www.ogre3d.org/tikiwiki/tiki-index.php?page=manualspheremeshes
void Util::createSphere(Ogre::SceneManager* sceneMgr, const std::string& strName, float r, int nRings, int nSegments)
{
    ManualObject * manual = sceneMgr->createManualObject(strName);
    manual->begin("BaseWhiteNoLighting", RenderOperation::OT_TRIANGLE_LIST);
    
    float fDeltaRingAngle = (Math::PI / nRings);
    float fDeltaSegAngle = (2 * Math::PI / nSegments);
    unsigned short wVerticeIndex = 0 ;
    
    // Generate the group of rings for the sphere
    for( int ring = 0; ring <= nRings; ring++ ) {
        float r0 = r * sinf (ring * fDeltaRingAngle);
        float y0 = r * cosf (ring * fDeltaRingAngle);
        
        // Generate the group of segments for the current ring
        for(int seg = 0; seg <= nSegments; seg++) {
            float x0 = r0 * sinf(seg * fDeltaSegAngle);
            float z0 = r0 * cosf(seg * fDeltaSegAngle);
            
            // Add one vertex to the strip which makes up the sphere
            manual->position( x0, y0, z0);
            
            manual->normal(Vector3(x0, y0, z0).normalisedCopy());
            manual->textureCoord((float) seg / (float) nSegments, (float) ring / (float) nRings);
            
            if (ring != nRings) {
                // each vertex (except the last) has six indicies pointing to it
                manual->index(wVerticeIndex + nSegments + 1);
                manual->index(wVerticeIndex);
                manual->index(wVerticeIndex + nSegments);
                manual->index(wVerticeIndex + nSegments + 1);
                manual->index(wVerticeIndex + 1);
                manual->index(wVerticeIndex);
                wVerticeIndex ++;
            }
        }; // end for seg
    } // end for ring
    manual->end();
    MeshPtr mesh = manual->convertToMesh(strName);
    mesh->_setBounds( AxisAlignedBox( Vector3(-r, -r, -r), Vector3(r, r, r) ), false );
    
    mesh->_setBoundingSphereRadius(r);
    unsigned short src, dest;
    if (!mesh->suggestTangentVectorBuildParams(VES_TANGENT, src, dest))
    {
        mesh->buildTangentVectors(VES_TANGENT, src, dest);
    }
    
    sceneMgr->destroyManualObject(manual);
}

void Util::createCylinder(Ogre::SceneManager* sceneMgr, const std::string& strName, float r, float h, int nSegments)
{
    ManualObject * manual = sceneMgr->createManualObject(strName);
    manual->begin("BaseWhiteNoLighting", RenderOperation::OT_TRIANGLE_LIST);
    
    float fDeltaSegAngle = (2 * Math::PI / nSegments);
    
    float x0 = 0;
    float y0 = -h / 2;
    float y1 = h / 2;
    float z0 = 0;
    
    manual->position(x0, y0, z0);
    manual->normal(Vector3(x0, y0, z0).normalisedCopy());
    manual->textureCoord(0.0, 0.0);
    manual->position(x0, y1, z0);
    manual->normal(Vector3(x0, y1, z0).normalisedCopy());
    manual->textureCoord(0.0, 1.0);
    
    unsigned short botVerticeIndex = 0;
    unsigned short topVerticeIndex = 1;
    unsigned short iVerticeIndex = 2;
    unsigned short wVerticeIndex = iVerticeIndex;
    
    for (int seg = 0; seg <= nSegments; ++seg)
    {
        x0 = r * sinf(seg * fDeltaSegAngle);
        z0 = r * cosf(seg * fDeltaSegAngle);
        
        manual->position(x0, y0, z0);
        manual->normal(Vector3(x0, 0, z0).normalisedCopy());
        manual->textureCoord((float) seg / (float) nSegments, 0.5);
        manual->position(x0, y1, z0);
        manual->normal(Vector3(x0, 0, z0).normalisedCopy());
        manual->textureCoord((float) seg / (float) nSegments, 0.5);
        
        if (seg > 1)
        {
            manual->triangle(botVerticeIndex, wVerticeIndex + 2, wVerticeIndex);
            manual->triangle(topVerticeIndex, wVerticeIndex + 1, wVerticeIndex + 3);
            manual->triangle(wVerticeIndex, wVerticeIndex + 2, wVerticeIndex + 1);
            manual->triangle(wVerticeIndex + 2, wVerticeIndex + 3, wVerticeIndex + 1);
            
            wVerticeIndex += 2;
        }
    }
    manual->triangle(botVerticeIndex, iVerticeIndex, wVerticeIndex); // last triangles
    manual->triangle(topVerticeIndex, wVerticeIndex + 1, iVerticeIndex + 1);
    manual->triangle(wVerticeIndex, iVerticeIndex, wVerticeIndex + 1);
    manual->triangle(iVerticeIndex, iVerticeIndex + 1, wVerticeIndex + 1);
    manual->end();
    
    MeshPtr mesh = manual->convertToMesh(strName);
    Vector3 bl = Vector3(-r, -h / 2, -r);
    Vector3 tr = Vector3(r, h / 2, r);
    mesh->_setBounds( AxisAlignedBox( bl, tr ), false );
    
    mesh->_setBoundingSphereRadius((tr - bl).length() / 2);
    unsigned short src, dest;
    if (!mesh->suggestTangentVectorBuildParams(VES_TANGENT, src, dest))
    {
        mesh->buildTangentVectors(VES_TANGENT, src, dest);
    }
    
    sceneMgr->destroyManualObject(manual);
}

void Util::createDiamond(Ogre::SceneManager* sceneMgr, const std::string& strName, float w, float h)
{
    ManualObject * manual = sceneMgr->createManualObject(strName);
    manual->begin("BaseWhiteNoLighting", RenderOperation::OT_TRIANGLE_LIST);
    
    manual->position(0, h, 0);
    manual->normal(0, 1, 0);
    manual->textureCoord(0.5, 1.0);
    manual->position(-w, 0, -w);
    manual->normal(-sqrt(2), 0, -sqrt(2));
    manual->textureCoord(0.0, 0.0);
    manual->position(w, 0, -w);
    manual->normal(sqrt(2), 0, -sqrt(2));
    manual->textureCoord(1.0, 0.0);
    manual->position(w, 0, w);
    manual->normal(sqrt(2), 0, sqrt(2));
    manual->textureCoord(1.0, 1.0);
    manual->position(-w, 0, w);
    manual->normal(-sqrt(2), 0, sqrt(2));
    manual->textureCoord(1.0, 0.0);
    manual->position(0, -h, 0);
    manual->normal(0, -1, 0);
    manual->textureCoord(0.5, 0.0);
    
    manual->triangle(4, 3, 0);
    manual->triangle(3, 2, 0);
    manual->triangle(2, 1, 0);
    manual->triangle(1, 4, 0);
    manual->triangle(1, 2, 5);
    manual->triangle(2, 3, 5);
    manual->triangle(3, 4, 5);
    manual->triangle(4, 1, 5);
    manual->end();
    
    MeshPtr mesh = manual->convertToMesh(strName);
    Vector3 bl = Vector3(-w, -h, -w);
    Vector3 tr = Vector3(w, h, w);
    mesh->_setBounds( AxisAlignedBox( bl, tr ), false );
    
    mesh->_setBoundingSphereRadius((tr - bl).length() / 2);
    unsigned short src, dest;
    if (!mesh->suggestTangentVectorBuildParams(VES_TANGENT, src, dest))
    {
        mesh->buildTangentVectors(VES_TANGENT, src, dest);
    }
    
    sceneMgr->destroyManualObject(manual);
}

void Util::createBox(Ogre::SceneManager* sceneMgr, const std::string& strName, float l, float w, float h)
{
    ManualObject * manual = sceneMgr->createManualObject(strName);
    manual->begin("BaseWhiteNoLighting", RenderOperation::OT_TRIANGLE_LIST);
    
    float mag = sqrt(l * l + w * w + h * h);
    
    manual->position(-l, -w, -h);
    manual->normal(-sqrt(mag), -sqrt(mag), -sqrt(mag));
    manual->textureCoord(0.0, 0.0);
    manual->position(l, -w, -h);
    manual->normal(sqrt(mag), -sqrt(mag), -sqrt(mag));
    manual->textureCoord(0.5, 0.0);
    manual->position(l, w, -h);
    manual->normal(sqrt(mag), sqrt(mag), -sqrt(mag));
    manual->textureCoord(0.5, 0.5);
    manual->position(-l, w, -h);
    manual->normal(-sqrt(mag), sqrt(mag), -sqrt(mag));
    manual->textureCoord(0.0, 0.5);
    manual->position(-l, -w, h);
    manual->normal(-sqrt(mag), -sqrt(mag), sqrt(mag));
    manual->textureCoord(0.5, 0.5);
    manual->position(l, -w, h);
    manual->normal(sqrt(mag), -sqrt(mag), sqrt(mag));
    manual->textureCoord(1.0, 0.5);
    manual->position(l, w, h);
    manual->normal(sqrt(mag), sqrt(mag), sqrt(mag));
    manual->textureCoord(1.0, 1.0);
    manual->position(-l, w, h);
    manual->normal(-sqrt(mag), sqrt(mag), sqrt(mag));
    manual->textureCoord(0.5, 1.0);
    manual->quad(3, 2, 1, 0);
    manual->quad(7, 6, 2, 3);
    manual->quad(4, 5, 6, 7);
    manual->quad(0, 1, 5, 4);
    manual->quad(1, 2, 6, 5);
    manual->quad(3, 0, 4, 7);
    
    manual->end();
    
    MeshPtr mesh = manual->convertToMesh(strName);
    Vector3 bl = Vector3(-l, -w, -h);
    Vector3 tr = Vector3(l, w, h);
    mesh->_setBounds( AxisAlignedBox( bl, tr ), false );
    
    mesh->_setBoundingSphereRadius((tr - bl).length() / 2);
    unsigned short src, dest;
    if (!mesh->suggestTangentVectorBuildParams(VES_TANGENT, src, dest))
    {
        mesh->buildTangentVectors(VES_TANGENT, src, dest);
    }
    
    sceneMgr->destroyManualObject(manual);
}

void Util::createPlane(Ogre::SceneManager* sceneMgr, const std::string& strName, float length, float depth)
{
    ManualObject * manual = sceneMgr->createManualObject(strName);
    manual->begin("BaseWhiteNoLighting", RenderOperation::OT_TRIANGLE_LIST);
    
    length += EPSILON;
    depth += EPSILON;
    
    manual->position(-length / 2, 0, -depth / 2);
    manual->normal(0, 1, 0);
    manual->textureCoord(0, 0);
    manual->position(length / 2, 0, -depth / 2);
    manual->normal(0, 1, 0);
    manual->textureCoord(1, 0);
    manual->position(length / 2, 0, depth / 2);
    manual->normal(0, 1, 0);
    manual->textureCoord(1, 1);
    manual->position(-length / 2, 0, depth / 2);
    manual->normal(0, 1, 0);
    manual->textureCoord(0, 1);
    manual->quad(3, 2, 1, 0);
    manual->end();
    
    MeshPtr mesh = manual->convertToMesh(strName);
    float len = length / 2;
    float dep = depth / 2;
    float diag1 = (Vector3(length, 0, depth) - Vector3(-length, 0, -depth)).length() / 2;
    float diag2 = (Vector3(length, 0, -depth) - Vector3(-length, 0, depth)).length() / 2;
    Vector3 bl = Vector3(-len, 0, -dep);
    Vector3 tr = Vector3(len, 0, dep);
    mesh->_setBounds( AxisAlignedBox( bl, tr ), true );
    
    mesh->_setBoundingSphereRadius(diag1 > diag2 ? diag1 : diag2);
    unsigned short src, dest;
    if (!mesh->suggestTangentVectorBuildParams(VES_TANGENT, src, dest))
    {
        mesh->buildTangentVectors(VES_TANGENT, src, dest);
    }
    
    sceneMgr->destroyManualObject(manual);
}

void Util::createSubPlane(Ogre::SceneManager* sceneMgr, ManualObject* manual, float length, float depth, Vector3 loc, Quaternion rot, Vector3 & bl, Vector3 & tr)
{
    manual->begin("BaseWhiteNoLighting", RenderOperation::OT_TRIANGLE_LIST);
    
    length += 2 * Util::EPSILON;
    depth += 2 * Util::EPSILON;
    
    Vector3 p1 = Vector3(-length / 2, 0, -depth / 2);
    Vector3 p2 = Vector3(length / 2, 0, -depth / 2);
    Vector3 p3 = Vector3(length / 2, 0, depth / 2);
    Vector3 p4 = Vector3(-length / 2, 0, depth / 2);
    Vector3 normal = Vector3(0, 1, 0);
    
    p1 = loc + rot * p1;
    p2 = loc + rot * p2;
    p3 = loc + rot * p3;
    p4 = loc + rot * p4;
    normal = rot * normal;
    
    manual->position(p1.x, p1.y, p1.z);
    manual->normal(normal.x, normal.y, normal.z);
    manual->textureCoord(0, 0);
    manual->position(p2.x, p2.y, p2.z);
    manual->normal(normal.x, normal.y, normal.z);
    manual->textureCoord(1, 0);
    manual->position(p3.x, p3.y, p3.z);
    manual->normal(normal.x, normal.y, normal.z);
    manual->textureCoord(1, 1);
    manual->position(p4.x, p4.y, p4.z);
    manual->normal(normal.x, normal.y, normal.z);
    manual->textureCoord(0, 1);
    manual->quad(3, 2, 1, 0);
    manual->end();
    
    bl = Vector3(fmin(bl.x, p1.x), fmin(bl.y, p2.y), fmin(bl.z, p1.z));
    bl = Vector3(fmin(bl.x, p2.x), fmin(bl.y, p2.y), fmin(bl.z, p2.z));
    bl = Vector3(fmin(bl.x, p3.x), fmin(bl.y, p3.y), fmin(bl.z, p3.z));
    bl = Vector3(fmin(bl.x, p4.x), fmin(bl.y, p4.y), fmin(bl.z, p4.z));
    tr = Vector3(fmax(tr.x, p1.x), fmax(tr.y, p2.y), fmax(tr.z, p1.z));
    tr = Vector3(fmax(tr.x, p2.x), fmax(tr.y, p2.y), fmax(tr.z, p2.z));
    tr = Vector3(fmax(tr.x, p3.x), fmax(tr.y, p3.y), fmax(tr.z, p3.z));
    tr = Vector3(fmax(tr.x, p4.x), fmax(tr.y, p4.y), fmax(tr.z, p4.z));
}

void Util::createSegment(Ogre::SceneManager* sceneMgr, const std::string& strName, float length, float depth, const bool sides[NUM_DIRECTIONS])
{
    ManualObject * manual = sceneMgr->createManualObject(strName);
    
	float wallLength = length / (2 * Math::Cos(Ogre::Radian(Math::PI) / 4) + 1);
    
    Vector3 bl = Vector3(Ogre::Math::POS_INFINITY, Ogre::Math::POS_INFINITY, Ogre::Math::POS_INFINITY);
    Vector3 tr = Vector3(Ogre::Math::NEG_INFINITY, Ogre::Math::NEG_INFINITY, Ogre::Math::NEG_INFINITY);
    
    Vector3 move;
    Quaternion rot;
    
    if (sides[NORTHWEST])
    {
        move = Vector3(-(length + wallLength) / 4, (length + wallLength) / 4, 0);
        rot.FromAngleAxis(Degree(225), Vector3(0, 0, 1));
        createSubPlane(sceneMgr, manual, wallLength, depth, move, rot, bl, tr);
    }
    
    if (sides[NORTH])
    {
        move = Vector3(0, length / 2, 0);
        rot.FromAngleAxis(Degree(180), Vector3(0, 0, 1));
        createSubPlane(sceneMgr, manual, wallLength, depth, move, rot, bl, tr);
    }
    
    if (sides[NORTHEAST])
    {
        move = Vector3((length + wallLength) / 4, (length + wallLength) / 4, 0);
        rot.FromAngleAxis(Degree(135), Vector3(0, 0, 1));
        createSubPlane(sceneMgr, manual, wallLength, depth, move, rot, bl, tr);
    }
    
    if (sides[EAST])
    {
        move = Vector3(length / 2, 0, 0);
        rot.FromAngleAxis(Degree(90), Vector3(0, 0, 1));
        createSubPlane(sceneMgr, manual, wallLength, depth, move, rot, bl, tr);
    }
    
    if (sides[SOUTHEAST])
    {
        move = Vector3((length + wallLength) / 4, -(length + wallLength) / 4, 0);
        rot.FromAngleAxis(Degree(45), Vector3(0, 0, 1));
        createSubPlane(sceneMgr, manual, wallLength, depth, move, rot, bl, tr);
    }
    
    if (sides[SOUTH])
    {
        move = Vector3(0, -length / 2, 0);
        rot.FromAngleAxis(Degree(0), Vector3(0, 0, 1));
        createSubPlane(sceneMgr, manual, wallLength, depth, move, rot, bl, tr);
    }
    
    if (sides[SOUTHWEST])
    {
        move = Vector3(-(length + wallLength) / 4, -(length + wallLength) / 4, 0);
        rot.FromAngleAxis(Degree(315), Vector3(0, 0, 1));
        createSubPlane(sceneMgr, manual, wallLength, depth, move, rot, bl, tr);
    }
    
    if (sides[WEST])
    {
        move = Vector3(-length / 2, 0, 0);
        rot.FromAngleAxis(Degree(270), Vector3(0, 0, 1));
        createSubPlane(sceneMgr, manual, wallLength, depth, move, rot, bl, tr);
    }
    
    MeshPtr mesh = manual->convertToMesh(strName);
    mesh->_setBounds( AxisAlignedBox( bl, tr ), true );
    
    float l = (tr - bl).length() / 2;
    mesh->_setBoundingSphereRadius(l);
    unsigned short src, dest;
    if (!mesh->suggestTangentVectorBuildParams(VES_TANGENT, src, dest))
    {
        mesh->buildTangentVectors(VES_TANGENT, src, dest);
    }

    sceneMgr->destroyManualObject(manual);
}

void Util::createDefaultSegments(Ogre::SceneManager* sceneMgr)
{
    bool sides[NUM_DIRECTIONS];
    for (int i = 0; i < NUM_DIRECTIONS; ++i)
        sides[i] = false;
    
    sides[SOUTHEAST] = true;
    sides[SOUTH] = true;
    sides[SOUTHWEST] = true;
    Util::createSegment(sceneMgr, "segmentMesh3", 1.0, 1.0, sides);
    
    sides[WEST] = true;
    sides[EAST] = true;
    Util::createSegment(sceneMgr, "segmentMesh5", 1.0, 1.0, sides);
    
    sides[NORTHWEST] = true;
    sides[NORTHEAST] = true;
    Util::createSegment(sceneMgr, "segmentMesh7", 1.0, 1.0, sides);
    
    sides[NORTH] = true;
    Util::createSegment(sceneMgr, "segmentMesh8", 1.0, 1.0, sides);
}

// Step function to increase speed by 1 or decrease by 1 is not included but could be.
void Util::tuneProficiencyExam(ConfigGlobal & globals, float initSpeed, float lengthPerTarget, float approxTotalTime, float bestTime)
{
    // (NumTargets * LengthPerTarget) / Speed = TimeTaken
    // NumTargets = TimeTaken * Speed / LengthPerTarget
    // NumTargets = 90 * 20 / 60
    //
    // NumTargets = TimeTaken * (InitSpeed + NumTargets / 2) / LengthPerTarget
    //
    // (NumTargets * LengthPerTarget) / (InitSpeed + NumTargets / 2) = TimeTaken
    //
    // NumTargets = 2 * TimeTaken * InitSpeed / (2 * LengthPerTarget - TimeTaken)
    //
    //
    // At tbuff = 30 and Si = 15, then N = 10,
    globals.initCamSpeed = initSpeed;
    
    globals.HPPositiveLimit = (2*bestTime*globals.initCamSpeed)/(2*lengthPerTarget-bestTime);
    globals.HPNegativeLimit = -globals.HPPositiveLimit / 3;
    
    globals.minCamSpeed = globals.initCamSpeed + globals.HPNegativeLimit / 2;
    globals.maxCamSpeed = globals.initCamSpeed + globals.HPPositiveLimit / 2;
    globals.stageTime = approxTotalTime - bestTime;
    
    std::cout << "HP Positive: " << globals.HPPositiveLimit << std::endl;
    std::cout << "Speeda: " << globals.maxCamSpeed << std::endl;
    
    globals.stageTotalSignals = 180; // Enough for constant speed of 30
    globals.stageTotalTargets1 = globals.stageTotalSignals / 3;
    globals.stageTotalTargets2 = globals.stageTotalSignals / 2;
    globals.stageTotalTargets3 = 3 * globals.stageTotalSignals / 4;
    globals.stageTotalTargetsVariance = 0;
    
    globals.startingHP = 0;
    globals.HPPositiveCorrectAnswer = 1;
    globals.HPNegativeCorrectAnswer = 1;
    globals.HPPositiveWrongAnswer = -1;
    globals.HPNegativeWrongAnswer = -1;
    globals.HPPositiveDistractor = 0;
    globals.HPNegativeDistractor = 0;
}

float Util::getModdedLengthByNumSegments(const ConfigGlobal & globals, int numSegments)
{
    return (globals.tunnelSegmentDepth + globals.tunnelSegmentBuffer) / globals.globalModifierCamSpeed * numSegments;
}

void Util::generateMaterials()
{
    /*
     MaterialPtr mat0 =
     MaterialManager::getSingleton().create("PodUnknownMaterial", ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, true);
     mat0->setLightingEnabled(true);
     mat0->setAmbient(0.3, 0.3, 0.3);
     mat0->setDiffuse(0.3, 0.3, 0.3, 1.0);
     mat0->load();
     
     MaterialPtr mat1 =
     MaterialManager::getSingleton().create("PodRedMaterial", ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, true);
     mat1->setLightingEnabled(true);
     mat1->setAmbient(0.5, 0.0, 0.0);
     mat1->setDiffuse(0.5, 0.0, 0.0, 1.0);
     mat1->load();
     
     MaterialPtr mat2 =
     MaterialManager::getSingleton().create("PodGreenMaterial", ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, true);
     mat2->setLightingEnabled(true);
     mat2->setAmbient(0.0, 0.5, 0.0);
     mat2->setDiffuse(0.0, 0.5, 0.0, 1.0);
     mat2->load();
     
     MaterialPtr mat3 =
     MaterialManager::getSingleton().create("PodBlueMaterial", ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, true);
     mat3->setAmbient(0.0, 0.0, 0.5);
     mat3->setDiffuse(0.0, 0.0, 0.5, 1.0);
     mat3->load();
     
     MaterialPtr mat4 =
     MaterialManager::getSingleton().create("PodYellowMaterial", ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, true);
     mat4->setLightingEnabled(true);
     mat4->setAmbient(0.5, 0.5, 0.0);
     mat4->setDiffuse(0.5, 0.5, 0.0, 1.0);
     mat4->load();
     
     MaterialPtr mat5 =
     MaterialManager::getSingleton().create("StemMaterial", ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, true);
     mat5->setLightingEnabled(true);
     mat5->setAmbient(0.25, 0.5, 0.25);
     mat5->setDiffuse(0.25, 0.5, 0.25, 1.0);
     mat5->load();
     
     MaterialPtr mat6 =
     MaterialManager::getSingleton().create("PodShellMaterial", ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, true);
     mat6->setLightingEnabled(true);
     mat6->setAmbient(0.5, 0.0, 0.0);
     mat6->setDiffuse(0.5, 0.0, 0.0, 0.1);
     mat6->load();
     */
    
    /*
     Technique *technique = m->createTechnique();
     Pass *pass = technique->createPass();
     pass->setIlluminationStage(Ogre::IS_PER_LIGHT);
     pass->setVertexColourTracking(Ogre::TVC_DIFFUSE);
     pass->setAmbient(1.0, 0.0, 0.0);
     pass->setLightingEnabled(true);
     */
    
}