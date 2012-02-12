#pragma once

#include "ofMain.h"
#include "ofxBox2d.h"
#include "SimpleTweener.h"
#include "Constants.h"
#include "PlayerGame.h"
#include "Boat.h"
#include "Water.h"

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
    
    enum GameState{
        START,
        TRANSITION_TO_PLAYING,
        PLAYING,
        TRANSITION_TO_END,
        END,
        TRANSITION_TO_START
    };
    
	void setup(float width, float height);
    void setData(ofxOscMessage &m);
    void update();
    void draw();
    
    void initStart();
    void initTransitionToPlaying();
    void initPlaying();
    void initTransitionToEnd();
    void initEnd();
    void initTransitionToStart();
    
    PlayerSkin* getRandomSkin();
    
    
    void contactStart(ofxBox2dContactArgs &e);
   
    
    CustomOfxBox2d box2d;
    
    float width;
    float height;
    
    GameState state;
    
    // Use as the timer for each state
    SimpleTweener startTimer;
    SimpleTweener transitionToPlayingTimer;
    SimpleTweener playingTimer;
    SimpleTweener transitionToEndTimer;
    SimpleTweener endTimer;
    SimpleTweener transitionToStartTimer;
    
    PlayerGame* game1;
    PlayerGame* game2;
    
    Player* player1;
    Player* player2;
    
    Boat boat;
    
    Water water;
    
    vector <PlayerSkin*> skins;    
    int currentSkinID;
    
    // Animation specific tweenrs
    SimpleTweener boatPositionTweener;
};