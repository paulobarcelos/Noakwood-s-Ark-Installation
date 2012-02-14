#include "CentralMessage.h"

void CentralMessage::setup(float x, float y, float width, float height, float duration) {
    this->x = x;
    this->y = y;
    this->width = width;
    this->height = height;
    setDuration(duration);
    
    texture = NULL;
    lastMessage = CentralMessage::NO_MESSAGE;
    
    noMessage.loadImage("blank.png");
    messages.push_back(noMessage);
    
    eye.loadImage("messages/eye.png");
    messages.push_back(eye);
    
    ready.loadImage("messages/ready.png");
    messages.push_back(ready);
    
    intro1.loadImage("messages/intro1.png");
    messages.push_back(intro1);
    
    intro2.loadImage("messages/intro2.png");
    messages.push_back(intro2);
    
    set.loadImage("messages/set.png");
    messages.push_back(set);
    
    go.loadImage("messages/go.png");
    messages.push_back(go);
    
    sinkWarning.loadImage("messages/sinkWarning.png");
    messages.push_back(sinkWarning);
    
    iDontWannaDie.loadImage("messages/iDontWannaDie.png");
    messages.push_back(iDontWannaDie);
    
    timeUp.loadImage("messages/timeUp.png");
    messages.push_back(timeUp);
    
    gameOver.loadImage("messages/gameOver.png");
    messages.push_back(gameOver);
    
    one.loadImage("messages/one.png");
    messages.push_back(one);
    
    two.loadImage("messages/two.png");
    messages.push_back(two);
    
    three.loadImage("messages/three.png");
    messages.push_back(three);

}
void CentralMessage::setDuration(float duration){
    this->duration = duration;
    tweenerMovement.setup(duration, 0, Sine::easeIn);
    tweenerOpacity.setup(duration/2, 0, Sine::easeIn, BACK_AND_FORTH);
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
void CentralMessage::queueMessage(Message message){
    lastMessage = message;
        queue.push_back(message);
        if(!texture){
            prepareNextMessage();
        } 
}
void CentralMessage::queueMessageOnce(Message message){
    if(message != lastMessage) queueMessage(message);
}
void CentralMessage::prepareNextMessage(){
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

ofImage * CentralMessage::getMessageTexture(Message message){
    switch (message) {
        case CentralMessage::NO_MESSAGE:
            return &noMessage;
            break;
        case CentralMessage::EYE:
            return &eye;
            break;
        case CentralMessage::READY:
            return &ready;
            break;
        case CentralMessage::INTRO_1:
            return &intro1;
            break;
        case CentralMessage::INTRO_2:
            return &intro2;
            break;
        case CentralMessage::SET:
            return &set;
            break;
        case CentralMessage::GO:
            return &go;
            break;
        case CentralMessage::SINK_WARNING:
            return &sinkWarning;
            break;
        case CentralMessage::I_DONT_WANNA_DIE:
            return &iDontWannaDie;
            break;
        case CentralMessage::TIME_UP:
            return &timeUp;
            break;
        case CentralMessage::GAME_OVER:
            return &gameOver;
            break;
        case CentralMessage::ONE:
            return &one;
            break;
        case CentralMessage::TWO:
            return &two;
            break;
        case CentralMessage::THREE:
            return &three;
            break;
    }
    return NULL;
}