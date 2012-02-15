#pragma once

#include "ofMain.h"
#include "SimpleTweener.h"



class PlayerMessage {
public:
    enum Message{
        NO_MESSAGE = -1,
        GREETING = 0,
        INCENTIVE_1,
        INCENTIVE_2,
        INCENTIVE_3,
        YOU_WILL_WIN,
        YOU_WILL_LOSE,
        ARROW_LEFT,
        ARROW_RIGHT
    };
    
	void setup(float x, float y, float width, float height, float duration = 1.f);
    void update();
    void setDuration(float duration);
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
    ofImage greeting;
    ofImage incentive1;
    ofImage incentive2;
    ofImage incentive3;
    ofImage youWillWin;
    ofImage youWillLose;
    ofImage arrowLeft;
    ofImage arrowRight;
    
    ofImage* texture;
    
    vector<Message> queue; 
    
    SimpleTweener tweenerMovement;
    SimpleTweener tweenerOpacity;
};