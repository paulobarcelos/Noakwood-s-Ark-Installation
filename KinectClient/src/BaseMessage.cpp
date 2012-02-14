#include "BaseMessage.h"

void BaseMessage::setup(float x, float y, float width, float height, float duration) {
    this->x = x;
    this->y = y;
    this->width = width;
    this->height = height;
    this->duration = duration;
    
    tweenerMovement.setup(duration, 0, Sine::easeIn);
    tweenerOpacity.setup(duration/2, 0, Sine::easeIn, BACK_AND_FORTH);
}

void BaseMessage::update(){
    float dt = 1.f / ofGetFrameRate();
    if(texture){
        tweenerMovement.update(dt);
        tweenerOpacity.update(dt);
        
        if(tweenerMovement.isComplete()){
            prepareNextMessage();
        }
    }
}

void BaseMessage::draw(){
    if(texture){
        ofPushMatrix();
            ofTranslate(x + width / 2, y + height / 2);
            ofScale(scale, scale);
            ofPushStyle();
                ofSetColor(255, 255, 255, opacity * (float) 255);
                texture->draw(0,0);
            ofPopStyle();
        ofPopMatrix();
    }    
}
void BaseMessage::queueMessage(Message message){
    queue.push_back(message);
    if(!texture){
        prepareNextMessage();
    }
}
void BaseMessage::prepareNextMessage(){
    ofImage * nextMessage = NULL;
    if( queue.size() > 0 ){
        nextMessage = getMessageTexture(queue[0]);
        if(nextMessage){
            nextMessage->setAnchorPercent(0.5, 0.5);
            
            queue.erase(queue.begin());
            
            scale = 0.5;
            tweenerMovement.clearTweens();
            tweenerMovement.addTween(&scale, 1.5);
            tweenerMovement.start();
            
            opacity = 0;
            tweenerOpacity.clearTweens();
            tweenerOpacity.addTween(&opacity, 1);
            tweenerOpacity.start();
        }        
    }
    texture = nextMessage;
}