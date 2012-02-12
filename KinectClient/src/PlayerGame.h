#pragma once

#include "ofMain.h"
#include "ofxOsc.h"
#include "ofxBox2d.h"
#include "Constants.h"
#include "Player.h"

class PlayerGame {
	public:
    
	void setup(b2World* world);
    void setPlayer(Player* player);
    void setData(ofxOscMessage &m);
    void update();
    void draw();
    
    ofxBox2dRect sensor;
    
    float waterLevel;
    
    ofPoint position;
    ofPoint offset;
    
    ofPoint sensorOffset;
    
    
    Player* player;        
    b2World* world;	
};