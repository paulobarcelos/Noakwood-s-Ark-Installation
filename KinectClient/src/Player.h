#pragma once

#include "ofMain.h"
#include "ofxOsc.h"
#include "ofxBox2d.h"

#define PLAYER_IDLE_COUNT_MAX 20


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
struct LimbSkin {
    void set(string textureFile, float horizontalOffset = 0, float verticalOffset = 0){
        ofImage temp;
        temp.loadImage(textureFile);
        this->width = temp.getHeight();
        this->height = temp.getWidth();
        this->thickness = this->height - verticalOffset;
        this->length = this->width - horizontalOffset;
        this->textureFile = textureFile;
    };
    void set(string textureFile, float width, float height, float boneWidth, float boneHeight){
        ofImage temp;
        this->width = height;
        this->height = width;
        this->thickness = boneWidth;
        this->length = boneHeight;
        this->textureFile = textureFile;
    };
    float thickness;
	float length;
    float width;
	float height;
    string textureFile;
};
struct PlayerSkin {    
    LimbSkin spineToShoulderCenter;
	LimbSkin spineToHipCenter;
    
	LimbSkin hipCenterToHipLeft;
	LimbSkin hipCenterToHipRight;
    
	LimbSkin shoulderCenterToHead;
	LimbSkin shoulderCenterToShoulderLeft;
	LimbSkin shoulderCenterToShoulderRight;
    
	LimbSkin shoulderLeftToElbowLeft;
	LimbSkin elbowLeftToWristLeft;
	LimbSkin wristLeftToHandLeft;
    
	LimbSkin shoulderRightToElbowRight;
	LimbSkin elbowRightToWristRight;
	LimbSkin wristRightToHandRight;
    
	LimbSkin hipLeftToKneeLeft;
	LimbSkin kneeLeftToAnkleLeft;
	LimbSkin ankleLeftToFootLeft;
    
	LimbSkin hipRightToKneeRight;
	LimbSkin kneeRightToAnkleRight;
	LimbSkin ankleRightToFootRight;
    
    ofPoint movingLimit;
};

class Player {
	public:
	
	void setup(b2World* world, PlayerSkin skin);    
    void setData(ofxOscMessage &m);
    void update();
    void updateLimb(Limb &limb);
    void draw();
    
    void setupLimb(Limb &limb, LimbSkin skin);
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