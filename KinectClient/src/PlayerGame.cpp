#include "PlayerGame.h"

void PlayerGame::setup(b2World* world, Player* player) {
    this->world = world;
    this->player = player;    
   
    
    // Boat
	ifstream fboat;
	fboat.open(ofToDataPath("boat.txt").c_str());
	vector <string> strLinesboat;
	while (!fboat.eof()) {
		string ptStr;
		getline(fboat, ptStr);
		strLinesboat.push_back(ptStr);
	}
	fboat.close();
    vector <string> offsetboat = ofSplitString(strLinesboat[0], ",");
    
    for (int i=1; i<strLinesboat.size()-1; i++) {
		vector <string> pts = ofSplitString(strLinesboat[i], ",");
		if(pts.size() > 0) {
			ofxBox2dPolygon poly;
			for (int j=0; j<pts.size(); j+=2) {
				if(pts[j].size() > 0) {
					float x = ofToFloat(pts[j]) * ofToFloat(offsetboat[2]) + ofToFloat(offsetboat[0]);
					float y = ofToFloat(pts[j+1]) * ofToFloat(offsetboat[2]) + ofToFloat(offsetboat[1]);
					poly.addVertex(x, y);
				}				
			}
            poly.create(world);
            boat.push_back(poly);			
		}
	}
    
    insideBoat.fixture.isSensor = true;
    insideBoat.setup(world, 0, 0, 170, 120);

    insideBoat.setData(new Data());
    Data * data = (Data*) insideBoat.getData();
    data->type = SENSOR_IN;
    
}

void PlayerGame::setData(ofxOscMessage &m){
    player->setData(m);
}

void PlayerGame::update(){
    player->position = position + offset;
    player->update();
    
    for (int i=0; i<boat.size(); i++) {
		boat[i].setPosition(position + offset);
	} 
    insideBoat.setPosition(position + offset + ofPoint(0,50));

}

void PlayerGame::draw(){
    player->draw();
    
    ofPushStyle();
    ofSetColor(0, 0, 0);
    for (int i=0; i<boat.size(); i++) {
		boat[i].draw();
	}
    ofPopStyle();
    
    //insideBoat.draw();
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
        }
	}
}

void PlayerGame::contactEnd(ofxBox2dContactArgs &e) {
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
            water->isActive = false;
        }
	}
}