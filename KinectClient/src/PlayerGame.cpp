#include "PlayerGame.h"

void PlayerGame::setup(b2World* world, Player* player) {
    this->world = world;
    this->player = player;    
   
    
    sensor.setup(world, 0, 0, 2000, 2);
    
    sensor.setData(new Data());
    Data * data = (Data*) sensor.getData();
    data->type = SENSOR_IN;
    
    sensorOffset.set(0, 0);
    
    
    waterLevel = 0;
}

void PlayerGame::setData(ofxOscMessage &m){
    player->setData(m);
}

void PlayerGame::update(){
    
    player->position = position + offset;
    player->update();
    
    sensor.setPosition(position + offset + sensorOffset);

}

void PlayerGame::draw(){
    player->draw();
    sensor.draw();
}

void PlayerGame::contactStart(ofxBox2dContactArgs &e) {
	if(e.a != NULL && e.b != NULL) {
        Data * sensorIn = NULL;
        Data * water = NULL;
        
        Data * aData = (Data*)e.a->GetBody()->GetUserData();        
        if(aData) {
            if(aData->type == WATER) water =  aData;
            else if(aData->type == SENSOR_IN) sensorIn = aData;
        }
        
        Data * bData  = (Data*)e.b->GetBody()->GetUserData();        
        if(bData) {
            if(bData->type == WATER) water =  bData;
            else if(bData->type == SENSOR_IN) sensorIn = bData;
        }
        
        
		if( sensorIn && water ){           
            water->isActive = true;
            waterLevel++;
        }
	}
}