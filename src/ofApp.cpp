#include "ofApp.h"
#include <algorithm>

//--------------------------------------------------------------
void ofApp::setup(){
    scrollSpeed = 5;
    titleStartY = 40;
    bodyStartY = 90;
    mode = "mainMenu";
    ofSetBackgroundColor(255);
    
    titleFont.load("bonvoyage.otf", 22);
    bodyFont.load("bonvoyage.otf", 18);
    generatedPoemTitle = "Press space to generate a poem";
    generatedPoemBody = "Press backspace to choose a different poet";
    generatedPoemBody += '\n';
    generatedPoemBody += "Press down/up to scroll the poem";
    
    std::string url = "http://poetrydb.org/author";
    authorsLoaded = false;
    
    buttonName = "none";
    gui.setup();
    gui.setPosition(0, 0);
    gui2.setup();
    gui2.setPosition(210, 0);
    gui3.setup();
    gui3.setPosition(420, 0);
    gui4.setup();
    gui4.setPosition(630, 0);

    
    bool parsingSuccessful = authorJson.open(url);
    
    if (!parsingSuccessful)
    {
        ofLogNotice("ofApp::setup") << "Failed to parse JSON.";
    }
    else
    {
        const Json::Value& authorsArray = authorJson["authors"];
        
        for(Json::ArrayIndex i = 0; i < authorsArray.size(); i++){
            authors.push_back(authorsArray[i].asString());
            
            buttons.push_back(new ofxButton());
            buttons.back()->addListener(this, &::ofApp::buttonPressed);
            
            if(i < 39){
                gui.add(buttons.back()->setup(authorsArray[i].asString()));
            }
            else if(i > 39 && i < 78){
                gui2.add(buttons.back()->setup(authorsArray[i].asString()));
            }
            else if(i > 78 && i < 117){
                gui3.add(buttons.back()->setup(authorsArray[i].asString()));
            }
            else{
                gui4.add(buttons.back()->setup(authorsArray[i].asString()));
            }
        }
        cout << "Poetry DB connected, " << authors.size() << " authors loaded" << endl;
        authorsLoaded = true;
    }
    
    loadDictionary();
}

void ofApp::exit(){
    for(int i = 0; i < buttons.size(); i++){
        buttons[i]->removeListener(this, &::ofApp::buttonPressed);
    }
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
    ofSetColor(30);
    if(!authorsLoaded){
        return;
    }
    
    if(mode == "mainMenu"){
        gui.draw();
        gui2.draw();
        gui3.draw();
        gui4.draw();
    }
    else if(mode == "author"){
        titleFont.drawString(generatedPoemTitle, 20, titleStartY);
        ofSetColor(10);
        bodyFont.drawString(generatedPoemBody, 20, bodyStartY);
    }
}
 
//--------------------------------------------------------------
bool ofApp::loadDictionary(){
    bool parsingSuccessful;
    string rootPath = "dictionary/";
    string path;
    ofxJSONElement jsonInput;
    
    for(char c = 'A'; c <= 'Z'; c++){
        path = rootPath + 'D' + c + ".json";
        parsingSuccessful = jsonInput.open(path);
        if (!parsingSuccessful)
        {
            ofLogNotice("ofApp::setup") << "Failed to parse JSON.";
        }
        else
        {
            dictionary.push_back(jsonInput);
        }
    }
    cout << "Dictionary loaded" << endl;
}

void ofApp::keyPressed(int key){
    if(mode != "author"){
        return;
    }
    
    if(key == 57357){
        titleStartY+=scrollSpeed;
        bodyStartY+=scrollSpeed;
    }
    else if(key == 57359){
        titleStartY-=scrollSpeed;
        bodyStartY-=scrollSpeed;
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    if(mode != "author"){
        return;
    }
    if(key == ' '){
        generateBody();
        titleStartY = 40;
        bodyStartY = 90;
    }
    else if(key == 8){
        mode = "mainMenu";
        generatedPoemTitle = "Press space to generate a poem";
        generatedPoemBody = "Press backspace to choose a different poet";
        generatedPoemBody += '\n';
        generatedPoemBody += "Press down/up to scroll the poem";
        titleStartY = 40;
        bodyStartY = 90;
    }
}

//--------------------------------------------------------------
void ofApp::buttonPressed(const void * sender){
    words.clear();
    nouns.clear();
    adjectives.clear();
    adverbs.clear();
    verbs.clear();
    
    ofParameter<void> * button = (ofParameter<void>*)sender;
    buttonName = button->getName();
    cout << buttonName << endl;
    
    string url = "http://poetrydb.org/author/" + tools::replaceAll(buttonName, " ", "%20");
    cout << url << endl;
    bool parsingSuccessful = authorPage.open(url);
    
    if(!parsingSuccessful){
        cout << "Error parsing author page" << endl;
        return;
    }
    for(Json::ArrayIndex i = 0; i < authorPage.size(); i++){
        for(Json::ArrayIndex l = 0; l < authorPage[i]["lines"].size(); l++){
            vector<string> splitWords = tools::splitWords(authorPage[i]["lines"][l].asString());
            words.insert(words.end(), splitWords.begin(), splitWords.end());
        }
    }
    
    for(int i = 0; i < words.size(); i++){
        words[i] = tools::upperCase(words[i]);
        getWordType(words[i]);
    }
    
    cout << nouns.size() << " nouns" << endl;
    cout << adverbs.size() << " adverbs" << endl;
    cout << adjectives.size() << " adjectives" << endl;
    cout << verbs.size() << " verbs" << endl;

    
    cout << buttonName << "'s word profile loaded" << endl;
    mode = "author";
}

void ofApp::getWordType(string word){
    try{
        string type = dictionary[tools::letterToIndex(word[0])][word]["MEANINGS"]["1"][0].asString();

        if(type == "Noun"){
            if (std::find(nouns.begin(), nouns.end(), word) != nouns.end())
            {
                return;
            }
            nouns.push_back(word);
        }
        else if(type == "Adjective"){
            if (std::find(adjectives.begin(), adjectives.end(), word) != adjectives.end())
            {
                return;
            }
            adjectives.push_back(word);
        }
        else if(type == "Adverb"){
            if (std::find(adverbs.begin(), adverbs.end(), word) != adverbs.end())
            {
                return;
            }
            adverbs.push_back(word);

        }
        else if(type == "Verb"){
            if (std::find(verbs.begin(), verbs.end(), word) != verbs.end())
            {
                return;
            }
            verbs.push_back(word);
        }
        
    }
    catch(exception ex){
        cout << "Can't do this word: " << word << endl;
    }
}

void ofApp::generateBody(){
    int poemIndex = ofRandom(0, authorPage.size());
    cout << "poem index: " << poemIndex << endl;
    cout << authorPage.size() << " poems" << endl;
    string body;
    generatedPoemTitle = remixString(authorPage[poemIndex]["title"].asString());
    
    for(Json::ArrayIndex i = 0; i < authorPage[poemIndex]["lines"].size(); i++){
        if(authorPage[poemIndex]["lines"][i].asString() == ""){
            body += '\n';
            continue;
        }
        body += remixString(authorPage[poemIndex]["lines"][i].asString());
        body += ",";
        body += '\n';
    }
    generatedPoemBody = body;
}

string ofApp::remixString(string line){
    vector<string> lineSplit = tools::splitWords(line);
    int randomWordIndex;
    string remixedString;
    
    for(int i = 0; i < lineSplit.size(); i++){
        string upperWord = tools::upperCase(lineSplit[i]);
        try{
            string type = dictionary[tools::letterToIndex(upperWord[0])][upperWord]["MEANINGS"]["1"][0].asString();

            if(type == "Noun"){
                random_shuffle(nouns.begin(), nouns.end());
                lineSplit[i] = nouns[0];
            }
            else if(type == "Adjective"){
                random_shuffle(adjectives.begin(), adjectives.end());
                lineSplit[i] = adjectives[0];
            }
            else if(type == "Adverb"){
                random_shuffle(adverbs.begin(), adverbs.end());
                lineSplit[i] = adverbs[0];
            }
            else if(type == "Verb"){
                random_shuffle(verbs.begin(), verbs.end());
                lineSplit[i] = verbs[0];
            }
        }
        catch(exception ex){
            cout << "Can't do this word: " << upperWord << endl;
        }
        
        if(lineSplit[i].size() > 1) remixedString += tools::lowerCase(lineSplit[i]);
        else remixedString += lineSplit[i];
        if(i < lineSplit.size() - 1) remixedString += " ";
    }
    
    remixedString[0] = toupper(remixedString[0]);
    return remixedString;
}
