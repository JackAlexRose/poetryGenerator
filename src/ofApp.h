#pragma once

#include "ofMain.h"
#include "ofxJSON.h"
#include "ofxGui.h"
#include "tools.h"
#include "ofxTextSuite.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
        void exit();

        void keyPressed(int key);
		void keyReleased(int key);
        bool loadDictionary();
        void buttonPressed(const void * sender);
        void getWordType(string word);
        void generateBody();
        string remixString(string line);
		
    ofxJSONElement json;
    ofxJSONElement authorJson;
    ofxJSONElement authorPage;
    vector<string> authors;
    vector<ofxJSONElement> dictionary;
    bool authorsLoaded;
    
    ofxPanel gui;
    ofxPanel gui2;
    ofxPanel gui3;
    ofxPanel gui4;
    vector<ofxButton*> buttons;
    string buttonName;
    
    vector<string> words;
    vector<string> nouns;
    vector<string> verbs;
    vector<string> adjectives;
    vector<string> adverbs;

    string mode;
    ofTrueTypeFont titleFont;
    ofTrueTypeFont bodyFont;
    string generatedPoemTitle;
    string generatedPoemBody;
    
    ofxTextBlock screenText;
    
    int titleStartY, bodyStartY, scrollSpeed;
};
