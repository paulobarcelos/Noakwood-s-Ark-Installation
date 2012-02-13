#include "PlayerGame.h"

void PlayerGame::setup(b2World* world, Player* player, Water* water, Layout layout, float width, float height) {
    this->world = world;
    this->water = water;
    this->player = player;
    this->layout = layout;
    this->width = width;
    this->height = height;
    
    
    sensorArea.set(0, height, 10000, 100);
    switch (layout) {
        case PlayerGame::LEFT:
            sensorArea.x = width/2 - sensorArea.width;
            break;            
        case PlayerGame::RIGHT:
            sensorArea.x = width/2;
            break;
    }
}
void PlayerGame::reset(){
    points = 0;
}
void PlayerGame::update(){
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
        case PlayerGame::LEFT:
            ofPushMatrix();
                ofTranslate(50, 50);
                player->icon.draw(0,0, TOP_ICON_SIZE, TOP_ICON_SIZE);
                ofDrawBitmapString(ofToString(points), 0,TOP_ICON_SIZE);
            ofPopMatrix();
            
            break;            
        case PlayerGame::RIGHT:
            ofPushMatrix();
                ofTranslate(width - 50 - TOP_ICON_SIZE, 50);
                player->icon.draw(0,0, TOP_ICON_SIZE, TOP_ICON_SIZE);
                ofDrawBitmapString(ofToString(points), 0,TOP_ICON_SIZE);
            ofPopMatrix();
            break;
    }
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