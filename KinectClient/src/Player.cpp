
#include "Player.h"

//--------------------------------------------------------------
void Player::setup() {
	setupLimb(spineToShoulderCenter, "image.jpg", 150.f, 100.f );
	setupLimb(spineToHipCenter, "image.jpg", 40.f, 40.f );

	setupLimb(hipCenterToHipLeft, "image.jpg", 50.f, 20.f );
	setupLimb(hipCenterToHipRight, "image.jpg", 50.f, 20.f );

	setupLimb(shoulderCenterToHead, "image.jpg", 40.f, 40.f );
	setupLimb(shoulderCenterToShoulderLeft, "image.jpg", 40.f, 20.f );
	setupLimb(shoulderCenterToShoulderRight, "image.jpg", 40.f, 20.f );

	setupLimb(shoulderLeftToElbowLeft, "image.jpg", 100.f, 20.f );
	setupLimb(elbowLeftToWristLeft, "image.jpg", 80.f, 20.f );
	setupLimb(wristLeftToHandLeft, "image.jpg", 50.f, 20.f );

	setupLimb(shoulderRightToElbowRight, "image.jpg", 100.f, 20.f );
	setupLimb(elbowRightToWristRight, "image.jpg", 80.f, 20.f );
	setupLimb(wristRightToHandRight, "image.jpg", 50.f, 20.f );

	setupLimb(hipLeftToKneeLeft, "image.jpg", 100.f, 20.f );
	setupLimb(kneeLeftToAnkleLeft, "image.jpg", 80.f, 20.f );
	setupLimb(ankleLeftToFootLeft, "image.jpg", 60.f, 20.f );

	setupLimb(hipRightToKneeRight, "image.jpg", 100.f, 20.f );
	setupLimb(kneeRightToAnkleRight, "image.jpg", 80.f, 20.f );
	setupLimb(ankleRightToFootRight, "image.jpg", 60.f, 20.f );


}

void Player::setupLimb(Limb &limb, string imageFile, float lenght, float thickness){
	limb.texture.loadImage(imageFile);
	limb.texture.setAnchorPercent(0.5,0.5);
	limb.thickness = thickness;
	limb.length = lenght;
}

void Player::calculateLimb( Limb &limb, ofPoint origin, ofPoint src, ofPoint dst){
	ofPoint diff = src - dst;
	limb.origin = origin;
	limb.angle = atan2(diff.y, diff.x) * RAD_TO_DEG + 90;
	limb.end = ofPoint(0, limb.length).getRotated(0,0,limb.angle);
	limb.end += origin; 
	limb.center = limb.origin.middle(limb.end);
}

void Player::drawLimb (Limb &limb){
	ofPushStyle();
		ofSetColor(255);
		ofPushMatrix();						
			ofTranslate(limb.center);
			ofRotate(limb.angle);					
			limb.texture.draw(0,0, limb.thickness, limb.length);
		ofPopMatrix();
	ofPopStyle();
}

