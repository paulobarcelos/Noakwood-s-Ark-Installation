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
    
    // boatHoles
	ifstream fboatHoles;
	fboatHoles.open(ofToDataPath("boatHoles.txt").c_str());
	vector <string> strLinesboatHoles;
	while (!fboatHoles.eof()) {
		string ptStr;
		getline(fboatHoles, ptStr);
		strLinesboatHoles.push_back(ptStr);
	}
	fboatHoles.close();
    vector <string> offsetboatHoles = ofSplitString(strLinesboatHoles[0], ",");
    
    for (int i=1; i<strLinesboatHoles.size()-1; i++) {
		vector <string> pts = ofSplitString(strLinesboatHoles[i], ",");
		if(pts.size() > 0) {
			ofxBox2dPolygon poly;
			for (int j=0; j<pts.size(); j+=2) {
				if(pts[j].size() > 0) {
					float x = ofToFloat(pts[j]) * ofToFloat(offsetboatHoles[2]) + ofToFloat(offsetboatHoles[0]);
					float y = ofToFloat(pts[j+1]) * ofToFloat(offsetboatHoles[2]) + ofToFloat(offsetboatHoles[1]);
					poly.addVertex(x, y);
				}				
			}
            poly.fixture.isSensor = true;
            poly.create(world);
            boatHoles.push_back(poly);			
		}
	}
    
    
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
    
    for (int i=0; i<boatHoles.size(); i++) {
		boatHoles[i].setPosition(position + offset);
	} 
}

void PlayerGame::draw(){
    player->draw();
    
    ofPushStyle();
    ofSetColor(0, 0, 0);
    for (int i=0; i<boat.size(); i++) {
		boat[i].draw();
	}
    ofPopStyle();
    
    ofPushStyle();
    ofSetColor(255, 0, 0);
    for (int i=0; i<boatHoles.size(); i++) {
		boatHoles[i].draw();
	}
    ofPopStyle();
}
