#include "Game.h"

void Game::setup(float width, float height) {
    
    this->width = width;
    this->height = height;
    
    box2d.init();
	box2d.setGravity(0, 15);
    ofAddListener(box2d.contactStartEvents, this, &Game::contactStart);
    
    currentSkinID = 0;
    
    // Players and games
    player1 = new Player();
    
    game1 = new PlayerGame();
    game1->setup(box2d.getWorld());
    game1->setPlayer(player1);
    game1->position.x = (width / 8) * 2;
    game1->position.y = height - 30;
    game1->sensorOffset.x = -2060;
    
    player2 = new Player();
    
    game2 = new PlayerGame();
    game2->setup(box2d.getWorld());
    game2->setPlayer(player2);
    game2->position.x = (width / 8) * 6;
    game2->position.y = height - 30;
    game2->sensorOffset.x = 2060;
    
    // State Timers
    startTimer.setup(2);
    transitionToPlayingTimer.setup(2);
    playingTimer.setup(60);
    transitionToEndTimer.setup(2);
    endTimer.setup(2);
    transitionToStartTimer.setup(2);
    
        
    // Bear skin
    PlayerSkin* bear = new PlayerSkin;
    bear->globalScale = 0.6;
    bear->spineToShoulderCenter.set("bear/spineToShoulderCenter.png", 0.7, ofPoint(0.5, 0.4));
    bear->spineToHipCenter.set(1);
    
    bear->hipCenterToHipLeft.set(30);
    bear->hipCenterToHipRight.set(30);
    
    bear->shoulderCenterToHead.set("bear/shoulderCenterToHead.png");
    bear->shoulderCenterToShoulderLeft.set(60);
    bear->shoulderCenterToShoulderRight.set(60);
    
    bear->shoulderLeftToElbowLeft.set("bear/shoulderLeftToElbowLeft.png");
    bear->elbowLeftToWristLeft.set("bear/elbowLeftToWristLeft.png");
    bear->wristLeftToHandLeft.set("bear/wristLeftToHandLeft.png");
    
    bear->shoulderRightToElbowRight.set("bear/shoulderRightToElbowRight.png");
    bear->elbowRightToWristRight.set("bear/elbowRightToWristRight.png");
    bear->wristRightToHandRight.set("bear/wristRightToHandRight.png");
    
    bear->hipLeftToKneeLeft.set("bear/hipLeftToKneeLeft.png");
    bear->kneeLeftToAnkleLeft.set("bear/kneeLeftToAnkleLeft.png");
    bear->ankleLeftToFootLeft.set("bear/ankleLeftToFootLeft.png");
    
    bear->hipRightToKneeRight.set("bear/hipRightToKneeRight.png");
    bear->kneeRightToAnkleRight.set("bear/kneeRightToAnkleRight.png");
    bear->ankleRightToFootRight.set("bear/ankleRightToFootRight.png");
    
    bear->movingLimit.set( width, bear->hipLeftToKneeLeft.length + bear->kneeLeftToAnkleLeft.length + bear->ankleLeftToFootLeft.length );
    
    skins.push_back(bear);
    
    // Boat
    boat.setup(box2d.getWorld());
    boat.position.set(width / 2, height );
    
    // Water
    water.setup(box2d.getWorld(), width, height);
    
    // Start!
    initTransitionToStart();
}

void Game::setData(ofxOscMessage &m){
    if ( m.getAddress() == "/player/1" ){
        player1->setData(m);
    }
    else if( m.getAddress() == "/player/2" ){
        player2->setData(m);
    }
}

void Game::update(){
    float dt = 1.f/ofGetFrameRate();
    
    switch (state) {
        case Game::START:
            player1->update();
            player2->update();
            
            startTimer.update(dt);
            if(startTimer.isComplete()){
                initTransitionToPlaying();
            }
            
            break;
        case Game::TRANSITION_TO_PLAYING:
            boatPositionTweener.update(dt);
            boat.update();
            
            player1->update();
            player2->update();
            
            transitionToPlayingTimer.update(dt);
            if(transitionToPlayingTimer.isComplete()){
                initPlaying();
            }            
            break;
        case Game::PLAYING:           
            boatPositionTweener.update(dt);
            boat.update();
            
            game1->update();
            game2->update();
            player1->update();
            player2->update();
            
            water.update();          
            
            playingTimer.update(dt);
            if(playingTimer.isComplete()){
                initTransitionToEnd();
            } 
            break;
        case Game::TRANSITION_TO_END:
            
            
            boatPositionTweener.update(dt);
            boat.update();
            
            player1->update();
            player2->update();
            
            water.update();
            
            transitionToEndTimer.update(dt);
            if(transitionToEndTimer.isComplete()){
                initEnd();
            } 
            break;
        case Game::END:
            
            
            player1->update();
            player2->update();
            
            endTimer.update(dt);
            if(endTimer.isComplete()){
                initTransitionToStart();
            } 
            break;
        case Game::TRANSITION_TO_START:
            transitionToStartTimer.update(dt);            
            if(transitionToStartTimer.isComplete()){             
                initStart();
            } 
            break;        

    }
    
    
    box2d.update();
}

void Game::draw(){
    ofEnableAlphaBlending();
    switch (state) {
        case Game::START:
            player1->draw();
            player2->draw();
            
            ofDrawBitmapString("START", 20, 30);
            ofDrawBitmapString(ofToString((startTimer.getDuration() - startTimer.getDuration() * startTimer.getProgress())), 20, 40);
            break;
        case Game::TRANSITION_TO_PLAYING:
            boat.draw();
            
            player1->draw();
            player2->draw();
            
            ofDrawBitmapString("TRANSITION_TO_PLAYING", 20, 30);
            ofDrawBitmapString(ofToString((transitionToPlayingTimer.getDuration() - transitionToPlayingTimer.getDuration() * transitionToPlayingTimer.getProgress())), 20, 40);
            break;
        case Game::PLAYING:
            boat.draw();
            
            game1->draw();
            game2->draw();
            player1->draw();
            player2->draw();
                    
            water.draw();
           
            ofDrawBitmapString("PLAYING", 20, 30);
            ofDrawBitmapString(ofToString((playingTimer.getDuration() - playingTimer.getDuration() * playingTimer.getProgress())), 20, 40);
            break;
        case Game::TRANSITION_TO_END:
            boat.draw();
            
            player1->draw();
            player2->draw();
            
            water.draw();
            ofDrawBitmapString("TRANSITION_TO_END", 20, 30);
            ofDrawBitmapString(ofToString((transitionToEndTimer.getDuration() - transitionToEndTimer.getDuration() * transitionToEndTimer.getProgress())), 20, 40);
            break;
        case Game::END:
            player1->draw();
            player2->draw();
            
            ofDrawBitmapString("END", 20, 30);
            ofDrawBitmapString(ofToString((endTimer.getDuration() - endTimer.getDuration() * endTimer.getProgress())), 20, 40);
            break;
        case Game::TRANSITION_TO_START:
            ofDrawBitmapString("TRANSITION_TO_START", 20, 30);
            ofDrawBitmapString(ofToString((transitionToStartTimer.getDuration() - transitionToStartTimer.getDuration() * transitionToStartTimer.getProgress())), 20, 40);
            break;        
            
    }
    
    
    
    
    
}

void Game::initStart() {
    state = Game::START;
    startTimer.start();
    
    player1->setup(box2d.getWorld(), getRandomSkin());
    player1->position.set((width / 8) * 3, height * 0.5);
    
    player2->setup(box2d.getWorld(), getRandomSkin());    
    player2->position.set((width / 8) * 5, height * 0.5);
}
void Game::initTransitionToPlaying() {
    state = Game::TRANSITION_TO_PLAYING;
    transitionToPlayingTimer.start();
    
    boat.position.y = height + 256;
    boatPositionTweener.clearTweens();
    boatPositionTweener.setup(transitionToPlayingTimer.getDuration(), 0, Elastic::easeOut);
    boatPositionTweener.addTween( &(boat.position.y) , - 256);
    boatPositionTweener.start();
}
void Game::initPlaying() {
    state = Game::PLAYING;
    playingTimer.start();
    
    boat.position.y = height;
    boatPositionTweener.clearTweens();
    boatPositionTweener.setup(2, 0, Sine::easeInOut, BACK_AND_FORTH);
    boatPositionTweener.addTween( &(boat.position.y) , -20);
    boatPositionTweener.start();
}
void Game::initTransitionToEnd() {
    state = Game::TRANSITION_TO_END;
    transitionToEndTimer.start();
    
    boat.position.y = height;
    boatPositionTweener.clearTweens();
    boatPositionTweener.setup(0.5, 0, Back::easeIn);
    boatPositionTweener.addTween( &(boat.position.y) , 300);
    boatPositionTweener.start();
    
    
}
void Game::initEnd() {
    state = Game::END;
    endTimer.start();
}
void Game::initTransitionToStart() {
    state = Game::TRANSITION_TO_START;
    transitionToStartTimer.start();
}

PlayerSkin* Game::getRandomSkin(){
    int id = currentSkinID;
    currentSkinID ++;
    if(currentSkinID == skins.size()){
        currentSkinID = 0;
    }
    return skins[id];
}

void Game::contactStart(ofxBox2dContactArgs &e) {
	if(e.a != NULL && e.b != NULL) {
        Data * sensorInData = NULL;
        Data * waterData = NULL;
        
        Data * aData = (Data*)e.a->GetBody()->GetUserData();        
        if(aData) {
            if(aData->type == WATER) waterData =  aData;
            else if(aData->type == SENSOR_IN) sensorInData = aData;
        }
        
        Data * bData  = (Data*)e.b->GetBody()->GetUserData();        
        if(bData) {
            if(bData->type == WATER) waterData =  bData;
            else if(bData->type == SENSOR_IN) sensorInData = bData;
        }
        
        
		if( sensorInData && waterData ){           
            water.removeCircle(waterData->label);
        }
	}
}