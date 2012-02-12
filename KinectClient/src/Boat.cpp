#include "Boat.h"

void Boat::setup(b2World* world) {
    this->world = world;
    
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
    scale = ofToFloat(offset[2]);
    
    for (int i=1; i<strLines.size()-1; i++) {
		vector <string> pts = ofSplitString(strLines[i], ",");
		if(pts.size() > 0) {
			ofxBox2dPolygon poly;
			for (int j=0; j<pts.size(); j+=2) {
				if(pts[j].size() > 0) {
					float x = ofToFloat(pts[j]) * scale + ofToFloat(offset[0]);
					float y = ofToFloat(pts[j+1]) * scale + ofToFloat(offset[1]);
					poly.addVertex(x, y);
				}				
			}
            poly.setPhysics(9999, 0, 0);
            poly.create(world);
            b2MassData massData;
            massData.mass = 999999999;
            massData.center = b2Vec2(0,0);
            massData.I = 0;
            poly.body->SetMassData(&massData);
            boat.push_back(poly);		
		}
	}
    
    texture.loadImage("boat.png");
    texture.setAnchorPercent(0.5, 1);
    
}
void Boat::update(){
    float time = 1.f/ofGetFrameRate();
    for (int i=0; i<boat.size(); i++) {
        //boat[i].setPosition(position.x, position.y - 128);
		ofPoint nextPosition(position.x, position.y - 128);
        ofPoint currentPositon(boat[i].body->GetPosition().x, boat[i].body->GetPosition().y);
        ofPoint distance = nextPosition / OFX_BOX2D_SCALE - currentPositon;
        ofPoint velocity = distance / time;
        boat[i].setVelocity(velocity);
        
        // Apply angle
        float nextAngle = 0;
        float currentAngle = boat[i].body->GetAngle();
        float deltaAngle = ofWrapRadians(nextAngle * DEG_TO_RAD - currentAngle);
        float angleVelocity = deltaAngle / time;
        boat[i].body->SetAngularVelocity(angleVelocity);
        
        // Cancel gravity
        boat[i].body->ApplyForce( boat[i].body->GetMass() * - world->GetGravity(), boat[i].body->GetWorldCenter() );
	}
}

void Boat::draw(){
    ofPushMatrix();
    ofTranslate(position);
    ofScale(scale, scale);
    texture.draw(0,0);
    ofPopMatrix();
    
    for (int i=0; i<boat.size(); i++) 
        boat[i].draw();
}
