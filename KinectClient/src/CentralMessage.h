#pragma once

#include "ofMain.h"
#include "SimpleTweener.h"



class CentralMessage {
public:
    enum Message{
        NO_MESSAGE = -1,
        EYE,
        READY,
        INTRO_1,
        INTRO_2,
        SET,
        GO,
        SINK_WARNING,
        I_DONT_WANNA_DIE,
        TIME_UP,
        GAME_OVER,
        ONE,
        TWO,
        THREE
    };
        
	void setup(float x, float y, float width, float height, float duration = 0.5);
    void setDuration(float duration);
    void update();
    void draw();
    
    void queueMessage(Message message);
    void queueMessageOnce(Message message);
    
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
    Message lastMessage;
    
    ofImage noMessage;
    ofImage eye;
    ofImage ready;
    ofImage intro1;
    ofImage intro2;
    ofImage set;
    ofImage go;
    ofImage sinkWarning;
    ofImage iDontWannaDie;
    ofImage timeUp;
    ofImage gameOver;
    ofImage one;
    ofImage two;
    ofImage three;
    
    ofImage* texture;
    
    vector<Message> queue; 
    
    SimpleTweener tweenerMovement;
    SimpleTweener tweenerOpacity;
};