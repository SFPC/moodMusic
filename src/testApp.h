#pragma once

#include "ofMain.h"
#include "ofxFaceTracker.h"

class testApp : public ofBaseApp {
public:
	void setup();
	void update();
	void draw();
	void keyPressed(int key);
    void loadSounds();
	
	ofVideoGrabber cam;
	ofxFaceTracker leftTracker;
    ofxFaceTracker rightTracker;
	ExpressionClassifier leftClassifier;
    ExpressionClassifier rightClassifier;
    
    vector<ofSoundPlayer> leftSounds;
    vector<ofSoundPlayer> rightSounds;
    
    ofImage leftSideImage;
    ofImage rightSideImage;
    ofPixels leftSidePixels;
    ofPixels rightSidePixels;
};
