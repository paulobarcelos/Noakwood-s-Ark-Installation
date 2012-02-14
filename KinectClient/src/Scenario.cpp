#include "Scenario.h"

void Scenario::setup(float width, float height) {
    this->width = width;
    this->height = height;

    sky.loadImage("scenario/sky.png");
    skyBlue.loadImage("scenario/skyBlue.png");
    skyRed.loadImage("scenario/skyRed.png");
    
    wave1.loadImage("scenario/waveWhite64.png");
    wave2.loadImage("scenario/waveWhite64.png");
    wave3.loadImage("scenario/waveWhite64.png");
    wave4.loadImage("scenario/waveWhite64.png");
    wave5.loadImage("scenario/waveWhite64.png");
    wave6.loadImage("scenario/waveWhite64.png");
    
    wave1.setAnchorPercent(0, 1.f);
    wave2.setAnchorPercent(0, 1.f);
    wave3.setAnchorPercent(0, 1.f);
    wave4.setAnchorPercent(0, 1.f);
    wave5.setAnchorPercent(0, 1.f);
    wave6.setAnchorPercent(0, 1.f);
}

void Scenario::update(){
    float dt = 1.f / ofGetFrameRate();
    waveXTweener1.update(dt);
    waveXTweener2.update(dt);
    waveXTweener3.update(dt);
    waveXTweener4.update(dt);
    waveXTweener5.update(dt);
    waveXTweener6.update(dt);
    
    waveYTweener1.update(dt);
    waveYTweener2.update(dt);
    waveYTweener3.update(dt);
    waveYTweener4.update(dt);
    waveYTweener5.update(dt);
    waveYTweener6.update(dt);
}

void Scenario::drawBackground(){
    ofPushStyle();
    
    if(state == Scenario::LOSE){
        skyRed.draw(0,0, width, sky.getHeight()/sky.getWidth() * width);
    }
    else if( state == Scenario::WIN){
        skyBlue.draw(0,0, width, sky.getHeight()/sky.getWidth() * width);    
    }
    else{
        sky.draw(0,0, width, sky.getHeight()/sky.getWidth() * width);
    }    
    
    ofSetHexColor(0x1a243f);
    wave6.draw(wave6Position);
    ofSetHexColor(0x090931);
    wave5.draw(wave5Position);
    ofSetHexColor(0x1e2d53);
    wave4.draw(wave4Position);
    ofSetHexColor(0x090931);
    wave3.draw(wave3Position);
    ofPopStyle();
}

void Scenario::drawForeground(){
    ofPushStyle();
    ofSetHexColor(0x1a243f);
    wave2.draw(wave2Position);
    ofSetHexColor(0x090931);
    wave1.draw(wave1Position);
    ofPopStyle();
}

void Scenario::setState(State state){
    this->state = state;
    
    float offset = wave1.getHeight() / 4;
    float begin = height + offset * 3;
    
    offset = 30;
    
    wave1Position.set(0, begin);
    wave2Position.set(0, begin - offset * 1);
    wave3Position.set(0, begin - offset * 2);
    wave4Position.set(0, begin - offset * 3);
    wave5Position.set(0, begin - offset * 4);
    wave6Position.set(0, begin - offset * 5);
    
    waveXTweener1.clearTweens();
    waveXTweener2.clearTweens();
    waveXTweener3.clearTweens();
    waveXTweener4.clearTweens();
    waveXTweener5.clearTweens();
    waveXTweener6.clearTweens();
    
    waveYTweener1.clearTweens();
    waveYTweener2.clearTweens();
    waveYTweener3.clearTweens();
    waveYTweener4.clearTweens();
    waveYTweener5.clearTweens();
    waveYTweener6.clearTweens(); 
    
    switch (state) {
        case Scenario::NORMAL:
        case Scenario::WIN:
            waveXTweener1.setup(4.f, 0, Sine::easeInOut, BACK_AND_FORTH);
            waveXTweener2.setup(4.f, 0, Sine::easeInOut, BACK_AND_FORTH);
            waveXTweener3.setup(4.f, 0, Sine::easeInOut, BACK_AND_FORTH);
            waveXTweener4.setup(4.f, 0, Sine::easeInOut, BACK_AND_FORTH);
            waveXTweener5.setup(4.f, 0, Sine::easeInOut, BACK_AND_FORTH);
            waveXTweener6.setup(4.f, 0, Sine::easeInOut, BACK_AND_FORTH);
            
            waveYTweener1.setup(2.f, 0, Sine::easeInOut, BACK_AND_FORTH);
            waveYTweener2.setup(2.f, 0, Sine::easeInOut, BACK_AND_FORTH);
            waveYTweener3.setup(2.f, 0, Sine::easeInOut, BACK_AND_FORTH);
            waveYTweener4.setup(2.f, 0, Sine::easeInOut, BACK_AND_FORTH);
            waveYTweener5.setup(2.f, 0, Sine::easeInOut, BACK_AND_FORTH);
            waveYTweener6.setup(2.f, 0, Sine::easeInOut, BACK_AND_FORTH);
            break;
            
        case Scenario::STRESS:
        case Scenario:: LOSE:
            waveXTweener1.setup(2.f, 0, Sine::easeInOut, BACK_AND_FORTH);
            waveXTweener2.setup(2.f, 0, Sine::easeInOut, BACK_AND_FORTH);
            waveXTweener3.setup(2.f, 0, Sine::easeInOut, BACK_AND_FORTH);
            waveXTweener4.setup(2.f, 0, Sine::easeInOut, BACK_AND_FORTH);
            waveXTweener5.setup(2.f, 0, Sine::easeInOut, BACK_AND_FORTH);
            waveXTweener6.setup(2.f, 0, Sine::easeInOut, BACK_AND_FORTH);
            
            waveYTweener1.setup(1.0f, 0, Sine::easeInOut, BACK_AND_FORTH);
            waveYTweener2.setup(1.0f, 0, Sine::easeInOut, BACK_AND_FORTH);
            waveYTweener3.setup(1.0f, 0, Sine::easeInOut, BACK_AND_FORTH);
            waveYTweener4.setup(1.0f, 0, Sine::easeInOut, BACK_AND_FORTH);
            waveYTweener5.setup(1.0f, 0, Sine::easeInOut, BACK_AND_FORTH);
            waveYTweener6.setup(1.0f, 0, Sine::easeInOut, BACK_AND_FORTH);        
            break;
            
    }
    
    waveXTweener1.addTween(&(wave1Position.x), -wave1.getHeight()/2);
    waveXTweener2.addTween(&(wave2Position.x), -wave2.getHeight()/2);
    waveXTweener3.addTween(&(wave3Position.x), -wave3.getHeight()/2);
    waveXTweener4.addTween(&(wave4Position.x), -wave4.getHeight()/2);
    waveXTweener5.addTween(&(wave5Position.x), -wave5.getHeight()/2);
    waveXTweener6.addTween(&(wave6Position.x), -wave6.getHeight()/2);
    
    waveYTweener1.addTween(&(wave1Position.y), -wave1.getHeight()/4);
    waveYTweener2.addTween(&(wave2Position.y), -wave2.getHeight()/4);
    waveYTweener3.addTween(&(wave3Position.y), -wave3.getHeight()/4);
    waveYTweener4.addTween(&(wave4Position.y), -wave4.getHeight()/4);
    waveYTweener5.addTween(&(wave5Position.y), -wave5.getHeight()/4);
    waveYTweener6.addTween(&(wave6Position.y), -wave6.getHeight()/4);
    
    waveXTweener1.start();
    waveXTweener2.start();
    waveXTweener3.start();
    waveXTweener4.start();
    waveXTweener5.start();
    waveXTweener6.start();
    
    waveYTweener1.start();
    waveYTweener2.start();
    waveYTweener3.start();
    waveYTweener4.start();
    waveYTweener5.start();
    waveYTweener6.start();
    
    waveXTweener1.setProgress(0.0f);
    waveXTweener2.setProgress(0.2f);
    waveXTweener3.setProgress(0.4f);
    waveXTweener4.setProgress(0.6f);
    waveXTweener5.setProgress(0.8f);
    waveXTweener6.setProgress(1.0f);
    
    waveYTweener1.setProgress(0.0f);
    waveYTweener2.setProgress(0.2f);
    waveYTweener3.setProgress(0.4f);
    waveYTweener4.setProgress(0.6f);
    waveYTweener5.setProgress(0.8f);
    waveYTweener6.setProgress(1.0f);
}