#pragma once

#include "ofMain.h"
#include "ofxOsc.h"
#include "ofxBox2d.h"
#include "Constants.h"
#include "Player.h"
#include "Water.h"
#include "PlayerMessage.h"

#define TOP_ICON_SIZE 138

class PlayerGame : public b2QueryCallback{
public:
    enum Layout{
        LEFT,
        RIGHT
    };
        
	void setup(b2World* world, Player* player, Water* water, Layout layout, float width, float height, float gameDuration);
    void reset();
    void update();
    void draw();
    int getPoints();

private:    
    b2World* world;
    Player* player;
    Water* water;    
    Layout layout;
    float width;
    float height;
    float gameDuration;

    PlayerMessage playerMessage;
    
    int points;
    int pointsControl;
    float pointUsage;
    float rawPointUsage;
    float currentTime;
    float currentTimeNormalized;
    
    bool hasArrowWarning;
    bool hasPointWarning1;
    bool hasPointWarning2;
    bool hasPointWarning3;
    
    ofTrueTypeFont font;
        
    // AAQQ
    bool ReportFixture(b2Fixture* fixture);
    ofRectangle sensorArea;
};