#pragma once

#include "ofMain.h"
#include "SimpleTweener.h"


class Scenario {
public:
    enum State{
        NORMAL,
        STRESS,
        WIN,
        LOSE
    };
    void setup(float width, float height);
    void update();
    void drawBackground();
    void drawForeground();
    
    void setState(State state);

private:
    float width;
    float height;
    
    State state;
    
    ofImage sky;
    ofImage skyBlue;
    ofImage skyRed;
    
    ofImage wave1;
    ofImage wave2;
    ofImage wave3;
    ofImage wave4;
    ofImage wave5;
    ofImage wave6;
    
    ofPoint wave1Position;
    ofPoint wave2Position;
    ofPoint wave3Position;
    ofPoint wave4Position;
    ofPoint wave5Position;
    ofPoint wave6Position;
    
    SimpleTweener waveXTweener1;
    SimpleTweener waveXTweener2;
    SimpleTweener waveXTweener3;
    SimpleTweener waveXTweener4;
    SimpleTweener waveXTweener5;
    SimpleTweener waveXTweener6;
    
    SimpleTweener waveYTweener1;
    SimpleTweener waveYTweener2;
    SimpleTweener waveYTweener3;
    SimpleTweener waveYTweener4;
    SimpleTweener waveYTweener5;
    SimpleTweener waveYTweener6;
};