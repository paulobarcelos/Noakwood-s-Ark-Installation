#pragma once

#include "ofMain.h"
#include "SimpleTweener.h"



class BaseMessage {
public:
    enum Message{};
        
	virtual void setup(float x, float y, float width, float height, float duration = 0.5);
    virtual void update();
    virtual void draw();
    
    virtual void queueMessage(Message message);
    
private:
    
    virtual void prepareNextMessage();
    virtual ofImage* getMessageTexture(Message message){};
    
    
    float x;
    float y;
    float width;
    float height;
    float duration;
    
    float scale;
    float opacity;
    
    vector<ofImage> messages;
    
    ofImage* texture;
    
    vector<Message> queue; 
    
    SimpleTweener tweenerMovement;
    SimpleTweener tweenerOpacity;
};