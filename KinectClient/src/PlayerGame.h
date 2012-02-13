#pragma once

#include "ofMain.h"
#include "ofxOsc.h"
#include "ofxBox2d.h"
#include "Constants.h"
#include "Player.h"
#include "Water.h"

class PlayerGame : public b2QueryCallback{
public:
    enum Layout{
        LEFT,
        RIGHT
    };
        
	void setup(b2World* world, Player* player, Water* water, Layout layout, float width, float height);
    void reset();
    void update();
    void draw();

private:    
    b2World* world;
    Player* player;
    Water* water;    
    Layout layout;
    float width;
    float height;
    
    int points;
        
    // AAQQ
    bool ReportFixture(b2Fixture* fixture);
    ofRectangle sensorArea;
};