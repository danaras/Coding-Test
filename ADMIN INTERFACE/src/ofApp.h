#pragma once

#include "ofMain.h"
#include "ofxDatGui.h"
#include "SQLiteCpp.h"
#include "ofxJSONElement.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
        std::string gameDB = ofToDataPath("game.sqlite", true);
        ofTrueTypeFont	helvetica14;
        ofxDatGuiDropdown* dropdown;
        vector<ofxDatGuiTextInput*> textComponents;
        vector<ofxDatGuiTextInput*> textComponentsEdit;
        vector<ofxDatGuiComponent*> compButtons;
        vector<ofxDatGuiComponent*> deleteTokenButtons;
    
        int numTokens;
        vector<ofxDatGuiTextInput*> tokens;
        vector<ofxDatGuiTextInput*> editComps;
        ofxDatGuiTextInput* componentToken;
        bool questionTokenExists = false;
        string questionToBeEdited;
 
        ofxDatGuiComponent* saveChangesButton;
        ofxDatGuiComponent* deleteTokenButton;
        ofxDatGuiComponent* deleteQuestionButton;
        string question;
        int numTokensToSave;
        vector<string> tokenValues;
        ofxJSONElement data;
        bool displaySS = false;
        ofxDatGuiScrollView* view;
        bool displayView = false;
        vector <string> slideQuestions;
        bool displayEdit = false;
        bool displayCreate = false;
		void onButtonEvent(ofxDatGuiButtonEvent e);
        void onDropdownEvent(ofxDatGuiDropdownEvent e);
        void onTextInputEvent(ofxDatGuiTextInputEvent e);
        void onScrollViewEvent(ofxDatGuiScrollViewEvent e);
        void createComponents(int y, string tokenName, string tokenYes, string tokenNo, vector<ofxDatGuiTextInput*>& comps, int j, string buttonName);
};
