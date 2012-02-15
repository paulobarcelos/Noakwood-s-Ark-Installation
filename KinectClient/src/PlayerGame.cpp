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
            playerMessage.setup(0,0, width/2, height, 0.7);
            break;            
        case PlayerGame::RIGHT:
            sensorArea.x = width/2;
            playerMessage.setup(width/2,0, width/2, height, 0.7);
            break;
    }
}
void PlayerGame::reset(){
    points = 0;
    currentTime = 0;
    hasArrowWarning = false;
    pointsControl = 0;
    pointUsage = 0;
    rawPointUsage = 0;
    hasPointWarning1 = 0;
    hasPointWarning2 = 0;
    hasPointWarning3 = 0;
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
                break;
            case PlayerGame::RIGHT:
                playerMessage.queueMessage(PlayerMessage::ARROW_RIGHT);
                playerMessage.queueMessage(PlayerMessage::ARROW_RIGHT);
                break;
        }
    }
    
    // Controls how the user is making points
    int dPoints = points - pointsControl;
    pointsControl = points;    
    rawPointUsage += (float)dPoints;
    rawPointUsage = ofLerp(rawPointUsage, 0, 0.1);
    pointUsage = ofLerp(pointUsage, rawPointUsage, 0.9);    
    float pointUsageNorm = ofClamp(ofMap(pointUsage, 0, 80, 0, 1), 0, 1);
    int score = pointUsageNorm * 10;
    
    if(currentTime > 8.5 && ofGetFrameNum() % 8 == 0){
        
        if(score > 9){
            if(!hasPointWarning3){
                hasPointWarning3 = true;
                hasPointWarning2 = true;
                hasPointWarning1 = true;
                playerMessage.queueMessage(PlayerMessage::POINT_WARNING_3);
                playerMessage.queueMessage(PlayerMessage::POINT_WARNING_3_BONUS);
                points += 1000;
            }            
        }
        else if(score > 5){
            if(!hasPointWarning2){
                hasPointWarning2 = true;
                hasPointWarning1 = true;
                playerMessage.queueMessage(PlayerMessage::POINT_WARNING_2);
                playerMessage.queueMessage(PlayerMessage::POINT_WARNING_2_BONUS);
                points += 250;
            }
        }
        else if(score > 1){
            if(!hasPointWarning1){
                hasPointWarning1 = true;
                playerMessage.queueMessage(PlayerMessage::POINT_WARNING_1);
                playerMessage.queueMessage(PlayerMessage::POINT_WARNING_1_BONUS);
                points += 50;
            }
            
        }                
        
        if(score <= 1) hasPointWarning1 = false;
        if(score <= 5) hasPointWarning2 = false;
        if(score <= 9) hasPointWarning3 = false;
        
        
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
                font.drawString(ofToString((int)((float)points/800.f * 10000)), - TOP_ICON_SIZE - 5, TOP_ICON_SIZE/2 + 22);
                ofPopStyle();
            ofPopMatrix();
            break;
        }
    }
    playerMessage.draw();
}
int PlayerGame::getPoints(){
    return points;
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