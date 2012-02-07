
#include "Player.h"

//--------------------------------------------------------------
void Player::setup(b2World* world) {
    this->world = world;
    
    active = false;
    idleCount = 0;    
    
	setupLimb(spineToShoulderCenter, "image.png", 150.f, 100.f, 150.f, 100.f  );
	setupLimb(spineToHipCenter, "image.png", 40.f, 40.f, 40.f, 40.f );

	setupLimb(hipCenterToHipLeft, "image.png", 50.f, 20.f, 50.f, 20.f );
	setupLimb(hipCenterToHipRight, "image.png", 50.f, 20.f, 50.f, 20.f  );

	setupLimb(shoulderCenterToHead, "image.png", 40.f, 40.f, 40.f, 40.f );
	setupLimb(shoulderCenterToShoulderLeft, "image.png", 40.f, 20.f, 40.f, 20.f );
	setupLimb(shoulderCenterToShoulderRight, "image.png", 40.f, 20.f, 40.f, 20.f );

	setupLimb(shoulderLeftToElbowLeft, "image.png", 100.f, 20.f , 100.f, 20.f );
	setupLimb(elbowLeftToWristLeft, "image.png", 80.f, 20.f, 80.f, 20.f );
	setupLimb(wristLeftToHandLeft, "image.png", 50.f, 20.f , 50.f, 20.f );

	setupLimb(shoulderRightToElbowRight, "image.png", 100.f, 20.f, 100.f, 20.f );
	setupLimb(elbowRightToWristRight, "image.png", 80.f, 20.f, 80.f, 20.f );
	setupLimb(wristRightToHandRight, "image.png", 50.f, 20.f , 50.f, 20.f );

	setupLimb(hipLeftToKneeLeft, "image.png", 100.f, 20.f , 100.f, 20.f );
	setupLimb(kneeLeftToAnkleLeft, "image.png", 80.f, 20.f, 80.f, 20.f );
	setupLimb(ankleLeftToFootLeft, "image.png", 60.f, 20.f , 60.f, 20.f );

	setupLimb(hipRightToKneeRight, "image.png", 100.f, 20.f, 100.f, 20.f );
	setupLimb(kneeRightToAnkleRight, "image.png", 80.f, 20.f, 80.f, 20.f );
	setupLimb(ankleRightToFootRight, "image.png", 60.f, 20.f, 60.f, 20.f );
    
    movingLimit.set( 600, hipLeftToKneeLeft.length + kneeLeftToAnkleLeft.length );

}

void Player::setupLimb(Limb &limb, string imageFile, float lenght, float thickness, float width, float height){
	limb.texture.loadImage(imageFile);
	limb.texture.setAnchorPercent(0.5,0.5);
	limb.thickness = thickness;
	limb.length = lenght;
    limb.width = width;
	limb.height = height;
    
    limb.body.fixture.filter.groupIndex = -1;
    limb.body.setPhysics(3.0, 0.53, 0.1);
    limb.body.setup(world, limb.center.x, limb.center.y, limb.width, limb.height);
    
}

void Player::update(ofxOscMessage &m){
    
    if( m.getArgAsInt32( 0 ) == 1 ){
        flagActive();
        
        data.spine.x = m.getArgAsFloat( 1 );
        data.spine.y = m.getArgAsFloat( 2 );
        data.head.x = m.getArgAsFloat( 3 );
        data.head.y = m.getArgAsFloat( 4 );
        data.shoulderCenter.x = m.getArgAsFloat( 5 );
        data.shoulderCenter.y = m.getArgAsFloat( 6 );
        data.hipCenter.x = m.getArgAsFloat( 7 );
        data.hipCenter.y = m.getArgAsFloat( 8 );
        data.shoulderLeft.x = m.getArgAsFloat( 9 );
        data.shoulderLeft.y = m.getArgAsFloat( 10 );
        data.elbowLeft.x = m.getArgAsFloat( 11 );
        data.elbowLeft.y = m.getArgAsFloat( 12 );
        data.wristLeft.x = m.getArgAsFloat( 13 );
        data.wristLeft.y = m.getArgAsFloat( 14 );
        data.handLeft.x = m.getArgAsFloat( 15 );
        data.handLeft.y = m.getArgAsFloat( 16 );
        data.hipLeft.x = m.getArgAsFloat( 17 );
        data.hipLeft.y = m.getArgAsFloat( 18 );
        data.kneeLeft.x = m.getArgAsFloat( 19 );
        data.kneeLeft.y = m.getArgAsFloat( 20 );
        data.ankleLeft.x = m.getArgAsFloat( 21 );
        data.ankleLeft.y = m.getArgAsFloat( 22 );
        data.footLeft.x = m.getArgAsFloat( 23 );
        data.footLeft.y = m.getArgAsFloat( 24 );
        data.shoulderRight.x = m.getArgAsFloat( 25 );
        data.shoulderRight.y = m.getArgAsFloat( 26 );
        data.elbowRight.x = m.getArgAsFloat( 27 );
        data.elbowRight.y = m.getArgAsFloat( 28 );
        data.wristRight.x = m.getArgAsFloat( 29 );
        data.wristRight.y = m.getArgAsFloat( 30 );
        data.handRight.x = m.getArgAsFloat( 31 );
        data.handRight.y = m.getArgAsFloat( 32 );
        data.hipRight.x = m.getArgAsFloat( 33 );
        data.hipRight.y = m.getArgAsFloat( 34 );
        data.kneeRight.x = m.getArgAsFloat( 35 );
        data.kneeRight.y = m.getArgAsFloat( 36 );
        data.ankleRight.x = m.getArgAsFloat( 37 );
        data.ankleRight.y = m.getArgAsFloat( 38 );
        data.footRight.x = m.getArgAsFloat( 39 );
        data.footRight.y = m.getArgAsFloat( 40 );
        
        data.targetOffset.x = m.getArgAsFloat( 41 );
        data.targetOffset.y = m.getArgAsFloat( 42 );
        
        ofPoint center;
        center.x = position.x + movingLimit.x * data.targetOffset.x;
        center.y = position.y + movingLimit.y * data.targetOffset.y;
        
        calculateLimb(spineToShoulderCenter, center, data.spine, data.shoulderCenter);
        calculateLimb(spineToHipCenter, center, data.spine, data.hipCenter);
        
        calculateLimb(hipCenterToHipLeft, spineToHipCenter.end, data.hipCenter, data.hipLeft);
        calculateLimb(hipCenterToHipRight, spineToHipCenter.end, data.hipCenter, data.hipRight);
        
        calculateLimb(shoulderCenterToHead, spineToShoulderCenter.end, data.shoulderCenter, data.head);
        calculateLimb(shoulderCenterToShoulderLeft, spineToShoulderCenter.end, data.shoulderCenter, data.shoulderLeft);
        calculateLimb(shoulderCenterToShoulderRight, spineToShoulderCenter.end, data.shoulderCenter, data.shoulderRight);
        
        calculateLimb(shoulderLeftToElbowLeft, shoulderCenterToShoulderLeft.end, data.shoulderLeft, data.elbowLeft);
        calculateLimb(elbowLeftToWristLeft, shoulderLeftToElbowLeft.end, data.elbowLeft, data.wristLeft);
        calculateLimb(wristLeftToHandLeft, elbowLeftToWristLeft.end, data.wristLeft, data.handLeft);
        
        calculateLimb(shoulderRightToElbowRight, shoulderCenterToShoulderRight.end, data.shoulderRight, data.elbowRight);
        calculateLimb(elbowRightToWristRight, shoulderRightToElbowRight.end, data.elbowRight, data.wristRight);
        calculateLimb(wristRightToHandRight, elbowRightToWristRight.end, data.wristRight, data.handRight);
        
        calculateLimb(hipLeftToKneeLeft, hipCenterToHipLeft.end, data.hipLeft, data.kneeLeft);
        calculateLimb(kneeLeftToAnkleLeft, hipLeftToKneeLeft.end, data.kneeLeft, data.ankleLeft);
        calculateLimb(ankleLeftToFootLeft, kneeLeftToAnkleLeft.end, data.ankleLeft, data.footLeft);
        
        calculateLimb(hipRightToKneeRight, hipCenterToHipRight.end, data.hipRight, data.kneeRight);
        calculateLimb(kneeRightToAnkleRight, hipRightToKneeRight.end, data.kneeRight, data.ankleRight);
        calculateLimb(ankleRightToFootRight, kneeRightToAnkleRight.end, data.ankleRight, data.footRight);
        
    }
    else{
        flagIdle();
    }   
}

void Player::calculateLimb( Limb &limb, ofPoint origin, ofPoint src, ofPoint dst){
	ofPoint diff = src - dst;
	limb.origin = origin;
	limb.angle = atan2(diff.y, diff.x) * RAD_TO_DEG + 90;
	limb.end = ofPoint(0, limb.length).getRotated(0,0,limb.angle);
	limb.end += origin; 
	limb.center = limb.origin.middle(limb.end);    
    limb.body.setPosition(limb.center);
    //limb.body.body->SetTransform(limb.body.body->GetPosition(), limb.angle * DEG_TO_RAD);
    
}

void Player::draw(){
    if(active){
        drawLimb(spineToShoulderCenter);
        drawLimb(spineToHipCenter);
        
        drawLimb(hipCenterToHipLeft);
        drawLimb(hipCenterToHipRight);
        
        drawLimb(shoulderCenterToHead);
        drawLimb(shoulderCenterToShoulderLeft);
        drawLimb(shoulderCenterToShoulderRight);
        
        drawLimb(shoulderLeftToElbowLeft);
        drawLimb(elbowLeftToWristLeft);
        drawLimb(wristLeftToHandLeft);
        
        drawLimb(shoulderRightToElbowRight);
        drawLimb(elbowRightToWristRight);
        drawLimb(wristRightToHandRight);
        
        drawLimb(hipLeftToKneeLeft);
        drawLimb(kneeLeftToAnkleLeft);
        drawLimb(ankleLeftToFootLeft);
        
        drawLimb(hipRightToKneeRight);
        drawLimb(kneeRightToAnkleRight);
        drawLimb(ankleRightToFootRight);
    }
    else{
        ofPushStyle();
            ofSetColor(255,0,0);
            ofDrawBitmapString("Waiting for player", position);
        ofPopStyle();
    }
    
}
void Player::drawLimb (Limb &limb){
	ofPushStyle();
		ofSetColor(255);
		ofPushMatrix();						
			ofTranslate(limb.center);
			ofRotate(limb.angle - 90);					
			limb.texture.draw(0,0, limb.width, limb.height);
		ofPopMatrix();
	ofPopStyle();
    
    ofPushMatrix();	
    ofPushStyle();
        ofSetColor(255,0,0);
        limb.body.draw();
    ofPopStyle();
    ofPopMatrix();
}

void Player::flagIdle (){
    idleCount ++;
	if( idleCount >= PLAYER_IDLE_COUNT_MAX ){
        active = false;
    }    
}
void Player::flagActive (){
    active = true;    
}

