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
            ofCircle(getPosition(), getRadius()+35);
        ofPopStyle();
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
    
    ofxBox2d box2d;
    int circleLabel;
	vector <Water> circles;
    vector <ofxBox2dPolygon> boat;
    
    ofFbo waterFbo;
    ofShader blurShader;
};