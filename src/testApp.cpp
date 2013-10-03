#include "testApp.h"

using namespace ofxCv;
using namespace cv;

void testApp::setup() {
	ofSetVerticalSync(true);
	cam.initGrabber(640, 480);
	
	leftTracker.setup();
	leftTracker.setRescale(.5);
    
    rightTracker.setup();
	rightTracker.setRescale(.5);
    
    
}

void testApp::update() {
	cam.update();
	if(cam.isFrameNew()) {
        //        ofPixels pixels = cam.getPixelsRef();
        ofPixelsRef pixels = cam.getPixelsRef();
        
        pixels.cropTo(leftSidePixels, 0, 0, ofGetWidth()/2, ofGetHeight());
        pixels.cropTo(rightSidePixels, ofGetWidth()/2, 0, ofGetWidth()/2, ofGetHeight());
        
        leftSideImage.setFromPixels(leftSidePixels.getPixels(), ofGetWidth()/2, ofGetHeight(), OF_IMAGE_COLOR);
        rightSideImage.setFromPixels(rightSidePixels.getPixels(), ofGetWidth()/2, ofGetHeight(), OF_IMAGE_COLOR);
        
		if(leftTracker.update(toCv(leftSidePixels))) {
			leftClassifier.classify(leftTracker);
		}
        
        if(rightTracker.update(toCv(rightSidePixels))) {
			rightClassifier.classify(rightTracker);
		}
	}
}

void testApp::draw() {
	ofSetColor(255);
    cam.draw(0, 0);
    //	leftSideImage.draw(0, 0);
    //   rightSideImage.draw(ofGetWidth()/2, 0);
    leftTracker.draw();
    rightTracker.draw();
    ofSetColor(255, 0, 0);
    //    rightTracker.draw();
    
	
	int w = 100, h = 12;
	ofPushStyle();
	ofPushMatrix();
	ofTranslate(5, 10);
	int n = leftClassifier.size();
	int primary = leftClassifier.getPrimaryExpression();
    for(int i = 0; i < n; i++){
        cout << rightClassifier.getDescription(i) << ": " << rightClassifier.getProbability(i) << endl;
		ofSetColor(i == primary ? ofColor::red : ofColor::black);
		ofRect(0, 0, w * leftClassifier.getProbability(i) + .5, h);
		ofSetColor(255);
		ofDrawBitmapString(leftClassifier.getDescription(i), 5, 9);
		ofTranslate(0, h + 5);
        
        leftSounds.at(i).setVolume(leftClassifier.getProbability(i));
        rightSounds.at(i).setVolume(rightClassifier.getProbability(i));
    }
	ofPopMatrix();
	ofPopStyle();
	
	ofDrawBitmapString(ofToString((int) ofGetFrameRate()), ofGetWidth() - 20, ofGetHeight() - 10);
	drawHighlightString(
                        string() +
                        "r - reset\n" +
                        "e - add expression\n" +
                        "a - add sample\n" +
                        "s - save expressions\n"
                        "l - load expressions",
                        14, ofGetHeight() - 7 * 12);
}

// here we'll load a sound for each expression
void testApp::loadSounds() {
    cout << leftClassifier.size() << endl;
    
    
    ofDirectory leftDir("sounds/left");
    ofDirectory rightDir("sounds/right");
    
    leftDir.listDir();     // ?
    rightDir.listDir();     // ?
    
    int numExpressions = leftClassifier.size();
    
    for (int i = 0; i < numExpressions; i++) {
        //        cout << "adding " << dir.getPath(i) << endl;
        // add a sound player
        
        
        // - zach
        //        ofSoundPlayer *leftSound2;
        //        leftSound2 = new ofSoundPlayer();
        //        leftSound2->loadSound("...");
        //        leftSounds.push_back(leftSound2);
        //        leftSounds[0]->play();
        
        
        
        
        ofSoundPlayer leftSound;
        ofSoundPlayer rightSound;
        
        leftSounds.push_back(leftSound);
        leftSounds.back().loadSound(leftDir.getPath(i));
        leftSounds.back().play();
        leftSounds.back().setVolume(0);
        leftSounds.back().setLoop(true);

        rightSounds.push_back(rightSound);
        rightSounds.back().loadSound(rightDir.getPath(i));
        rightSounds.back().play();
        rightSounds.back().setVolume(0);
        rightSounds.back().setLoop(true);
        
    }
}

void testApp::keyPressed(int key) {
	if(key == 'r') {
		leftTracker.reset();
		leftClassifier.reset();
	}
	if(key == 'e') {
		leftClassifier.addExpression();
	}
	if(key == 'a') {
		leftClassifier.addSample(leftTracker);
	}
	if(key == 's') {
		leftClassifier.save("expressions");
	}
	if(key == 'l') {
		leftClassifier.load("expressions");
        rightClassifier.load("expressions");
        loadSounds();
	}
}