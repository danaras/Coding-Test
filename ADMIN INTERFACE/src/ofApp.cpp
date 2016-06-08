#include "ofApp.h"



void ofApp::setup()
{
    ofSetWindowPosition(0, 0);
    ofSetWindowShape(2450, 1080);
    int x = 100;
    int y = 100;
    int p = 30;
    helvetica14.load("HelveticaNeueLTCom-Roman.ttf", 14, true, true);
    helvetica14.setLineHeight(18.0f);
    helvetica14.setLetterSpacing(1.037);
    vector<string> options = {"Create a new question", "Edit an existing question", "View All Data"};
    dropdown = new ofxDatGuiDropdown("Choose an Option", options);
    dropdown->setPosition(x, y);
    dropdown->onDropdownEvent(this, &ofApp::onDropdownEvent);
}

void ofApp::update()
{
    dropdown -> update();
    for(int i=0; i<textComponents.size(); i++) textComponents[i]->update();
    for(int i=0; i<textComponentsEdit.size(); i++) textComponentsEdit[i]->update();
    for(int i=0; i<tokens.size(); i++) tokens[i]->update();
    for(int i=0; i<compButtons.size(); i++) compButtons[i]->update();
    
    if(view != NULL && displayView==true){
        view -> update();
    }
    if(displayEdit){
        for(int i=0; i<editComps.size(); i++) editComps[i]->update();
        for(int i=0; i<deleteTokenButtons.size(); i++) deleteTokenButtons[i]->update();
    }
}

void ofApp::draw()
{
    ofBackground(0,0,0);
    if(displaySS)
    {
        int xData = 100;
        int yDataTitle = 150;
        int yData = 170;
        helvetica14.drawString("Question", xData, yDataTitle);
        helvetica14.drawString("Token", xData + 300, yDataTitle);
        helvetica14.drawString("Value for 'YES'", xData + 600, yDataTitle);
        helvetica14.drawString("Value for 'NO'", xData + 900, yDataTitle);
        try
        {
            // Open a database file in create/write mode
            SQLite::Database db(gameDB);

            SQLite::Statement query(db, "SELECT * FROM rpg ORDER BY question");

            int i = 0;
            while (query.executeStep())
            {
                xData = 100;
                
                helvetica14.drawString(query.getColumn(0), xData, yData);
                helvetica14.drawString(query.getColumn(1), xData + 300, yData);
                helvetica14.drawString(query.getColumn(2), xData + 600, yData);
                helvetica14.drawString(query.getColumn(3), xData + 900, yData);
                
                yData +=20;
            }
        }
        catch (std::exception& e)
        {
            ofLogError() << "SQLite exception: " << e.what();
        }
    }
    else if(displayView){
        view -> draw();
    }
    else if(displayEdit){
        for(int i=0; i<textComponentsEdit.size(); i++) textComponentsEdit[i]->draw();
        for(int i=0; i<editComps.size(); i++) editComps[i]->draw();
        for(int i=0; i<compButtons.size(); i++) compButtons[i]->draw();
        for(int i=0; i<deleteTokenButtons.size(); i++) deleteTokenButtons[i]->draw();
    }
    else if(displayCreate){
        for(int i=0; i<tokens.size(); i++) tokens[i]->draw();
        for(int i=0; i<textComponents.size(); i++) textComponents[i]->draw();
        for(int i=0; i<compButtons.size(); i++) compButtons[i]->draw();
    }
    dropdown->draw();
}

/*
 event listeners
 */

void ofApp::onButtonEvent(ofxDatGuiButtonEvent e)
{
    string buttonName = e.target->getLabel();
    cout << "onButtonEvent: " << e.target->getLabel() << endl;
    
    //On click add the values from input as a new entry
    if(buttonName=="ADD"){
        for(int i = 0; i < textComponents.size(); i++){
            if(textComponents[i]->getLabel()=="QUESTION"){
                question = textComponents[i]->getText();
                std::transform(question.begin(), question.end(), question.begin(), ::tolower);
                ofLog()<<"Question: "<< question << endl;
            }
        }
        try
        {
            // Open a database file in create/write mode
            SQLite::Database db(gameDB, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE);
            ofLogNotice() << "SQLite database file '" << db.getFilename() << "' opened successfully";

            for(int i = 0; i < tokens.size(); i += 3){
                string tokens0 = tokens[i] -> getText();
                std::transform(tokens0.begin(), tokens0.end(), tokens0.begin(), ::tolower);
                string tokens1 = tokens [i+1] ->getText();
                std::transform(tokens1.begin(), tokens1.end(), tokens1.begin(), ::tolower);
                string tokens2 = tokens [i+2] ->getText();
                std::transform(tokens2.begin(), tokens2.end(), tokens2.begin(), ::tolower);
                ofLog()<<"Token 1: "<< tokens0 << ", " << tokens1 << ", " << tokens2 << endl;
                ofLog()<<"here" <<endl;
                ofLog()<<"INSERT INTO rpg VALUES (\"" + question + "\", \"" + tokens0 + "\", " + tokens1 + ", " + tokens2 + ")"<<endl;
                db.exec("INSERT OR IGNORE INTO rpg VALUES (\"" + question + "\", \"" + tokens0 + "\", " + tokens1 + ", " + tokens2 + ")");
                //ofLog()<< "tokens.size: "<<tokens.size()<<endl;
            }
            
        }
        catch (std::exception& e)
        {
            ofLogError() << "SQLite exception: " << e.what();
        }
        
        tokens.clear();
        for(int i = 0; i < textComponents.size(); i++){
            if(textComponents[i]->getLabel()=="#TOKENS"){
                textComponents[i]->setText("how many tokens?");
                
            }
            if(textComponents[i]->getLabel()=="QUESTION"){
                textComponents[i]->setText("write here");
                
            }
        }
        
        
    }
    
//I didn't finish coding the following functions(buttons). However I wrote descriptions of the ways I would do it.
    if(buttonName=="ADD TOKEN"){
        //adds another row of (token name, yes value, no value) to the edit view only
    }
    if(buttonName.find("DELETE TOKEN") != string::npos){
        //the number id of the delete token button will tell which row of tokens to deleten (both from the UI and SQLite table)
    }
    if(buttonName=="SAVE CHANGES"){
        //On click SAVE THE CHANGES
        //First delete the question (questionToBeEdited) and all of it's related tokens from database and then add the new edited question to the database (like with "ADD" button)
        for(int i = 0; i < textComponents.size(); i++){
            if(textComponents[i]->getLabel()=="QUESTION"){
                question = textComponents[i]->getText();
                std::transform(question.begin(), question.end(), question.begin(), ::tolower);
                
                ofLog()<<"Question: "<< question << endl;
            }
        }
    }
}

void ofApp::onDropdownEvent(ofxDatGuiDropdownEvent e)
{
    //Show the questions
    cout << "onDropdownEvent: " << e.child << endl;
    if (e.child==0){
        displayCreate = true;
        displaySS = false;
        displayView = false;
        displayEdit = false;
        tokens.clear();
        textComponents.clear();
        compButtons.clear();
        int x = 100;
        int y = 250;
        int p = 10;
        ofxDatGuiTextInput* textComponent;
        textComponent = new ofxDatGuiTextInput("Question", "write here");
        textComponent->setPosition(x, y);
        textComponent->onTextInputEvent(this, &ofApp::onTextInputEvent);
        textComponents.push_back(textComponent);
        x += textComponent->getWidth() + p;
        textComponent = new ofxDatGuiTextInput("#Tokens", "how many tokens?");
        textComponent->setPosition(x, y);
        textComponent->onTextInputEvent(this, &ofApp::onTextInputEvent);
        textComponents.push_back(textComponent);
    }
    if (e.child==1){
        textComponents.clear();
        displayCreate = false;
        displayEdit = false;
        displayView = true;
        displaySS = false;
        int width = 540;
        int x = 100;
        int y = 250;
        
        view = new ofxDatGuiScrollView("ScrollView", 8);
        view->setWidth(width);
        view->setPosition(x, y);
        view->onScrollViewEvent(this, &ofApp::onScrollViewEvent);
        try
        {
            // Open a database file in read mode
            SQLite::Database db(gameDB);
            SQLite::Statement query(db, "SELECT question FROM rpg ORDER BY question");
            while (query.executeStep())
            {
                string questionSlide = query.getColumn(0);
                if(view->get(questionSlide) == NULL){
                    view->add(query.getColumn(0));
                }
            }
        }
        catch (std::exception& e)
        {
            ofLogError() << "SQLite exception: " << e.what();
        }
    }
    if (e.child==2){
        textComponents.clear();
        displayCreate = false;
        displayEdit = false;
        displayView = false;
        displaySS = true;
    }
}


void ofApp::onTextInputEvent(ofxDatGuiTextInputEvent e)
{
    cout << "onButtonEvent: " << e.text <<" "<< e.target->getLabel() << endl;
    if (e.target->getLabel()=="#TOKENS"){
        tokens.clear();
        numTokens = std::stoi(e.text);
        int y = 300;
        int x = 100;
        int p = 30;
        ofLog()<<"numtokens: "<<numTokens<<endl;
        createComponents(y, "write here", "write here", "write here", tokens, numTokens, "Add");
    }
}

void ofApp::onScrollViewEvent(ofxDatGuiScrollViewEvent e)
{
    displayView = false;
    displayEdit = true;
    
    textComponentsEdit.clear();
    compButtons.clear();
    editComps.clear();
    
    int x = 100;
    int y = 250;
    cout << e.target->getLabel() << " [index " << e.index << "] selected in [" << e.parent->getName() << "]" << endl;
    ofxDatGuiTextInput* textComponent;
    
    textComponent = new ofxDatGuiTextInput("Edit Question", e.target->getLabel());
    textComponent->setPosition(x, y);
    textComponent->onTextInputEvent(this, &ofApp::onTextInputEvent);
    textComponentsEdit.push_back(textComponent);
    int yEdit =300;
    int p = 10;
    string questionLabel = e.target->getLabel();
    std::transform(questionLabel.begin(), questionLabel.end(), questionLabel.begin(), ::tolower);
    questionToBeEdited=questionLabel;
    int howManyTokens = 0;
    try
    {
        // Open a database file in create/write mode
        SQLite::Database db(gameDB);
        SQLite::Statement query(db, "SELECT * FROM rpg WHERE question IN ('"+questionLabel+"')");
        while (query.executeStep())
        {
            ofLogNotice() << "row (" << query.getColumn(0) << ", \"" << query.getColumn(1) << ", \"" << query.getColumn(2) << ", \"" << query.getColumn(3) << "\")";
            
            createComponents(yEdit, query.getColumn(1), query.getColumn(2), query.getColumn(3), editComps, 1, "Save Changes");
            howManyTokens++;
            deleteTokenButton = new ofxDatGuiButton("Delete Token " + to_string(howManyTokens));
            deleteTokenButton->setPosition(3*(p+dropdown->getWidth())+x,yEdit);
            deleteTokenButton->onButtonEvent(this, &ofApp::onButtonEvent);
            deleteTokenButtons.push_back(deleteTokenButton);
            yEdit+= editComps[0]->getHeight() + p;
        }
    }
    catch (std::exception& e)
    {
        ofLogError() << "SQLite exception: " << e.what();
    }
    ofxDatGuiComponent* addTokenButton;
    addTokenButton = new ofxDatGuiButton("Add Token");
    addTokenButton->setPosition(3*(dropdown->getWidth()+p)+x, 100 + dropdown->getHeight()+p);
    addTokenButton->onButtonEvent(this, &ofApp::onButtonEvent);
    compButtons.push_back(addTokenButton);

    addTokenButton = new ofxDatGuiButton("Delete Question");
    addTokenButton->setPosition(2*(dropdown->getWidth()+p)+x,100);
    addTokenButton->onButtonEvent(this, &ofApp::onButtonEvent);
    compButtons.push_back(addTokenButton);
}

void ofApp::createComponents(int y, string tokenName, string tokenYes, string tokenNo, vector<ofxDatGuiTextInput*>& comps, int j, string buttonName){
    int x = 100;
    int p = 10;
    ofxDatGuiComponent* componentButton;
    componentButton = new ofxDatGuiButton(buttonName);
    componentButton->setPosition(3*(dropdown->getWidth()+p)+x,100);
    componentButton->onButtonEvent(this, &ofApp::onButtonEvent);
    compButtons.push_back(componentButton);
    
    for(int i=0; i<j; i++){
        ofLog()<< "tokens.size inside loop: "<<tokens.size()<<endl;
        x = 100;
        
        ofxDatGuiTextInput* comp;
        comp = new ofxDatGuiTextInput("Token", tokenName);
        comp->setPosition(x, y);
        comp->onTextInputEvent(this, &ofApp::onTextInputEvent);
        comps.push_back(comp);
        x += comp->getWidth() + p;
        comp = new ofxDatGuiTextInput("'Yes' Value", tokenYes);
        comp->setPosition(x, y);
        comp->onTextInputEvent(this, &ofApp::onTextInputEvent);
        comps.push_back(comp);
        x += comp->getWidth() + p;
        comp = new ofxDatGuiTextInput("'No' Value ", tokenNo);
        comp->setPosition(x, y);
        comp->onTextInputEvent(this, &ofApp::onTextInputEvent);
        comps.push_back(comp);
        
        y += comp->getHeight() + p;
    }
}


