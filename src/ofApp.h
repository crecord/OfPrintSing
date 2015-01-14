#pragma once

#include "ofMain.h"
#include "ofxESCPOS.h"
#include "ofxXmlSettings.h"
#include "threadedObject.h"

class ofApp : public ofBaseApp{
public:
    void setup();
    void update();
    void draw();
    ofBuffer buffer;
    void exit();
    void newFile(int switcherz);
    void changesVars(int index);
    ofSoundPlayer mySound;
    ofxXmlSettings XML;
    ofxXmlSettings XML2;
    ofxXmlSettings bookmark;
    int fileSwitcher;
    int index;
    int switcher;
    int length;
    float timeGap;
    float timePress;
    string keyValue;
    string currentString;
    int shePhraseIndex;
    int stringIndex;
    int timeBefore;
    int timeNow;
    int cutNum;
    int timeDiff;
    int xmlIndex;
    void write(string thewords, ofBuffer buff);
    bool isWait;
    int waitCount;
    ofVideoPlayer myPlayer;
    int screenWidth;
    int screenHeight;
    float vidWidth;
    float vidHeight;
    int blackSecCount;
    int blackTime;
    bool onOfCycles;
    int startTime;
    ofx::ESCPOS::DefaultSerialPrinter printer;
    float newHeight;
    float newWidth;
    void keyPressed(int key);
    threadedObject	sentencePlayBack;
    bool isMuseumHours();
    bool overRideHours; 
    
		
};
