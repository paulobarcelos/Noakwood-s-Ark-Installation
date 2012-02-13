#include "Game.h"

void Game::setup(float width, float height) {
    
    this->width = width;
    this->height = height;
    
    box2d.init();
	box2d.setGravity(0, 15);
    
    currentSkinID = 0;
    
    // State Timers
    startTimer.setup(0.5);
    transitionToPlayingTimer.setup(2);
    playingTimer.setup(60);
    transitionToEndTimer.setup(2);
    endTimer.setup(2);
    transitionToStartTimer.setup(0.5);
    
        
    // Bear skin
    PlayerSkin* bear = new PlayerSkin();
    Player::loadSkin("bear", bear);    
    bear->globalScale = 0.6;
    bear->movingLimit.x = width;    
    skins.push_back(bear);
    
    // Boat
    boat.setup(box2d.getWorld());
    boat.position.set(width / 2, height );
    
    // Water
    water.setup(box2d.getWorld(), width, height);
    
    // Players and games
    game1.setup(box2d.getWorld(), &player1, &water, PlayerGame::LEFT, width, height);
    game2.setup(box2d.getWorld(), &player2, &water, PlayerGame::RIGHT, width, height);
    
    // FBO
    gamePlayFbo.allocate(width + FBO_MARGIN, height + FBO_MARGIN);
    gamePlayFbo.getTextureReference().setAnchorPercent(0.5, 0.5);
    
    // Start!
    initTransitionToStart();
}

void Game::setData(ofxOscMessage &m){
    if ( m.getAddress() == "/player/1" ){
        player1.setData(m);
    }
    else if( m.getAddress() == "/player/2" ){
        player2.setData(m);
    }
}

void Game::update(){
    float dt = 1.f/ofGetFrameRate();
    
    switch (state) {
        case Game::START:
            player1.update();
            player2.update();            
            playersOpacityTweener.update(dt);
            
            startTimer.update(dt);
            if(startTimer.isComplete()){
                initTransitionToPlaying();
            }
            
            break;
        case Game::TRANSITION_TO_PLAYING:
            boatPositionTweener.update(dt);
            boat.update();
            
            player1.update();
            player2.update();
            
            transitionToPlayingTimer.update(dt);
            if(transitionToPlayingTimer.isComplete()){
                initPlaying();
            }            
            break;
        case Game::PLAYING:{
            //boatPositionTweener.update(dt);
            boat.update();
            
            game1.update();
            game2.update();
            player1.update();
            player2.update();
            
            water.update();
            
            gamePlayTweener1.update(dt);
            gamePlayTweener2.update(dt);
            
            float currentWaterLevel = water.getLevel();
            currentWaterLevel *= currentWaterLevel; 
    
            float deltaWaterLevel = currentWaterLevel - lastWaterLevel;
            
            if(deltaWaterLevel > 0){
                gamePlayCapacityPositiveTweener.setProgress(deltaWaterLevel);
                gamePlayCapacityPositiveTweener.update(0);
            }
            else{
                gamePlayCapacityNegativeTweener.setProgress(deltaWaterLevel);
                gamePlayCapacityNegativeTweener.update(0);
            }                      
            
            playingTimer.update(dt);
            if(playingTimer.isComplete()){
                initTransitionToEnd();
            } 
            break;
        }
        case Game::TRANSITION_TO_END:       
            
            boatPositionTweener.update(dt);
            boat.update();
            
            player1.update();
            player2.update();
            playersOpacityTweener.update(dt);
            
            water.update();            
            
            
            transitionToEndTimer.update(dt);
            if(transitionToEndTimer.isComplete()){
                initEnd();
            } 
            break;
        case Game::END:
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
            ofPushStyle();
            ofSetColor(255, 255, 255, playersOpacity * (float) 255);
            player1.draw();
            player2.draw();
            ofPopStyle();
            
            ofDrawBitmapString("START", 20, 30);
            ofDrawBitmapString(ofToString((startTimer.getDuration() - startTimer.getDuration() * startTimer.getProgress())), 20, 40);
            break;
        case Game::TRANSITION_TO_PLAYING:
            beginGamePlayDraw();
                boat.draw();            
                player1.draw();
                player2.draw();
            endGamePlayDraw();
            
            
            ofDrawBitmapString("TRANSITION_TO_PLAYING", 20, 30);
            ofDrawBitmapString(ofToString((transitionToPlayingTimer.getDuration() - transitionToPlayingTimer.getDuration() * transitionToPlayingTimer.getProgress())), 20, 40);
            break;
        case Game::PLAYING:
            beginGamePlayDraw();
                boat.draw();            
                player1.draw();
                player2.draw();
                water.draw();
            endGamePlayDraw();
            
            game1.draw();
            game2.draw();
           
            ofDrawBitmapString("PLAYING", 20, 30);
            ofDrawBitmapString(ofToString((playingTimer.getDuration() - playingTimer.getDuration() * playingTimer.getProgress())), 20, 40);
            break;
        case Game::TRANSITION_TO_END:
            beginGamePlayDraw();
                boat.draw();
                ofPushStyle();
                ofSetColor(255, 255, 255, playersOpacity * (float) 255);
                    player1.draw();
                    player2.draw();
                ofPopStyle();
                water.draw();
            endGamePlayDraw();
            
            
            ofDrawBitmapString("TRANSITION_TO_END", 20, 30);
            ofDrawBitmapString(ofToString((transitionToEndTimer.getDuration() - transitionToEndTimer.getDuration() * transitionToEndTimer.getProgress())), 20, 40);
            break;
        case Game::END:
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
    
    playersOpacity = 0;
    playersOpacityTweener.clearTweens();
    playersOpacityTweener.setup(0.5f, 0, Sine::easeOut);
    playersOpacityTweener.addTween(&playersOpacity, 1);
    playersOpacityTweener.start();
    
    player1.setup(box2d.getWorld(), getRandomSkin());
    player1.position.set((width / 8) * 3, height * 0.5);
    
    player2.setup(box2d.getWorld(), getRandomSkin());    
    player2.position.set((width / 8) * 5, height * 0.5);
}
void Game::initTransitionToPlaying() {
    state = Game::TRANSITION_TO_PLAYING;
    transitionToPlayingTimer.start();
    
    gamePlayPosition.set(width/2, height/2);
    gamePlayRotation = 0;
    
    boat.position.y = height + 256;
    boatPositionTweener.clearTweens();
    boatPositionTweener.setup(transitionToPlayingTimer.getDuration(), 0, Elastic::easeOut);
    boatPositionTweener.addTween( &(boat.position.y) , - 256);
    boatPositionTweener.start();
}
void Game::initPlaying() {
    state = Game::PLAYING;
    playingTimer.start();
    
    gamePlayPosition.set(width/2 - 10, height/2 - 10);
    gamePlayRotation = -3;
    gamePlayTweener1.clearTweens();
    gamePlayTweener1.setup(1.5, 0, Sine::easeInOut, BACK_AND_FORTH);
    gamePlayTweener1.addTween( &(gamePlayPosition.y) , + 20);
    gamePlayTweener1.start();
    gamePlayTweener1.setProgress(0.5f);
    gamePlayTweener1.update(0);
    gamePlayTweener2.clearTweens();
    gamePlayTweener2.setup(3, 0, Sine::easeInOut, BACK_AND_FORTH);
    gamePlayTweener2.addTween( &(gamePlayPosition.x) , + 20);
    gamePlayTweener2.addTween( &(gamePlayRotation) , + 6);
    gamePlayTweener2.start();
    gamePlayTweener2.setProgress(0.5f);
    gamePlayTweener2.update(0);
    
    gamePlayCapacityPositiveTweener.clearTweens();
    gamePlayCapacityPositiveTweener.setup(1.f);
    gamePlayCapacityPositiveTweener.addTween( &(gamePlayPosition.y) , BOAT_LEVEL_OFFSET);
    gamePlayCapacityPositiveTweener.start();
    
    gamePlayCapacityNegativeTweener.clearTweens();
    gamePlayCapacityNegativeTweener.setup(1.f);
    gamePlayCapacityNegativeTweener.addTween( &(gamePlayPosition.y) , -BOAT_LEVEL_OFFSET);
    gamePlayCapacityNegativeTweener.start();
    
    player1.position.set((width / 8) * 2, height - 30);
    player2.position.set((width / 8) * 6, height - 30);
    
    game1.reset();
    game2.reset();
    
    water.reset();
    lastWaterLevel = water.getLevel();
    positiveWaterLevel = 0;
    negativeWaterLevel = 0;
    
    boat.position.y = height;
    boatPositionTweener.clearTweens();
    /*boatPositionTweener.setup(2, 0, Sine::easeInOut, BACK_AND_FORTH);
    boatPositionTweener.addTween( &(boat.position.y) , -20);
    boatPositionTweener.start();*/
}
void Game::initTransitionToEnd() {
    state = Game::TRANSITION_TO_END;
    transitionToEndTimer.start();
    
    playersOpacity = 1;
    playersOpacityTweener.clearTweens();
    playersOpacityTweener.setup(1.f, 0, Sine::easeOut);
    playersOpacityTweener.addTween(&playersOpacity, -1);
    playersOpacityTweener.start();
    
    boat.position.y = height;
    boatPositionTweener.clearTweens();
    boatPositionTweener.setup(1, 0, Back::easeIn);
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
void Game::beginGamePlayDraw() {
    gamePlayFbo.begin();
    ofPushStyle();
    ofEnableAlphaBlending();
    ofClear(0,0,0,0);
    ofPushMatrix();
    ofTranslate(FBO_MARGIN/2, FBO_MARGIN/2);
}
void Game::endGamePlayDraw() {
    ofPopMatrix();
    ofPopStyle();
    gamePlayFbo.end();
    
    ofPushMatrix();
    ofTranslate(gamePlayPosition);
    ofRotateZ(gamePlayRotation);
    gamePlayFbo.draw(0,0);
    ofPopMatrix();
}