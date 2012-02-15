#include "PlayerGame.h"

void PlayerGame::setup(b2World* world, Player* player, Water* water, Layout layout, float width, float height, float gameDuration) {
    this->world = world;
    this->water = water;
    this->player = player;
    this->layout = layout;
    this->width = width;
    this->height = height;
    this->gameDuration = gameDuration;
    
    font.loadFont("FuturaLT-Bold.ttf", 23);
    
    
    sensorArea.set(0, height, 10000, 100);
    switch (layout) {
        case PlayerGame::LEFT:
            sensorArea.x = width/2 - sensorArea.width;
            playerMessage.setup(0,0, width/2, height, 1.5);
            break;            
        case PlayerGame::RIGHT:
            sensorArea.x = width/2;
            playerMessage.setup(width/2,0, width/2, height, 1.5);
            break;
    }
}
void PlayerGame::reset(){
    points = 0;
    currentTime = 0;
    hasArrowWarning = false;
}
void PlayerGame::update(){
    float dt = 1./ ofGetFrameRate();
    currentTime += dt;
    currentTimeNormalized = currentTime / gameDuration;
    
    if(currentTime > 8.5 && !hasArrowWarning){
        hasArrowWarning = true;
        
        switch (layout) {
            case PlayerGame::LEFT:
                playerMessage.queueMessage(PlayerMessage::ARROW_LEFT);
                playerMessage.queueMessage(PlayerMessage::ARROW_LEFT);
                playerMessage.queueMessage(PlayerMessage::ARROW_LEFT);
                break;
            case PlayerGame::RIGHT:
                playerMessage.queueMessage(PlayerMessage::ARROW_RIGHT);
                playerMessage.queueMessage(PlayerMessage::ARROW_RIGHT);
                playerMessage.queueMessage(PlayerMessage::ARROW_RIGHT);
                break;
        }
    }
    
    playerMessage.update();
    
    
    ofPoint lower(sensorArea.x, sensorArea.y);
    lower /= OFX_BOX2D_SCALE;
    
    ofPoint upper(sensorArea.x + sensorArea.width, sensorArea.y + sensorArea.height);
    upper /= OFX_BOX2D_SCALE;
    
    b2AABB aabb;
    aabb.lowerBound.Set(lower.x, lower.y);
    aabb.upperBound.Set(upper.x, upper.y);
    world->QueryAABB( this, aabb );
}

void PlayerGame::draw(){
    switch (layout) {
        case PlayerGame::LEFT:{
            ofPushMatrix();
                ofTranslate(20, 10);
                player->icon.draw(0,0, TOP_ICON_SIZE, TOP_ICON_SIZE);
                int currentAlpha = ofGetStyle().color.a;
                ofPushStyle();
                ofSetColor(50,50,50, currentAlpha);
                font.drawString(ofToString((int)((float)points/800.f * 10000)), TOP_ICON_SIZE + 10, TOP_ICON_SIZE/2 + 22);
                ofPopStyle();
            ofPopMatrix();
            
            break;
        }
        case PlayerGame::RIGHT:{
            ofPushMatrix();
                ofTranslate(width - 20 - TOP_ICON_SIZE, 10);
                player->icon.draw(0,0, TOP_ICON_SIZE, TOP_ICON_SIZE);
                int currentAlpha = ofGetStyle().color.a;
                ofPushStyle();
                ofSetColor(50,50,50, currentAlpha);
                font.drawString(ofToString((int)((float)points/800.f * 10000)), - TOP_ICON_SIZE + 45, TOP_ICON_SIZE/2 + 22);
                ofPopStyle();
            ofPopMatrix();
            break;
        }
    }
    playerMessage.draw();
}

bool PlayerGame::ReportFixture(b2Fixture* fixture) {
    Data * data = (Data*)fixture->GetBody()->GetUserData();        
    if(data) {
        if(data->type == WATER){
            water->removeParticle(data->label);
            points++;
        }
    }
    return true;
}