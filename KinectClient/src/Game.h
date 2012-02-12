#pragma once

#include "ofMain.h"
#include "ofxBox2d.h"
#include "PlayerGame.h"

class Water : public ofxBox2dCircle {
	
public:
	void draw() {
        ofPushStyle();
            ofFill();
            ofSetColor(0);
            ofCircle(getPosition(), getRadius() + 10);
        ofPopStyle();
    };   
    
};
/**
 * Custom ofxBox2d that updates it's world accordingly to current FPS and not a fixed one.
 * Also note that as velocityIterations, positionIterations and private members of ofxBox2d
 * setIterations(int velocityTimes, int positionTimes) won't work. Set those isntead via update(int velocityTimes, int positionTimes)
 */
class CustomOfxBox2d : public ofxBox2d {
	
public:
	void update(int velocityTimes = 40, int positionTimes = 20) {
        if(world == NULL) return;
        world->Step(1.f/ofGetFrameRate(), velocityTimes, positionTimes);
    };   
    
};

class Game {
	public:
    
	void setup(float width, float height);
    void setData(ofxOscMessage &m);
    void update();
    void draw();
    void contactStart(ofxBox2dContactArgs &e);
    void removeCircle(int label);
    Water* getNextCircle();
    
    float width;
    float height;
    
    PlayerGame* game1;
    PlayerGame* game2;
    
    Player * player1;
    Player * player2;
    
    CustomOfxBox2d box2d;
    int circleLabel;
	vector <Water> circles;
    vector <ofxBox2dPolygon> boat;
    
    ofFbo waterFbo;
    ofShader blurShader;
    ofShader thresholdShader;
};