#include "Game.h"

void Game::setup(float width, float height) {
    
    this->width = width;
    this->height = height;
    gamePlayHeight = height - 30;
    
    box2d.init();
	box2d.setGravity(0, 15);
    
    currentSkinID = 0;
    
    // State Timers
    /*startTimer.setup(48);
    transitionToPlayingTimer.setup(8);
    playingTimer.setup(68);
    transitionToEndTimer.setup(4);
    endTimer.setup(10);
    transitionToStartTimer.setup(2);*/
    startTimer.setup(2);
    transitionToPlayingTimer.setup(8);
    playingTimer.setup(68);
    transitionToEndTimer.setup(4);
    endTimer.setup(10);
    transitionToStartTimer.setup(2);
            
    // Bear skin
    PlayerSkin* bear = new PlayerSkin();
    Player::loadSkin("bear", bear);    
    bear->globalScale = 0.6;
    bear->movingLimit.x = width;    
    skins.push_back(bear);
    
    // Boat
    boat.setup(box2d.getWorld());
    boat.position.set(width / 2, gamePlayHeight );
    
    // Water
    water.setup(box2d.getWorld(), width, gamePlayHeight);
    
    // Players and games
    game1.setup(box2d.getWorld(), &player1, &water, PlayerGame::LEFT, width, gamePlayHeight, playingTimer.getDuration());
    game2.setup(box2d.getWorld(), &player2, &water, PlayerGame::RIGHT, width, gamePlayHeight, playingTimer.getDuration());
    
    // Scenario
    scenario.setup(width, height);
    
    // State overlays
    startOverlay.loadImage("overlays/start.png");
    transitionToPlayingOverlay.loadImage("overlays/transitionToPlaying.png");
    playingOverlay.loadImage("overlays/playing.png");
    endWinOverlay.loadImage("overlays/endWin.png");
    endLoseOverlay.loadImage("overlays/endLose.png");    
    
    // Timer Font
    timerFont.loadFont("FuturaLT-Bold.ttf", 30);
    
    // Playing Timer
    playingTimerFont.loadFont("FuturaLT-Bold.ttf", 40);
    
    // Border
    border.loadImage("border.png");
    
    // Central Message
    centralMessage.setup(0, 0, width, height, 1.5f);
    
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
            
            gamePlayTweener1.update(dt);
            gamePlayTweener2.update(dt);
            
            gamePlayInterfaceTweener.update(dt);
            
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
            
            gamePlayInterfaceTweener.update(dt);
            
            float currentWaterLevel = water.getLevel();
            
            if(currentWaterLevel > 0.7 && !hasSinkWarning){
                hasSinkWarning = true;
                centralMessage.queueMessageOnce(CentralMessage::SINK_WARNING);
            }
            
            if(currentWaterLevel < 0.65 && hasSinkWarning){
                hasSinkWarning = false;
            }
            
            currentWaterLevel = powf(currentWaterLevel, 3.5); 
    
            float deltaWaterLevel = currentWaterLevel - lastWaterLevel;
            
            if(deltaWaterLevel > 0){
                gamePlayCapacityPositiveTweener.setProgress(deltaWaterLevel);
                gamePlayCapacityPositiveTweener.update(0);
            }
            else{
                gamePlayCapacityNegativeTweener.setProgress(deltaWaterLevel);
                gamePlayCapacityNegativeTweener.update(0);
            }
            
            
            if(currentWaterLevel >= 1){
                initTransitionToEnd();
                finalState = Game::GAME_OVER;
                scenario.setState(Scenario::LOSE);
            }
            
            playingTimer.update(dt);
            
            /*if( (playingTimer.getDuration() - playingTimer.getProgress() * playingTimer.getDuration()) < 4.5 && !hasTimeWarning){
                hasTimeWarning = true;
                centralMessage.queueMessage(CentralMessage::THREE);
                centralMessage.queueMessage(CentralMessage::TWO);
                centralMessage.queueMessage(CentralMessage::ONE);                
            }*/
            
            if(playingTimer.isComplete()){
                initTransitionToEnd();
                finalState = Game::NORMAL;
                scenario.setState(Scenario::WIN);
            } 
            break;
        }
        case Game::TRANSITION_TO_END:       
            
            boatPositionTweener.update(dt);
            boat.update();
            
            player1.update();
            player2.update();
            playersOpacityTweener.update(dt);
            
            gamePlayInterfaceTweener.update(dt);
            
            
            transitionToEndTimer.update(dt);
            if(transitionToEndTimer.isComplete()){
                initEnd();
            } 
            break;
        case Game::END:            
            gamePlayInterfaceTweener.update(dt);
            
            endTimer.update(dt);
            if(endTimer.isComplete()){
                initTransitionToStart();
            } 
            break;
        case Game::TRANSITION_TO_START:
            gamePlayInterfaceTweener.update(dt);
            
            transitionToStartTimer.update(dt);            
            if(transitionToStartTimer.isComplete()){             
                initStart();
            } 
            break;        

    }
    
    scenario.update();
    centralMessage.update();
    box2d.update();
}

void Game::draw(){
    ofEnableAlphaBlending();
    switch (state) {
        case Game::START:{
            scenario.drawBackground();
            
            ofPushStyle();
            ofSetColor(255, 255, 255, playersOpacity * (float) 255);
            player1.draw();
            player2.draw();
            ofPopStyle();
            
            scenario.drawForeground();
            
            int departure = (int)(startTimer.getDuration() - startTimer.getProgress() * startTimer.getDuration());
            
            stringstream ss;
            ss << "00:";
            if(departure < 10) ss << "0";
            ss << ofToString(departure);
            timerFont.drawString(ss.str(), width / 2 - 57, 150);
            
            border.draw(0,0, width, height);
            
            startOverlay.draw(0,0, width, height);
            
            ofDrawBitmapString("START", 20, 30);
            ofDrawBitmapString(ofToString((startTimer.getDuration() - startTimer.getDuration() * startTimer.getProgress())), 20, 40);
            
            break;
        }
        case Game::TRANSITION_TO_PLAYING:
            scenario.drawBackground();
            
            beginGamePlayDraw();
                boat.draw();            
                player1.draw();
                player2.draw();
            endGamePlayDraw();
            
            scenario.drawForeground(); 
            
            border.draw(0,0);
            
            ofPushStyle();
            ofSetColor(255, 255, 255, gamePlayInterfaceOpacity * (float) 255);
          //  transitionToPlayingOverlay.draw(0,0, width, height);
            ofPopStyle();
            
            
            ofDrawBitmapString("TRANSITION_TO_PLAYING", 20, 30);
            ofDrawBitmapString(ofToString((transitionToPlayingTimer.getDuration() - transitionToPlayingTimer.getDuration() * transitionToPlayingTimer.getProgress())), 20, 40);
            break;
        case Game::PLAYING:{
            scenario.drawBackground();
            
            beginGamePlayDraw();
                boat.draw();            
                player1.draw();
                player2.draw();
                water.draw();
            endGamePlayDraw();
            
            scenario.drawForeground();
            
            border.draw(0,0);
            
            ofPushStyle();
            ofSetColor(255, 255, 255, gamePlayInterfaceOpacity * (float) 255);            
            playingOverlay.draw(0,0, width, height);
            
            int gameEnd = (int)(playingTimer.getDuration() - playingTimer.getProgress() * playingTimer.getDuration());
            
            stringstream ss;
            ss << "00:";
            if(gameEnd < 10) ss << "0";
            ss << ofToString(gameEnd);
            playingTimerFont.drawString(ss.str(), width / 2 - 80, 85);
            
            game1.draw();
            game2.draw();
            ofPopStyle();
            
           
            ofDrawBitmapString("PLAYING", 20, 30);
            ofDrawBitmapString(ofToString((playingTimer.getDuration() - playingTimer.getDuration() * playingTimer.getProgress())), 20, 40);
            break;
        }
        case Game::TRANSITION_TO_END:
            scenario.drawBackground();
            
            beginGamePlayDraw();
                boat.draw();
                ofPushStyle();
                ofSetColor(255, 255, 255, playersOpacity * (float) 255);
                    player1.draw();
                    player2.draw();
                ofPopStyle();
                water.draw();
            endGamePlayDraw();
            
            scenario.drawForeground();
            
            border.draw(0,0);
            
            ofPushStyle();
            ofSetColor(255, 255, 255, gamePlayInterfaceOpacity * (float) 255);
                game1.draw();
                game2.draw();
            ofPopStyle();
            
            
            ofDrawBitmapString("TRANSITION_TO_END", 20, 30);
            ofDrawBitmapString(ofToString((transitionToEndTimer.getDuration() - transitionToEndTimer.getDuration() * transitionToEndTimer.getProgress())), 20, 40);
            break;
        case Game::END:
            switch (finalState) {
                case Game::NORMAL:
                    scenario.drawBackground();
                    
                    ofPushStyle();
                    ofSetColor(255, 255, 255, gamePlayInterfaceOpacity * (float) 255);
           
                    ofPushMatrix();
                    ofTranslate(width / 4, height / 2);
                    player1.icon.draw(-FINAL_ICON_SIZE / 2, -FINAL_ICON_SIZE / 2, FINAL_ICON_SIZE, FINAL_ICON_SIZE);
                    ofPopMatrix();
                    ofPushMatrix();
                    ofTranslate((width / 4) * 3 , height / 2);
                    player2.icon.draw(-FINAL_ICON_SIZE / 2, -FINAL_ICON_SIZE / 2, FINAL_ICON_SIZE, FINAL_ICON_SIZE);
                    ofPopMatrix();
                                        
                    ofPopStyle();
                    
                    scenario.drawForeground();
                    border.draw(0,0);
                    
                    ofPushStyle();
                    ofSetColor(255, 255, 255, gamePlayInterfaceOpacity * (float) 255);
                    endWinOverlay.draw(0,0, width, height);
                    ofPopStyle();
                    break;
                    
                case Game::GAME_OVER:
                    scenario.drawBackground();                    
                    scenario.drawForeground();
                    border.draw(0,0);
                    
                    ofPushStyle();
                    ofSetColor(255, 255, 255, gamePlayInterfaceOpacity * (float) 255);
                    endLoseOverlay.draw(0,0, width, height);
                    ofPopStyle();
                    
                    break;
            }
            
            
            ofDrawBitmapString("END", 20, 30);
            ofDrawBitmapString(ofToString((endTimer.getDuration() - endTimer.getDuration() * endTimer.getProgress())), 20, 40);
            break;
        case Game::TRANSITION_TO_START:
            
            switch (finalState) {
                case Game::NORMAL:
                    scenario.drawBackground();
                    
                    ofPushStyle();
                    ofSetColor(255, 255, 255, gamePlayInterfaceOpacity * (float) 255);
                    
                    ofPushMatrix();
                    ofTranslate(width / 4, height / 2);
                    player1.icon.draw(-FINAL_ICON_SIZE / 2, -FINAL_ICON_SIZE / 2, FINAL_ICON_SIZE, FINAL_ICON_SIZE);
                    ofPopMatrix();
                    ofPushMatrix();
                    ofTranslate((width / 4) * 3 , height / 2);
                    player2.icon.draw(-FINAL_ICON_SIZE / 2, -FINAL_ICON_SIZE / 2, FINAL_ICON_SIZE, FINAL_ICON_SIZE);
                    ofPopMatrix();
                    
                    ofPopStyle();
                    
                    scenario.drawForeground();
                    border.draw(0,0);
                    
                    ofPushStyle();
                    ofSetColor(255, 255, 255, gamePlayInterfaceOpacity * (float) 255);
                    endWinOverlay.draw(0,0, width, height);
                    ofPopStyle();
                    break;
                    
                case Game::GAME_OVER:
                    scenario.drawBackground();                    
                    scenario.drawForeground();
                    border.draw(0,0);
                    
                    ofPushStyle();
                    ofSetColor(255, 255, 255, gamePlayInterfaceOpacity * (float) 255);
                    endLoseOverlay.draw(0,0, width, height);
                    ofPopStyle();
                    break;
            }
                        

            ofDrawBitmapString("TRANSITION_TO_START", 20, 30);
            ofDrawBitmapString(ofToString((transitionToStartTimer.getDuration() - transitionToStartTimer.getDuration() * transitionToStartTimer.getProgress())), 20, 40);
            break;        
            
    }
    centralMessage.draw();
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
    player2.setup(box2d.getWorld(), getRandomSkin());    
    
    player1.position.set((width / 8) * 2, gamePlayHeight - 30);
    player2.position.set((width / 8) * 6, gamePlayHeight - 30);
    
    scenario.setState(Scenario::NORMAL);
    
}
void Game::initTransitionToPlaying() {
    state = Game::TRANSITION_TO_PLAYING;
    transitionToPlayingTimer.start();
    
    gamePlayPosition.set(width/2, height/2);
    gamePlayRotation = 0;
    
    boat.position.y = gamePlayHeight + 256;
    boatPositionTweener.clearTweens();
    boatPositionTweener.setup(6.f, 1.5f, Elastic::easeOut);
    boatPositionTweener.addTween( &(boat.position.y) , - 256);
    boatPositionTweener.start();
    
    gamePlayInterfaceOpacity = 0;
    gamePlayInterfaceTweener.clearTweens();
    gamePlayInterfaceTweener.setup(1.f, 0, Sine::easeOut);
    gamePlayInterfaceTweener.addTween(&gamePlayInterfaceOpacity, 1);
    gamePlayInterfaceTweener.start();
    
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
    
    centralMessage.setDuration(2.f);
    centralMessage.queueMessage(CentralMessage::EYE);
}
void Game::initPlaying() {
    state = Game::PLAYING;
    playingTimer.start();
    
    gamePlayCapacityPositiveTweener.clearTweens();
    gamePlayCapacityPositiveTweener.setup(1.f);
    gamePlayCapacityPositiveTweener.addTween( &(gamePlayPosition.y) , BOAT_LEVEL_OFFSET);
    gamePlayCapacityPositiveTweener.start();
    
    gamePlayCapacityNegativeTweener.clearTweens();
    gamePlayCapacityNegativeTweener.setup(1.f);
    gamePlayCapacityNegativeTweener.addTween( &(gamePlayPosition.y) , -BOAT_LEVEL_OFFSET);
    gamePlayCapacityNegativeTweener.start();
    
    gamePlayInterfaceOpacity = 0;
    gamePlayInterfaceTweener.clearTweens();
    gamePlayInterfaceTweener.setup(1.f, 8.f, Sine::easeOut);
    gamePlayInterfaceTweener.addTween(&gamePlayInterfaceOpacity, 1);
    gamePlayInterfaceTweener.start();
    
    game1.reset();
    game2.reset();
    
    water.reset();
    lastWaterLevel = water.getLevel();
    positiveWaterLevel = 0;
    negativeWaterLevel = 0;
    
    boat.position.y = gamePlayHeight;
    
    hasSinkWarning = false;
    hasTimeWarning = false;
    
    scenario.setState(Scenario::STRESS);
    
    centralMessage.setDuration(2.666666f);
    centralMessage.queueMessage(CentralMessage::INTRO_1);
    centralMessage.queueMessage(CentralMessage::NO_MESSAGE);
    centralMessage.queueMessage(CentralMessage::INTRO_2);
}
void Game::initTransitionToEnd() {
    state = Game::TRANSITION_TO_END;
    transitionToEndTimer.start();
    
    playersOpacity = 1;
    playersOpacityTweener.clearTweens();
    playersOpacityTweener.setup(1.f, 0, Sine::easeOut);
    playersOpacityTweener.addTween(&playersOpacity, -1);
    playersOpacityTweener.start();
    
    boat.position.y = gamePlayHeight;
    boatPositionTweener.clearTweens();
    boatPositionTweener.setup(1, 0, Back::easeIn);
    boatPositionTweener.addTween( &(boat.position.y) , 300);
    boatPositionTweener.start();
    
    gamePlayInterfaceOpacity = 1;
    gamePlayInterfaceTweener.clearTweens();
    gamePlayInterfaceTweener.setup(1.f, 0, Sine::easeOut);
    gamePlayInterfaceTweener.addTween(&gamePlayInterfaceOpacity, -1);
    gamePlayInterfaceTweener.start();
    
    
    centralMessage.setDuration(5.f);
    centralMessage.queueMessage(CentralMessage::EYE);
    
    /*switch (finalState) {
        case Game::NORMAL:
            centralMessage.queueMessage(CentralMessage::TIME_UP);
            break;
        case Game::GAME_OVER:
            centralMessage.queueMessage(CentralMessage::GAME_OVER);
            break;
    }*/
            
    
    
}
void Game::initEnd() {
    state = Game::END;
    endTimer.start();
    
    gamePlayInterfaceOpacity = 0;
    gamePlayInterfaceTweener.clearTweens();
    gamePlayInterfaceTweener.setup(1.f, 0, Sine::easeOut);
    gamePlayInterfaceTweener.addTween(&gamePlayInterfaceOpacity, 1);
    gamePlayInterfaceTweener.start();
}
void Game::initTransitionToStart() {
    state = Game::TRANSITION_TO_START;
    transitionToStartTimer.start();
    
    gamePlayInterfaceOpacity = 1;
    gamePlayInterfaceTweener.clearTweens();
    gamePlayInterfaceTweener.setup(1.f, 0, Sine::easeOut);
    gamePlayInterfaceTweener.addTween(&gamePlayInterfaceOpacity, -1);
    gamePlayInterfaceTweener.start();
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