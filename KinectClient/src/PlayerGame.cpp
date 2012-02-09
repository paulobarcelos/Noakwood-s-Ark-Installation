#include "PlayerGame.h"

void PlayerGame::setup(b2World* world, Player* player) {
    this->world = world;
    this->player = player;    
   
    
    // Boat
	ifstream f;
	f.open(ofToDataPath("boat.txt").c_str());
	vector <string> strLines;
	while (!f.eof()) {
		string ptStr;
		getline(f, ptStr);
		strLines.push_back(ptStr);
	}
	f.close();
    vector <string> offset = ofSplitString(strLines[0], ",");
    
    for (int i=1; i<strLines.size()-1; i++) {
		vector <string> pts = ofSplitString(strLines[i], ",");
		if(pts.size() > 0) {
			ofxBox2dPolygon poly;
			for (int j=0; j<pts.size(); j+=2) {
				if(pts[j].size() > 0) {
					float x = ofToFloat(pts[j]) * ofToFloat(offset[2]) + ofToFloat(offset[0]);
					float y = ofToFloat(pts[j+1]) * ofToFloat(offset[2]) + ofToFloat(offset[1]);
					poly.addVertex(x, y);
				}				
			}
            poly.create(world);
            polyLines.push_back(poly);			
		}
	}
    
    
}

void PlayerGame::setData(ofxOscMessage &m){
    player->setData(m);
}

void PlayerGame::update(){
    player->position = position + offset;
    player->update();
    
    for (int i=0; i<polyLines.size(); i++) {
		polyLines[i].setPosition(position + offset);
	}   
}

void PlayerGame::draw(){
    player->draw();
    for (int i=0; i<polyLines.size(); i++) {
		polyLines[i].draw();
	}    
}
