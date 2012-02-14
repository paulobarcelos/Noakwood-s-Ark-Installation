#pragma once

#include "ofMain.h"
#include "SimpleTweener.h"



class CentralMessage {
public:
    enum Message{
        READY = 0,
        SET,
        GO,
        SINK_WARNING,
        TIME_UP,
        GAME_OVER
    };
        
	void setup(float x, float y, float width, float height, float duration = 0.5);
    void update();
    void draw();
    
    void queueMessage(Message message);
    
private:
    
    void prepareNextMessage();
    ofImage* getMessageTexture(Message message);
    
    
    float x;
    float y;
    float width;
    float height;
    float duration;
    
    float scale;
    float opacity;
    
    vector<ofImage> messages;
    
    ofImage ready;
    ofImage set;
    ofImage go;
    ofImage sinkWarning;
    ofImage timeUp;
    ofImage gameOver;
    
    ofImage* texture;
    
    vector<Message> queue; 
    
    SimpleTweener tweenerMovement;
    SimpleTweener tweenerOpacity;
};