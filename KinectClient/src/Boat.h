#pragma once

#include "ofMain.h"
#include "ofxBox2d.h"
#include "PlayerGame.h"

class Boat {
public:    
	void setup(b2World* world);
    void update();
    void draw();
    
    ofPoint position;
    
private:
    b2World* world;
    ofImage texture;  
    vector <ofxBox2dPolygon> boat;
    float scale;
};