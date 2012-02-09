#pragma once

#include "ofMain.h"
#include "ofxOsc.h"
#include "ofxBox2d.h"
#include "Player.h"

class PlayerGame {
	public:
	
	void setup(b2World* world, Player* player);
    void setData(ofxOscMessage &m);
    void update();
    void draw();
    
    vector <ofxBox2dPolygon> boat;
    vector <ofxBox2dPolygon> boatHoles;
    
    ofPoint position;
    ofPoint offset;
    
    Player* player;        
    b2World* world;	
};