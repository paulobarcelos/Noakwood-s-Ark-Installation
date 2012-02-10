#include "Game.h"

void Game::setup(float width, float height) {
    
    this->width = width;
    this->height = height;
    
    box2d.init();
	box2d.setGravity(0, 10);
	box2d.setFPS(30.0);
    ofAddListener(box2d.contactStartEvents, this, &Game::contactStart);
        
    // Bear skin
    PlayerSkin bear;
    bear.globalScale = 0.7;
    bear.spineToShoulderCenter.set("bear/spineToShoulderCenter.png", 0.7, ofPoint(0.5, 0.4));
    bear.spineToHipCenter.set(0);
    
    bear.hipCenterToHipLeft.set(30);
    bear.hipCenterToHipRight.set(30);
    
    bear.shoulderCenterToHead.set("bear/shoulderCenterToHead.png");
    bear.shoulderCenterToShoulderLeft.set(60);
    bear.shoulderCenterToShoulderRight.set(60);
    
    bear.shoulderLeftToElbowLeft.set("bear/shoulderLeftToElbowLeft.png");
    bear.elbowLeftToWristLeft.set("bear/elbowLeftToWristLeft.png");
    bear.wristLeftToHandLeft.set("bear/wristLeftToHandLeft.png");
    
    bear.shoulderRightToElbowRight.set("bear/shoulderRightToElbowRight.png");
    bear.elbowRightToWristRight.set("bear/elbowRightToWristRight.png");
    bear.wristRightToHandRight.set("bear/wristRightToHandRight.png");
    
    bear.hipLeftToKneeLeft.set("bear/hipLeftToKneeLeft.png");
    bear.kneeLeftToAnkleLeft.set("bear/kneeLeftToAnkleLeft.png");
    bear.ankleLeftToFootLeft.set("bear/ankleLeftToFootLeft.png");
    
    bear.hipRightToKneeRight.set("bear/hipRightToKneeRight.png");
    bear.kneeRightToAnkleRight.set("bear/kneeRightToAnkleRight.png");
    bear.ankleRightToFootRight.set("bear/ankleRightToFootRight.png");
    
    bear.movingLimit.set( width / 2, bear.hipLeftToKneeLeft.length + bear.kneeLeftToAnkleLeft.length );
    
    player1 = new Player();
    player1->setup(box2d.getWorld(), bear);
    
    game1 = new PlayerGame();
    game1->setup(box2d.getWorld(), player1);
    game1->position.x = (width / 8) * 2;
    game1->position.y = height - 220;
    game1->insideBoatOffset.x = -500;
    
    player2 = new Player();
    player2->setup(box2d.getWorld(), bear);
    
    game2 = new PlayerGame();
    game2->setup(box2d.getWorld(), player2);
    game2->position.x = (width / 8) * 6;
    game2->position.y = height - 220;
    game2->insideBoatOffset.x = 500;
    
    
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
            poly.create(box2d.getWorld());
            boat.push_back(poly);		
		}
	}
    for (int i=0; i<boat.size(); i++) {
		boat[i].setPosition(width/2, height);
	} 
   
    
    // Water
    circleLabel = 600;
    
    for(int i = 0; i < 600; i ++ ){
        float r = ofRandom(10, 15);
        ofxBox2dCircle circle;
        circle.setPhysics(5, 0.6, 0);
        circle.setup(box2d.getWorld(), i * -100, -100, r);
        circle.dead = true;
        circles.push_back(circle);
        
        circle.setData(new Data());
		Data * data = (Data*)circle.getData();
        data->type = WATER;
		data->isActive = false;
        data->label = i;
		
	}
}

void Game::setData(ofxOscMessage &m){
    if ( m.getAddress() == "/player/1" ){
        game1->setData(m);
    }
}

void Game::update(){
    // add some circles every so often
	if(ofGetFrameNum()% 3 == 0) {
        ofxBox2dCircle* circlePtr = getNextCircle();
        if(circlePtr){
            circlePtr->setPosition(width / 2 + ofRandom(-10,10), height - 50 );
        }
	}
    
    game1->update();
    game2->update();
    
    box2d.update();
}

void Game::draw(){
    game1->draw();
    game2->draw();
    
    for(int i=0; i<circles.size(); i++) {
        ofFill();
        Data * data = (Data*)circles[i].getData();	
		if(data && data->isActive) ofSetHexColor(0xff0000);
		else ofSetHexColor(0x4ccae9);
		circles[i].draw();
	}
    
    ofPushStyle();
    ofSetColor(0, 0, 0);
    for (int i=0; i<boat.size(); i++) {
		boat[i].draw();
	}
    ofPopStyle();
}

void Game::contactStart(ofxBox2dContactArgs &e) {
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
            removeCircle(water->label);
        }
	}
}
ofxBox2dCircle* Game::getNextCircle() {
    for(int i=0; i<circles.size(); i++) {
        if(circles[i].dead){
            circles[i].dead = false;
            circles[i].setVelocity(0,0);
            circles[i].enableGravity(true);
            return &(circles[i]);
        }
	}
    return NULL;
}
void Game::removeCircle(int label) {
	for(std::vector<ofxBox2dCircle>::iterator it = circles.begin(); it != circles.end(); ++it) {
        Data * data = (Data*)(*it).getData();
        if( data->label == label ){
            (*it).dead = true;
            (*it).setPosition(label * -100, -100);
            (*it).setVelocity(0,0);
            (*it).enableGravity(false);
            break;
        }
        
    }
}