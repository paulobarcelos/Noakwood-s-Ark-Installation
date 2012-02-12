#pragma once

#include "ofMain.h"
#include "ofxBox2d.h"
#include "Constants.h"

class WaterParticle : public ofxBox2dCircle {
	
public:
	void draw() {
        ofPushStyle();
        ofFill();
        ofSetColor(0,0,255);
        ofCircle(getPosition(), getRadius() + 10);
        ofPopStyle();
    };   
    
};

class Water {
public:
    
    void setup(b2World* world, float width, float height);
    void update();
    void draw();
    
    void removeCircle(int label);
    WaterParticle* getNextCircle();
    
    float width;
    float height;
    b2World* world;
    vector <WaterParticle> circles;

    ofFbo waterFbo;
    ofShader blurShader;
    ofShader thresholdShader;
};