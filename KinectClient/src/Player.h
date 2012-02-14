#pragma once

#include "ofMain.h"
#include "ofxOsc.h"
#include "ofxBox2d.h"

#define PLAYER_IDLE_COUNT_MAX 20


struct Limb {
    Limb(){
        angle = 0;
        isExtremity = true;
    }
    
	float thickness;
	float length;
    float width;
	float height;
    ofImage texture;
    ofxBox2dRect body;
    ofxBox2dJoint joint;
    ofPoint center;
	ofPoint origin;
	ofPoint end;
	float angle;
    bool isExtremity;
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
    void set(string textureFile, float boneRatio = 1.f, ofPoint textureAnchor = ofPoint(0.5, 0.5), float thickness = 20){
        ofImage temp;
        temp.loadImage(textureFile);
        this->textureFile = textureFile;
        this->width = temp.getHeight();
        this->height = temp.getWidth();
        this->textureAnchor = textureAnchor;
        this->thickness = thickness;
        this->length = this->width * boneRatio;
       
    };
    void set(float boneLength, float thickness = 20){
        this->textureFile = "blank.png";
        this->width = 0;
        this->height = 0;
        this->textureAnchor = ofPoint(0.5, 0.5);
        this->thickness = thickness;
        this->length = boneLength;
    };
    float thickness;
	float length;
    float width;
	float height;
    ofPoint textureAnchor;
    string textureFile;
};
struct PlayerSkin {
    
    PlayerSkin(){
        globalScale = 1.0;
        name = "Unnamed Player";
    }
    
    float globalScale;
    
    string name;
    
    string iconFile;
    string iconDeadFile;
    
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

struct LimbSkinSettings{
    float boneRatio;
    ofPoint anchorPercent;
    float boneThickness;
};
struct InvisibleLimbSkinSettings{
    float length;
    float boneThickness;
};

class Player {
	public:
    Player(){
        isReady = false;  
    };
    
    static void loadSkin(string name, PlayerSkin* skin);
    static void loadLimbSkinSettings(string file, LimbSkinSettings* settings);
    static void loadInvisibleLimbSkinSettings(string file, InvisibleLimbSkinSettings* settings);
	
	void setup(b2World* world, PlayerSkin* skin, ofPoint position = ofPoint(ofGetWidth()/2, ofGetHeight()/2));    
    void setData(ofxOscMessage &m);
    void update();
    void updateLimb(Limb &limb);
    void draw();
    
    void setupLimb(Limb &limb, LimbSkin skin);
    void connectLimb(Limb &limbA, Limb &limbB);
	void calculateLimb(Limb &limb, ofPoint origin, ofPoint src, ofPoint dst);
	void drawLimb(Limb &limb);
    
    void flagIdle();
    void flagActive();
    
    bool active;
    int idleCount;
    
    RawData data;
    PlayerSkin* skin;
    
    ofPoint position;
    
    ofImage placeholder;
    
    ofImage icon;
    ofImage iconDead;
    
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
	
    bool isReady;	
};