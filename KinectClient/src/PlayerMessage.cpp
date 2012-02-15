#include "PlayerMessage.h"

void PlayerMessage::setup(float x, float y, float width, float height, float duration) {
    this->x = x;
    this->y = y;
    this->width = width;
    this->height = height;
    this->duration = duration;
    setDuration(duration);
    
    texture = NULL;
    lastMessage = PlayerMessage::NO_MESSAGE;
    
    noMessage.loadImage("blank.png");
    messages.push_back(noMessage);
    
    greeting.loadImage("messages/greeting.png");
    messages.push_back(greeting);
    
    incentive1.loadImage("messages/incentive1.png");
    messages.push_back(incentive1);
    
    incentive2.loadImage("messages/incentive2.png");
    messages.push_back(incentive2);
    
    incentive3.loadImage("messages/incentive3.png");
    messages.push_back(incentive3);
    
    youWillWin.loadImage("messages/youWillWin.png");
    messages.push_back(youWillWin);
    
    youWillLose.loadImage("messages/youWillLose.png");
    messages.push_back(youWillLose);
    
    arrowLeft.loadImage("messages/arrowLeft.png");
    messages.push_back(arrowLeft);
    
    arrowRight.loadImage("messages/arrowRight.png");
    messages.push_back(arrowRight);
    

}
void PlayerMessage::setDuration(float duration){
    this->duration = duration;
    tweenerMovement.setup(duration, 0, Sine::easeIn);
    tweenerOpacity.setup(duration/2, 0, Sine::easeIn, BACK_AND_FORTH);
}
void PlayerMessage::update(){
    float dt = 1.f / ofGetFrameRate();
    if(texture){
        tweenerMovement.update(dt);
        tweenerOpacity.update(dt);
        
        if(tweenerMovement.isComplete()){
            prepareNextMessage();
        }
    }
}

void PlayerMessage::draw(){
    if(texture){
        if(texture->isAllocated()){
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
}
void PlayerMessage::queueMessage(Message message){
    lastMessage = message;
    queue.push_back(message);
    if(!texture){
        prepareNextMessage();
    } 
}
void PlayerMessage::queueMessageOnce(Message message){
    if(message != lastMessage) queueMessage(message);
}
void PlayerMessage::prepareNextMessage(){
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

ofImage * PlayerMessage::getMessageTexture(Message message){
    switch (message) {
        case PlayerMessage::NO_MESSAGE:
            return &noMessage;
            break;
        case PlayerMessage::GREETING:
            return &greeting;
            break;
        case PlayerMessage::INCENTIVE_1:
            return &incentive1;
            break;
        case PlayerMessage::INCENTIVE_2:
            return &incentive2;
            break;
        case PlayerMessage::INCENTIVE_3:
            return &incentive3;
            break;
        case PlayerMessage::YOU_WILL_WIN:
            return &youWillWin;
            break;
        case PlayerMessage::YOU_WILL_LOSE:
            return &youWillLose;
            break;
        case PlayerMessage::ARROW_LEFT:
            return &arrowLeft;
            break;
        case PlayerMessage::ARROW_RIGHT:
            return &arrowRight;
            break;
    }
    return NULL;
}