#pragma once

#include "ofMain.h"
#include "ofxOsc.h"
#include "ofxBox2d.h"
#include "Player.h"

#define BOAT_VERTICAL_OFFSET 300

enum ShapeType{
    UNDEFINED_SHAPE = 0,
    WATER = 1,
    SENSOR_IN = 2,
    SENSOR_OUT = 3
};

class Data {
public:
    Data(){
        type = UNDEFINED_SHAPE;
        isActive = false;
    };
    int	 label;
    ShapeType type;
    bool isActive;
};

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