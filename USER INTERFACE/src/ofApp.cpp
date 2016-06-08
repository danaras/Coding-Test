#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    helvetica14.load("HelveticaNeueLTCom-Roman.ttf", 14, true, true);
    helvetica14.setLineHeight(18.0f);
    helvetica14.setLetterSpacing(1.037);
    
    try
    {
        // Open a database file in read mode
        SQLite::Database db(gameDB);
        SQLite::Statement query(db, "SELECT * FROM rpg ORDER BY question");
        
        while (query.executeStep())
        {
            string questionSlide = query.getColumn(0);
            string token = query.getColumn(1);
            
            ofLog() << "Question Slide: " << questionSlide << endl;
            
            //get unique questions
            if (binary_search (questions.begin(), questions.end(), questionSlide) == false ){
                questions.push_back(questionSlide);
            }
            //get unique tokens
            if (binary_search (tokenNames.begin(), tokenNames.end(), token) == false ){
                tokenNames.push_back(token);
            }
        }
    }
    catch (std::exception& e)
    {
        ofLogError() << "SQLite exception: " << e.what();
    }
    ofLog()<<"width: "<<ofGetWidth()<<endl;
    int x = 100;
    int y = ofGetHeight()-100;
    ofxDatGuiComponent* componentButton;
    componentButton = new ofxDatGuiButton("YES");
    componentButton->setPosition(x,y);
    componentButton->onButtonEvent(this, &ofApp::onButtonEvent);
    componentButton->setTheme(new ofxDatGuiThemeWireframe);
    componentButtons.push_back(componentButton);
    x = ofGetWidth()-100-componentButtons[0]->getWidth();
    componentButton = new ofxDatGuiButton("NO");
    componentButton->setPosition(x,y);
    componentButton->onButtonEvent(this, &ofApp::onButtonEvent);
    componentButton->setTheme(new ofxDatGuiThemeWireframe);

    componentButtons.push_back(componentButton);

    for(int i = 0; i < tokenNames.size(); i++){
        tokens.push_back(make_pair(tokenNames[i],0));
        //ofLog() << "Tokens in the vector: " << tokens[i].first << ", " << tokens[i].second<<endl;
    }
}

//--------------------------------------------------------------
void ofApp::update(){
    if(page<questions.size()){
        for(int i=0; i<componentButtons.size(); i++) componentButtons[i]->update();
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    ofBackground(30,30,30);
    ofEnableAlphaBlending();

    if(page<questions.size()){
        for(int i=0; i<componentButtons.size(); i++) componentButtons[i]->draw();
        ofSetColor(225);
        helvetica14.drawString(questions[page], 100, 445);
    }
    else
    {
        ofSetColor(225);
        int y = 100;
        int x = 100;
        int p = 20;
        helvetica14.drawString("0", x+495, y-2*p);
        for(int i = 0; i < tokens.size(); i++){
            helvetica14.drawString(tokens[i].first, x, y);
            ofDrawRectangle(x+500, y-p, tokens[i].second*p, p);
            y += 2*p;
        }
    
    }

}
void ofApp::onButtonEvent(ofxDatGuiButtonEvent e){
    try
    {
        // Open a database file in read mode
        SQLite::Database db(gameDB);
        SQLite::Statement query(db, "SELECT * FROM rpg WHERE question IN ('"+questions[page]+"')");
        while (query.executeStep())
        {
            string tokenInQuestion = query.getColumn(1);
            int tokenYesValue = query.getColumn(2);
            int tokenNoValue = query.getColumn(3);
            //ofLog()<< "question and token: "<< query.getColumn(0)<<", "<<query.getColumn(1)<<endl;

            for(int i = 0; i < tokens.size(); i++){
                if(tokenInQuestion == tokens[i].first){
                    if(e.target->getLabel()=="YES"){
                        tokens[i].second += tokenYesValue;
                    }
                    if(e.target->getLabel()=="NO"){
                        tokens[i].second += tokenNoValue;
                    }
                }
            }
        }
    }
    catch (std::exception& e)
    {
        ofLogError() << "SQLite exception: " << e.what();
    }
    page++;
}

