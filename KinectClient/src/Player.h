#pragma once

#include "ofMain.h"
#include "ofxOsc.h"
#include "ofxBox2d.h"
#define PLAYER_IDLE_COUNT_MAX 10


struct Limb {
	float thickness;
	float length;
    float width;
	float height;
	ofPoint center;
	ofPoint origin;
	ofPoint end;
	float angle;
	ofImage texture;
    ofxBox2dRect body;    
};

struct RawData {
    ofPoint spine;
    ofPoint head;
    ofPoint shoulderCenter;
    ofPoint hipCenter;
    ofPoint shoulderLeft;
    ofPoint elbowLeft;
    ofPoint wristLeft;
    ofPoint handLeft;
    ofPoint hipLeft;
    ofPoint kneeLeft;
    ofPoint ankleLeft;
    ofPoint footLeft;
    ofPoint shoulderRight;
    ofPoint elbowRight;
    ofPoint wristRight;
    ofPoint handRight;
    ofPoint hipRight;
    ofPoint kneeRight;
    ofPoint ankleRight;
    ofPoint footRight;
    ofPoint targetOffset;    
};

class Player {
	public:
	
	void setup(b2World* world);    
    void update(ofxOscMessage &m);
    void draw();
    
    void setupLimb(Limb &limb, string imageFile, float lenght, float thickness, float width, float height);
	void calculateLimb(Limb &limb, ofPoint origin, ofPoint src, ofPoint dst);
	void drawLimb(Limb &limb);
    
    void flagIdle();
    void flagActive();
    
    bool active;
    int idleCount;
    
    RawData data;
    
    ofPoint movingLimit;
    
    ofPoint position;
    
	Limb spineToShoulderCenter;
	Limb spineToHipCenter;

	Limb hipCenterToHipLeft;
	Limb hipCenterToHipRight;

	Limb shoulderCenterToHead;
	Limb shoulderCenterToShoulderLeft;
	Limb shoulderCenterToShoulderRight;

	Limb shoulderLeftToElbowLeft;
	Limb elbowLeftToWristLeft;
	Limb wristLeftToHandLeft;

	Limb shoulderRightToElbowRight;
	Limb elbowRightToWristRight;
	Limb wristRightToHandRight;

	Limb hipLeftToKneeLeft;
	Limb kneeLeftToAnkleLeft;
	Limb ankleLeftToFootLeft;

	Limb hipRightToKneeRight;
	Limb kneeRightToAnkleRight;
	Limb ankleRightToFootRight;
    
    b2World* world;
	
	
};