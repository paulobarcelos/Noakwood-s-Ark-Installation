#pragma once

#include "ofMain.h"

struct Limb {
	float thickness;
	float length;
	ofPoint center;
	ofPoint origin;
	ofPoint end;
	float angle;
	ofImage texture;
};


class Player {
	public:
	
	void setup();

	Limb spineToShoulderCenter;
	Limb spineToHipCenter;

	Limb hipCenterToHipLeft;
	Limb hipCenterToHipRight;

	Limb shoulderCenterToHead;
	Limb shoulderCenterToShoulderLeft;
	Limb shoulderCenterToShoulderRight;

	Limb shoulderLeftToElbowLeft;
	Limb elbowLeftToWristLeft;
	Limb wristLeftToHandLeft;

	Limb shoulderRightToElbowRight;
	Limb elbowRightToWristRight;
	Limb wristRightToHandRight;

	Limb hipLeftToKneeLeft;
	Limb kneeLeftToAnkleLeft;
	Limb ankleLeftToFootLeft;

	Limb hipRightToKneeRight;
	Limb kneeRightToAnkleRight;
	Limb ankleRightToFootRight;
	
	void setupLimb(Limb &limb, string imageFile, float lenght, float thickness);
	void calculateLimb(Limb &limb, ofPoint origin, ofPoint src, ofPoint dst);
	void drawLimb(Limb &limb);
};