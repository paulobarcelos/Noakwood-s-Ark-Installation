#include "PlayerGame.h"

void PlayerGame::setup(b2World* world, Player* player) {
    this->world = world;
    this->player = player;    
   
    
    insideBoat.fixture.isSensor = true;
    insideBoat.setup(world, 0, 0, 300, 10);
    
    insideBoatOffset.set(0, 0);

    insideBoat.setData(new Data());
    Data * data = (Data*) insideBoat.getData();
    data->type = SENSOR_IN;
    
    
    waterLevel = 0;
}

void PlayerGame::setData(ofxOscMessage &m){
    player->setData(m);
}

void PlayerGame::update(){
    
    player->position = position + offset;
    player->update();
    
    insideBoat.setPosition(position + offset + insideBoatOffset);

}

void PlayerGame::draw(){
    player->draw();
    insideBoat.draw();
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