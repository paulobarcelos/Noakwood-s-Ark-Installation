#pragma once

#include "ofMain.h"
#include "ofxBox2d.h"
#include "Constants.h"


class WaterParticle : public ofxBox2dCircle {
	
public:
    WaterParticle(){
        ofxBox2dCircle::ofxBox2dCircle();
        
        vector<int> colors;
        colors.resize(3);
        colors[0] = 0x1e2d53;
        colors[1] = 0x1e2d53;
        colors[2] = 0x1e2d53;
        colors[3] = 0x1e2d53;
        
        
        color = colors[((int)ofRandom(2))];
    }
	void draw() {
        ofPushStyle();
        ofFill();
        ofSetHexColor(color);
        ofCircle(getPosition(), getRadius() + 15);
        ofPopStyle();
    };   
    
    int color;
    
};

class Water {
public:
    void setup(b2World* world, float width, float height);
    void reset();
    void update();
    void draw();
    float getLevel();
    
    void removeParticle(int label);
    WaterParticle* getNextParticle();
private:
    
    void inactivateParticle(WaterParticle* particle);
    
    int capacity;
    int levelCount;
    
    int sideSwitch;
    
    float width;
    float height;
    b2World* world;
    vector <WaterParticle*> particles;

    ofFbo waterFbo;
    ofShader blurShader;
    ofShader thresholdShader;
};