#include "CentralMessage.h"

void CentralMessage::setup(float width, float height, float duration) {
    this->width = width;
    this->height = height;
    this->duration = duration;
    
    tweenerMovement.setup(duration, 0, Sine::easeIn);
    tweenerOpacity.setup(duration/2, 0, Sine::easeIn, BACK_AND_FORTH);
    
    
    ready.loadImage("messages/ready.png");
    messages.push_back(ready);
    
    set.loadImage("messages/set.png");
    messages.push_back(set);
    
    go.loadImage("messages/go.png");
    messages.push_back(go);
    
    sinkWarning.loadImage("messages/sinkWarning.png");
    messages.push_back(sinkWarning);
    
    timeUp.loadImage("messages/timeUp.png");
    messages.push_back(timeUp);
    
    gameOver.loadImage("messages/gameOver.png");
    messages.push_back(gameOver);

}

void CentralMessage::update(){
    float dt = 1.f / ofGetFrameRate();
    if(texture){
        tweenerMovement.update(dt);
        tweenerOpacity.update(dt);
        
        if(tweenerMovement.isComplete()){
            prepareNextMessage();
        }
    }
}

void CentralMessage::draw(){
    if(texture){
        ofPushMatrix();
            ofTranslate(width / 2, height / 2);
            ofScale(scale, scale);
            ofPushStyle();
                ofSetColor(255, 255, 255, opacity * (float) 255);
                texture->draw(0,0);
            ofPopStyle();
        ofPopMatrix();
    }    
}
void CentralMessage::queueMessage(Message message){
    queue.push_back(message);
    if(!texture){
        prepareNextMessage();
    }
}
void CentralMessage::prepareNextMessage(){
    ofImage * nextMessage = NULL;
    if( queue.size() > 0 ){
        Message message = queue[0];
        switch (message) {
            case CentralMessage::READY:
                nextMessage = &ready;
                break;
            case CentralMessage::SET:
                nextMessage = &set;
                break;
            case CentralMessage::GO:
                nextMessage = &go;
                break;
            case CentralMessage::SINK_WARNING:
                nextMessage = &sinkWarning;
                break;
            case CentralMessage::TIME_UP:
                nextMessage = &timeUp;
                break;
            case CentralMessage::GAME_OVER:
                nextMessage = &gameOver;
                break;
        }
        
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
    texture = nextMessage;
}