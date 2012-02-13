
#include "Player.h"

void Player::setup(b2World* world, PlayerSkin* skin, ofPoint position) {
    this->world = world;
    this->skin = skin;
    active = false;
    idleCount = 0;
    
    this->position = position;
    
    // Setup
	setupLimb(spineToShoulderCenter, skin->spineToShoulderCenter);
	setupLimb(spineToHipCenter, skin->spineToHipCenter);
    setupLimb(shoulderCenterToHead, skin->shoulderCenterToHead);

    setupLimb(shoulderCenterToShoulderLeft, skin->shoulderCenterToShoulderLeft);
	setupLimb(shoulderLeftToElbowLeft, skin->shoulderLeftToElbowLeft);
	setupLimb(elbowLeftToWristLeft, skin->elbowLeftToWristLeft);
	setupLimb(wristLeftToHandLeft, skin->wristLeftToHandLeft);
    
    setupLimb(hipCenterToHipLeft, skin->hipCenterToHipLeft);
    setupLimb(hipLeftToKneeLeft, skin->hipLeftToKneeLeft);
	setupLimb(kneeLeftToAnkleLeft, skin->kneeLeftToAnkleLeft);
	setupLimb(ankleLeftToFootLeft, skin->ankleLeftToFootLeft);

    setupLimb(shoulderCenterToShoulderRight, skin->shoulderCenterToShoulderRight);
	setupLimb(shoulderRightToElbowRight, skin->shoulderRightToElbowRight);
	setupLimb(elbowRightToWristRight, skin->elbowRightToWristRight);
	setupLimb(wristRightToHandRight, skin->wristRightToHandRight);

    setupLimb(hipCenterToHipRight, skin->hipCenterToHipRight);
	setupLimb(hipRightToKneeRight, skin->hipRightToKneeRight);
	setupLimb(kneeRightToAnkleRight, skin->kneeRightToAnkleRight);
	setupLimb(ankleRightToFootRight, skin->ankleRightToFootRight);
    
    icon.loadImage(skin->iconFile);
    iconDead.loadImage(skin->iconDeadFile);
    
    // Connect
    /*connectLimb(spineToShoulderCenter, spineToHipCenter);
    connectLimb(spineToShoulderCenter, shoulderCenterToHead);
    
    connectLimb(shoulderCenterToHead, shoulderCenterToShoulderLeft);
    connectLimb(shoulderCenterToShoulderLeft, shoulderLeftToElbowLeft);
    connectLimb(shoulderLeftToElbowLeft, elbowLeftToWristLeft);
    connectLimb(elbowLeftToWristLeft, wristLeftToHandLeft);
    
    connectLimb(spineToHipCenter, hipCenterToHipLeft);
    connectLimb(hipCenterToHipLeft, hipLeftToKneeLeft);
    connectLimb(hipLeftToKneeLeft, kneeLeftToAnkleLeft);
    connectLimb(kneeLeftToAnkleLeft, ankleLeftToFootLeft);
    
    connectLimb(shoulderCenterToHead, shoulderCenterToShoulderRight);
    connectLimb(shoulderCenterToShoulderRight, shoulderRightToElbowRight);
    connectLimb(shoulderRightToElbowRight, elbowRightToWristRight);
    connectLimb(elbowRightToWristRight, wristRightToHandRight);
    
    connectLimb(spineToHipCenter, hipCenterToHipRight);
    connectLimb(hipCenterToHipRight, hipRightToKneeRight);
    connectLimb(hipRightToKneeRight, kneeRightToAnkleRight);
    connectLimb(kneeRightToAnkleRight, ankleRightToFootRight);*/
    
    isReady = true;
}

void Player::setupLimb(Limb &limb, LimbSkin limbSkin){
	limb.texture.loadImage(limbSkin.textureFile);
	limb.texture.setAnchorPercent(limbSkin.textureAnchor.x, limbSkin.textureAnchor.y);
	limb.thickness = limbSkin.thickness * skin->globalScale;
	limb.length = limbSkin.length * skin->globalScale;
    limb.width = limbSkin.width * skin->globalScale;
	limb.height = limbSkin.height * skin->globalScale;
    
    limb.body.fixture.filter.groupIndex = -1;
    limb.body.setPhysics(5, 0, 0);
    limb.body.setup(world, position.x, position.y, limb.length/2, limb.thickness/2);    
}

void Player::connectLimb(Limb &limbA, Limb &limbB){
    limbA.isExtremity = false;
    limbA.joint.setup(world, limbA.body.body, limbB.body.body, 5.0, 0, false);
    limbA.joint.setLength(limbA.length / 2);
}

void Player::setData(ofxOscMessage &m){
    if(!isReady) return;
    
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
        ofPoint powMovingLimit;
        
        center.y = position.y + powf(skin->movingLimit.y, 1.15) * data.targetOffset.y - skin->movingLimit.y * 0.7;
        center.x = position.x + skin->movingLimit.x * data.targetOffset.x;
        
        calculateLimb(spineToShoulderCenter, center, data.spine, data.shoulderCenter);
        calculateLimb(spineToHipCenter, center, data.spine, data.hipCenter);
        
        calculateLimb(hipCenterToHipLeft, spineToHipCenter.end, data.hipCenter, data.hipLeft);
        calculateLimb(hipCenterToHipRight, spineToHipCenter.end, data.hipCenter, data.hipRight);
        
        calculateLimb(shoulderCenterToHead, spineToShoulderCenter.end, data.shoulderCenter, data.head);
        calculateLimb(shoulderCenterToShoulderLeft, spineToShoulderCenter.end, data.shoulderCenter, data.shoulderLeft);
        calculateLimb(shoulderCenterToShoulderRight, spineToShoulderCenter.end, data.shoulderCenter, data.shoulderRight);
        
        calculateLimb(shoulderLeftToElbowLeft, ofPoint(shoulderCenterToShoulderLeft.end.x, shoulderCenterToShoulderLeft.origin.y), data.shoulderLeft, data.elbowLeft);
        calculateLimb(elbowLeftToWristLeft, shoulderLeftToElbowLeft.end, data.elbowLeft, data.wristLeft);
        calculateLimb(wristLeftToHandLeft, elbowLeftToWristLeft.end, data.wristLeft, data.handLeft);
        
        calculateLimb(shoulderRightToElbowRight, ofPoint(shoulderCenterToShoulderRight.end.x, shoulderCenterToShoulderRight.origin.y), data.shoulderRight, data.elbowRight);
        calculateLimb(elbowRightToWristRight, shoulderRightToElbowRight.end, data.elbowRight, data.wristRight);
        calculateLimb(wristRightToHandRight, elbowRightToWristRight.end, data.wristRight, data.handRight);
        
        calculateLimb(hipLeftToKneeLeft, ofPoint(hipCenterToHipLeft.end.x, hipCenterToHipLeft.origin.y), data.hipLeft, data.kneeLeft);
        calculateLimb(kneeLeftToAnkleLeft, hipLeftToKneeLeft.end, data.kneeLeft, data.ankleLeft);
        calculateLimb(ankleLeftToFootLeft, kneeLeftToAnkleLeft.end, data.ankleLeft, data.footLeft);
        
        calculateLimb(hipRightToKneeRight,  ofPoint(hipCenterToHipRight.end.x, hipCenterToHipRight.origin.y), data.hipRight, data.kneeRight);
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
}

void Player::update(){
    updateLimb(spineToShoulderCenter);
    updateLimb(spineToHipCenter);
    
    updateLimb(hipCenterToHipLeft);
    updateLimb(hipCenterToHipRight);
    
    updateLimb(shoulderCenterToHead);
    updateLimb(shoulderCenterToShoulderLeft);
    updateLimb(shoulderCenterToShoulderRight);
    
    updateLimb(shoulderLeftToElbowLeft);
    updateLimb(elbowLeftToWristLeft);
    updateLimb(wristLeftToHandLeft);
    
    updateLimb(shoulderRightToElbowRight);
    updateLimb(elbowRightToWristRight);
    updateLimb(wristRightToHandRight);
    
    updateLimb(hipLeftToKneeLeft);
    updateLimb(kneeLeftToAnkleLeft);
    updateLimb(ankleLeftToFootLeft);
    
    updateLimb(hipRightToKneeRight);
    updateLimb(kneeRightToAnkleRight);
    updateLimb(ankleRightToFootRight);
}

void Player::updateLimb( Limb &limb ){
    
    float time = 1.f/ofGetFrameRate();
    
    // Apply position
    ofPoint currentPositon(limb.body.body->GetPosition().x, limb.body.body->GetPosition().y);
    ofPoint distance = limb.center / OFX_BOX2D_SCALE - currentPositon;
    ofPoint velocity = distance / time;
    limb.body.setVelocity(velocity);
    
    // Apply angle
    float currentAngle = limb.body.body->GetAngle();
    float deltaAngle = ofWrapRadians((limb.angle - 90) * DEG_TO_RAD - currentAngle);
    float angleVelocity = deltaAngle / time;
    limb.body.body->SetAngularVelocity(angleVelocity);
    
    // Cancel gravity
    limb.body.body->ApplyForce( limb.body.body->GetMass() * - world->GetGravity(), limb.body.body->GetWorldCenter() );
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
}
void Player::drawLimb (Limb &limb){
    ofPoint currentPositon(limb.body.body->GetPosition().x, limb.body.body->GetPosition().y);
    currentPositon = currentPositon * OFX_BOX2D_SCALE;
    float currentAngle = limb.body.body->GetAngle() * RAD_TO_DEG - 90;
    
		ofPushMatrix();            
			ofTranslate(currentPositon);
			ofRotate(currentAngle);					
                limb.texture.draw(0,0, limb.height,limb.width);
		ofPopMatrix();
    
    //limb.body.draw();
}

void Player::flagIdle (){
    idleCount ++;
	if( idleCount >= PLAYER_IDLE_COUNT_MAX ){
        active = false;
    }    
}
void Player::flagActive (){
    idleCount = 0;
    active = true;    
}

void Player::loadSkin(string name, PlayerSkin* skin) {
    skin->name = name;
    
    stringstream spineToShoulderCenter;
    spineToShoulderCenter << name << "/spineToShoulderCenter";
    stringstream spineToShoulderCenterImage;
    spineToShoulderCenterImage << spineToShoulderCenter.str() << ".png";
    LimbSkinSettings spineToShoulderCenterSettings;
    loadLimbSkinSettings(spineToShoulderCenter.str(), &spineToShoulderCenterSettings);
    
    stringstream shoulderCenterToHead;
    shoulderCenterToHead << name << "/shoulderCenterToHead";
    stringstream shoulderCenterToHeadImage;
    shoulderCenterToHeadImage << shoulderCenterToHead.str() << ".png";
    LimbSkinSettings shoulderCenterToHeadSettings;
    loadLimbSkinSettings(shoulderCenterToHead.str(), &shoulderCenterToHeadSettings);
    
    
    stringstream shoulderCenterToShoulderLeft;
    shoulderCenterToShoulderLeft << name << "/shoulderCenterToShoulderLeft";
    InvisibleLimbSkinSettings shoulderCenterToShoulderLeftSettings;
    loadInvisibleLimbSkinSettings(shoulderCenterToShoulderLeft.str(), &shoulderCenterToShoulderLeftSettings);
    
    stringstream shoulderLeftToElbowLeft;
    shoulderLeftToElbowLeft << name << "/shoulderLeftToElbowLeft";
    stringstream shoulderLeftToElbowLeftImage;
    shoulderLeftToElbowLeftImage << shoulderLeftToElbowLeft.str() << ".png";
    LimbSkinSettings shoulderLeftToElbowLeftSettings;
    loadLimbSkinSettings(shoulderLeftToElbowLeft.str(), &shoulderLeftToElbowLeftSettings);
    
    stringstream elbowLeftToWristLeft;
    elbowLeftToWristLeft << name << "/elbowLeftToWristLeft";
    stringstream elbowLeftToWristLeftImage;
    elbowLeftToWristLeftImage << elbowLeftToWristLeft.str() << ".png";
    LimbSkinSettings elbowLeftToWristLeftSettings;
    loadLimbSkinSettings(elbowLeftToWristLeft.str(), &elbowLeftToWristLeftSettings);
    
    stringstream wristLeftToHandLeft;
    wristLeftToHandLeft << name << "/wristLeftToHandLeft";
    stringstream wristLeftToHandLeftImage;
    wristLeftToHandLeftImage << wristLeftToHandLeft.str() << ".png";
    LimbSkinSettings wristLeftToHandLeftSettings;
    loadLimbSkinSettings(wristLeftToHandLeft.str(), &wristLeftToHandLeftSettings);
    
    stringstream hipCenterToHipLeft;
    hipCenterToHipLeft << name << "/hipCenterToHipLeft";
    InvisibleLimbSkinSettings hipCenterToHipLeftSettings;
    loadInvisibleLimbSkinSettings(hipCenterToHipLeft.str(), &hipCenterToHipLeftSettings);
    
    stringstream hipLeftToKneeLeft;
    hipLeftToKneeLeft << name << "/hipLeftToKneeLeft";
    stringstream hipLeftToKneeLeftImage;
    hipLeftToKneeLeftImage << hipLeftToKneeLeft.str() << ".png";
    LimbSkinSettings hipLeftToKneeLeftSettings;
    loadLimbSkinSettings(hipLeftToKneeLeft.str(), &hipLeftToKneeLeftSettings);
    
    stringstream kneeLeftToAnkleLeft;
    kneeLeftToAnkleLeft << name << "/kneeLeftToAnkleLeft";
    stringstream kneeLeftToAnkleLeftImage;
    kneeLeftToAnkleLeftImage << kneeLeftToAnkleLeft.str() << ".png";
    LimbSkinSettings kneeLeftToAnkleLeftSettings;
    loadLimbSkinSettings(kneeLeftToAnkleLeft.str(), &kneeLeftToAnkleLeftSettings);
    
    stringstream ankleLeftToFootLeft;
    ankleLeftToFootLeft << name << "/ankleLeftToFootLeft";
    stringstream ankleLeftToFootLeftImage;
    ankleLeftToFootLeftImage << ankleLeftToFootLeft.str() << ".png";
    LimbSkinSettings ankleLeftToFootLeftSettings;
    loadLimbSkinSettings(ankleLeftToFootLeft.str(), &ankleLeftToFootLeftSettings);
    
        
    stringstream shoulderCenterToShoulderRight;
    shoulderCenterToShoulderRight << name << "/shoulderCenterToShoulderRight";
    InvisibleLimbSkinSettings shoulderCenterToShoulderRightSettings;
    loadInvisibleLimbSkinSettings(shoulderCenterToShoulderRight.str(), &shoulderCenterToShoulderRightSettings);
    
    stringstream shoulderRightToElbowRight;
    shoulderRightToElbowRight << name << "/shoulderRightToElbowRight";
    stringstream shoulderRightToElbowRightImage;
    shoulderRightToElbowRightImage << shoulderRightToElbowRight.str() << ".png";
    LimbSkinSettings shoulderRightToElbowRightSettings;
    loadLimbSkinSettings(shoulderRightToElbowRight.str(), &shoulderRightToElbowRightSettings);
    
    stringstream elbowRightToWristRight;
    elbowRightToWristRight << name << "/elbowRightToWristRight";
    stringstream elbowRightToWristRightImage;
    elbowRightToWristRightImage << elbowRightToWristRight.str() << ".png";
    LimbSkinSettings elbowRightToWristRightSettings;
    loadLimbSkinSettings(elbowRightToWristRight.str(), &elbowRightToWristRightSettings);
    
    stringstream wristRightToHandRight;
    wristRightToHandRight << name << "/wristRightToHandRight";
    stringstream wristRightToHandRightImage;
    wristRightToHandRightImage << wristRightToHandRight.str() << ".png";
    LimbSkinSettings wristRightToHandRightSettings;
    loadLimbSkinSettings(wristRightToHandRight.str(), &wristRightToHandRightSettings);
    
    stringstream hipCenterToHipRight;
    hipCenterToHipRight << name << "/hipCenterToHipRight";
    InvisibleLimbSkinSettings hipCenterToHipRighttSettings;
    loadInvisibleLimbSkinSettings(hipCenterToHipRight.str(), &hipCenterToHipRighttSettings);
    
    stringstream hipRightToKneeRight;
    hipRightToKneeRight << name << "/hipRightToKneeRight";
    stringstream hipRightToKneeRightImage;
    hipRightToKneeRightImage << hipRightToKneeRight.str() << ".png";
    LimbSkinSettings hipRightToKneeRightSettings;
    loadLimbSkinSettings(hipRightToKneeRight.str(), &hipRightToKneeRightSettings);
    
    stringstream kneeRightToAnkleRight;
    kneeRightToAnkleRight << name << "/kneeRightToAnkleRight";
    stringstream kneeRightToAnkleRightImage;
    kneeRightToAnkleRightImage << kneeRightToAnkleRight.str() << ".png";
    LimbSkinSettings kneeRightToAnkleRightSettings;
    loadLimbSkinSettings(kneeRightToAnkleRight.str(), &kneeRightToAnkleRightSettings);
    
    stringstream ankleRightToFootRight;
    ankleRightToFootRight << name << "/ankleRightToFootRight";
    stringstream ankleRightToFootRightImage;
    ankleRightToFootRightImage << ankleRightToFootRight.str() << ".png";
    LimbSkinSettings ankleRightToFootRightSettings;
    loadLimbSkinSettings(ankleRightToFootRight.str(), &ankleRightToFootRightSettings);
    
    
    skin->spineToShoulderCenter.set(spineToShoulderCenterImage.str(), spineToShoulderCenterSettings.boneRatio, spineToShoulderCenterSettings.anchorPercent, spineToShoulderCenterSettings.boneThickness);
    skin->spineToHipCenter.set(1);
    
    skin->hipCenterToHipLeft.set(hipCenterToHipLeftSettings.length, hipCenterToHipLeftSettings.boneThickness);
    skin->hipCenterToHipRight.set(hipCenterToHipRighttSettings.length, hipCenterToHipRighttSettings.boneThickness);
    
    skin->shoulderCenterToHead.set(shoulderCenterToHeadImage.str(), shoulderCenterToHeadSettings.boneRatio, shoulderCenterToHeadSettings.anchorPercent, shoulderCenterToHeadSettings.boneThickness);
    skin->shoulderCenterToShoulderLeft.set(shoulderCenterToShoulderLeftSettings.length, shoulderCenterToShoulderLeftSettings.boneThickness);
    skin->shoulderCenterToShoulderRight.set(shoulderCenterToShoulderRightSettings.length, shoulderCenterToShoulderRightSettings.boneThickness);
    
    skin->shoulderLeftToElbowLeft.set(shoulderLeftToElbowLeftImage.str(), shoulderLeftToElbowLeftSettings.boneRatio, shoulderLeftToElbowLeftSettings.anchorPercent, shoulderLeftToElbowLeftSettings.boneThickness);
    skin->elbowLeftToWristLeft.set(elbowLeftToWristLeftImage.str(), elbowLeftToWristLeftSettings.boneRatio, elbowLeftToWristLeftSettings.anchorPercent, elbowLeftToWristLeftSettings.boneThickness);
    skin->wristLeftToHandLeft.set(wristLeftToHandLeftImage.str(), wristLeftToHandLeftSettings.boneRatio, wristLeftToHandLeftSettings.anchorPercent, wristLeftToHandLeftSettings.boneThickness);
    
    skin->hipLeftToKneeLeft.set(hipLeftToKneeLeftImage.str(), hipLeftToKneeLeftSettings.boneRatio, hipLeftToKneeLeftSettings.anchorPercent, hipLeftToKneeLeftSettings.boneThickness);
    skin->kneeLeftToAnkleLeft.set(kneeLeftToAnkleLeftImage.str(), kneeLeftToAnkleLeftSettings.boneRatio, kneeLeftToAnkleLeftSettings.anchorPercent, kneeLeftToAnkleLeftSettings.boneThickness);
    skin->ankleLeftToFootLeft.set(ankleLeftToFootLeftImage.str(), ankleLeftToFootLeftSettings.boneRatio, ankleLeftToFootLeftSettings.anchorPercent, ankleLeftToFootLeftSettings.boneThickness);
    
    skin->shoulderRightToElbowRight.set(shoulderRightToElbowRightImage.str(), shoulderRightToElbowRightSettings.boneRatio, shoulderRightToElbowRightSettings.anchorPercent, shoulderRightToElbowRightSettings.boneThickness);
    skin->elbowRightToWristRight.set(elbowRightToWristRightImage.str(), elbowRightToWristRightSettings.boneRatio, elbowRightToWristRightSettings.anchorPercent, elbowRightToWristRightSettings.boneThickness);
    skin->wristRightToHandRight.set(wristRightToHandRightImage.str(), wristRightToHandRightSettings.boneRatio, wristRightToHandRightSettings.anchorPercent, wristRightToHandRightSettings.boneThickness);
    
    skin->hipRightToKneeRight.set(hipRightToKneeRightImage.str(), hipRightToKneeRightSettings.boneRatio, hipRightToKneeRightSettings.anchorPercent, hipRightToKneeRightSettings.boneThickness);
    skin->kneeRightToAnkleRight.set(kneeRightToAnkleRightImage.str(), kneeRightToAnkleRightSettings.boneRatio, kneeRightToAnkleRightSettings.anchorPercent, kneeRightToAnkleRightSettings.boneThickness);
    skin->ankleRightToFootRight.set(ankleRightToFootRightImage.str(), ankleRightToFootRightSettings.boneRatio, ankleRightToFootRightSettings.anchorPercent, ankleRightToFootRightSettings.boneThickness);
    
    skin->movingLimit.set( skin->hipLeftToKneeLeft.length * 5, skin->hipLeftToKneeLeft.length + skin->kneeLeftToAnkleLeft.length + skin->ankleLeftToFootLeft.length );
    
    stringstream icon;
    icon << name << "/icon.png";
    skin->iconFile = icon.str();
    
    stringstream iconDead;
    iconDead << name << "/iconDead.png";
    skin->iconDeadFile = iconDead.str();
}

void Player::loadLimbSkinSettings(string file, LimbSkinSettings* settings) {
    ifstream f;
	f.open(ofToDataPath(file).c_str());
	vector <string> strLines;
	while (!f.eof()) {
		string ptStr;
		getline(f, ptStr);
		strLines.push_back(ptStr);
	}
	f.close();
    
    settings->boneRatio = ofToFloat(strLines[0]);
    settings->anchorPercent = ofPoint(ofToFloat(strLines[1]),ofToFloat(strLines[2]));
    settings->boneThickness = ofToFloat(strLines[3]);
}
void Player::loadInvisibleLimbSkinSettings(string file, InvisibleLimbSkinSettings* settings) {
    ifstream f;
	f.open(ofToDataPath(file).c_str());
	vector <string> strLines;
	while (!f.eof()) {
		string ptStr;
		getline(f, ptStr);
		strLines.push_back(ptStr);
	}
	f.close();
    
    settings->length = ofToFloat(strLines[0]);
    settings->boneThickness = ofToFloat(strLines[1]);
}