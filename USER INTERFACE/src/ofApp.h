#pragma once

#include "ofMain.h"
#include "ofxDatGui.h"
#include "SQLiteCpp.h"
#include "ofxJSONElement.h"

class ofApp : public ofBaseApp{

	public:
    
        std::string gameDB = ofToDataPath("../../../ADMIN INTERFACE/bin/data/game.sqlite", true);
		void setup();
		void update();
		void draw();
        vector<string> questions;
        vector<ofxDatGuiComponent*> componentButtons;
        void onButtonEvent(ofxDatGuiButtonEvent e);
        int page = 0;
        ofTrueTypeFont	helvetica14;
        vector<pair<string, int> > tokens;
        vector<string> tokenNames;

};
