#include "PlayerGame.h"

void PlayerGame::setup(b2World* world) {
    this->world = world;
    player = NULL;    
       
    sensor.setup(world, 0, 0, 2000, 2);
    
    sensor.setData(new Data());
    Data * data = (Data*) sensor.getData();
    data->type = SENSOR_IN;
    
    sensorOffset.set(0, 0);    
    
    waterLevel = 0;
}
void PlayerGame::setData(ofxOscMessage &m){
    if(player)player->setData(m);
}
void PlayerGame::setPlayer(Player* player){
    this->player = player;
}

void PlayerGame::update(){
    
    if(player){
        player->position = position + offset;
    }    
    
    sensor.setPosition(position + offset + sensorOffset);

}

void PlayerGame::draw(){
    sensor.draw();
}