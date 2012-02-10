#pragma once

#include "ofMain.h"
#include "ofxBox2d.h"
#include "PlayerGame.h"

class Game {
	public:
    
	void setup(float width, float height);
    void setData(ofxOscMessage &m);
    void update();
    void draw();
    void contactStart(ofxBox2dContactArgs &e);
    void removeCircle(int label);
    ofxBox2dCircle* getNextCircle();
    
    float width;
    float height;
    
    PlayerGame* game1;
    PlayerGame* game2;
    
    Player * player1;
    Player * player2;
    
    ofxBox2d box2d;
    int circleLabel;
	vector <ofxBox2dCircle> circles;
    vector <ofxBox2dPolygon> boat;
};