#include "Game.h"

void Game::setup(float width, float height) {
    
    this->width = width;
    this->height = height;
    
    box2d.init();
	box2d.setGravity(0, 15);
    ofAddListener(box2d.contactStartEvents, this, &Game::contactStart);
        
    // Bear skin
    PlayerSkin bear;
    bear.globalScale = 0.6;
    bear.spineToShoulderCenter.set("bear/spineToShoulderCenter.png", 0.7, ofPoint(0.5, 0.4));
    bear.spineToHipCenter.set(1);
    
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
    
    bear.movingLimit.set( width, bear.hipLeftToKneeLeft.length + bear.kneeLeftToAnkleLeft.length + bear.ankleLeftToFootLeft.length );
    
    player1 = new Player();
    player1->setup(box2d.getWorld(), bear);
    
    game1 = new PlayerGame();
    game1->setup(box2d.getWorld(), player1);
    game1->position.x = (width / 8) * 2;
    game1->position.y = height - 30;
    game1->sensorOffset.x = -2060;
    
    player2 = new Player();
    player2->setup(box2d.getWorld(), bear);
    
    game2 = new PlayerGame();
    game2->setup(box2d.getWorld(), player2);
    game2->position.x = (width / 8) * 6;
    game2->position.y = height - 30;
    game2->sensorOffset.x = 2060;
    
    
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
		boat[i].setPosition(width/2, height - 128);
	} 
   
    
    // Water
    circleLabel = 800;
    
    for(int i = 0; i < 800; i ++ ){
        float r = ofRandom(5, 8);
        Water circle;
        circle.setPhysics(10, 0, 0);
        circle.setup(box2d.getWorld(), i * -100, -100, r);
        circle.dead = true;
        circles.push_back(circle);
        
        circle.setData(new Data());
		Data * data = (Data*)circle.getData();
        data->type = WATER;
		data->isActive = false;
        data->label = i;
		
	}
    
    ofFbo::Settings s;
	s.width	= width;
	s.height = height;
	s.numColorbuffers = 2;
    waterFbo.allocate(s);
    blurShader.load("", "blur_frag.glsl");
    thresholdShader.load("", "threshold_frag.glsl");
}

void Game::setData(ofxOscMessage &m){
    if ( m.getAddress() == "/player/1" ){
        game1->setData(m);
    }
}

void Game::update(){
    // add some circles every so often
	if(ofGetFrameNum()% 1 == 0) {
        Water* circlePtr = getNextCircle();
        if(circlePtr){
            circlePtr->setPosition(width / 2 + ofRandom(-300,300), height - 50 );
        }
	}
    
    game1->update();
    game2->update();
    
    box2d.update();
}

void Game::draw(){
    game1->draw();
    game2->draw();
    
    
    for (int i=0; i<boat.size(); i++) {
        ofPushStyle();
        ofSetColor(0, 0, 0);
            boat[i].draw();
        ofPopStyle();
	}   
    
    waterFbo.begin();
        ofPushStyle();
        ofEnableAlphaBlending();
            ofClear(0,0,0,0);    
            for(int i=0; i<circles.size(); i++) {	
                circles[i].draw();
            }
        ofPopStyle();
    waterFbo.end();    

    waterFbo.begin();    
    blurShader.begin(); 
    glColor3f(1, 1, 1);    
	for(int i=0; i<4; i++) {
		int srcPos = i % 2;				// attachment to write to
		int dstPos = 1 - srcPos;		// attachment to read from
		glDrawBuffer(GL_COLOR_ATTACHMENT0_EXT + dstPos);	// write to this texture
		ofClear(0, 0, 0, 0);
        
		blurShader.setUniform1i("tex0", 0);
		blurShader.setUniform1f("sampleOffset", i*2+1);
		waterFbo.getTextureReference(srcPos).draw(0, 0, width, height);
	}
    blurShader.end();
    
    waterFbo.end();
    
    thresholdShader.begin();
    thresholdShader.setUniform1i("tex0", 0);
    thresholdShader.setUniform1f("brightPassThreshold", 0.5);
    waterFbo.draw(0,0, width, height);
    thresholdShader.end();
    
    
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
Water* Game::getNextCircle() {
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
	for(std::vector<Water>::iterator it = circles.begin(); it != circles.end(); ++it) {
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