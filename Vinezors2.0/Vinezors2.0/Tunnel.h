//
//  Tunnel.h
//  Testing
//
//  Created by Calvin Phung on 9/10/13.
//
//

#ifndef __Vinezors2_0__Tunnel__
#define __Vinezors2_0__Tunnel__

#include <list>
#include <vector>
#include "Util.h"
#include "TunnelSlice.h"
#include "History.h"

// Stores the list of tunnel segments
class Tunnel
{
public:
    Ogre::SceneManager* sceneMgr;

	Vector3 start;
	Vector3 end;
    std::list<TunnelSlice*> segments;
    std::list<TunnelSlice*>::iterator current;
	double segmentWidth;
	double segmentDepth;
    Quaternion endRot;
    
    std::vector<SectionInfo> sections;
    std::vector<PodInfo> types;
    int sectionSize;
    int podSegmentSize;
    int sectionIndex;
    int podIndex;
    int renewalSectionCounter;
    int renewalPodCounter;
    
    // Stage attributes
    int nback;
    History* history;
    bool sidesUsed[NUM_DIRECTIONS];
    
    bool done;
public:
	Tunnel();
	Tunnel(Ogre::SceneManager* sceneMgr, Vector3 start, double segmentWidth, double segmentDepth, int nback, int control, Direction sloc, int sectionSize, int podSegmentSize);
	
	Vector3 getStart() const;
	Vector3 getEnd() const;
	Vector3 getCenter() const;
    std::list<TunnelSlice*>::const_iterator getCurrentIterator() const;
    std::list<TunnelSlice*>::const_iterator getBeginIterator() const;
    std::list<TunnelSlice*>::const_iterator getEndIterator() const;
    TunnelSlice* findSliceFromCurrent(Vector3 pos, double tOffset, double & tLeft) const;
	TunnelSlice* getCurrent() const;
	TunnelSlice* getNext(int i) const;
	int getSectionIndex() const;
	int getPodIndex() const;
    History* getHistory() const;
    
	double getSegmentWidth() const;
	double getSegmentDepth() const;
    std::vector<SectionInfo> getSectionInfo() const;
    std::vector<PodInfo> getPodInfo() const;
    Quaternion getNewSegmentQuaternion(Direction dir, int degrees);
    PodType getNBackTest(int section) const;
    int getNBack() const;
    bool hasAvailableSide(Direction side) const;
    
    bool isDone() const;
    void setDone(bool value);
    
	void removeSegment();
    
    SectionInfo getNextSectionInfo() const;
    PodInfo getNextPodInfo(SectionInfo & sectionInfo) const;
    void addSegment(TunnelType segmentType, Direction segmentTurn, int turnDegrees, PodType podType, Direction podLoc, bool podGood);
	void renewSegment(TunnelType segmentType, Direction segmentTurn, int turnDegrees, PodType podType, Direction podLoc, bool podGood);
    void addSection(SectionInfo newSection);
    void renewSection(SectionInfo newSection);
    
	bool renewIfNecessary(Vector3 checkPos);
    
    std::vector<Pod *> findPodCollisions(SceneNode* ent);
    
    void constructTunnel(int size, Quaternion q = Quaternion(1, 0, 0, 0));
    
    void update(double elapsed);
    
	~Tunnel();
};

#endif /* defined(__Testing__Tunnel__) */
