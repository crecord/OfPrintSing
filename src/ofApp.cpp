#include "ofApp.h"
#define files 4
string fileNames[files];

//--------------------------------------------------------------
void ofApp::setup(){
    
    
    
    ofSerial g;
    g.listDevices();
    
    
    std::string port = "/dev/cu.PL2303-00001014";
    
    if (!printer.setup(port,
                       38400,
                       SerialDevice::DATA_BITS_EIGHT,
                       SerialDevice::PAR_NONE,
                       SerialDevice::STOP_ONE,
                       SerialDevice::FLOW_CTRL_HARDWARE))
    {
        ofLogError("ofApp::setup") << "Unable to connect to: " << port;
        ofExit();
    }
    
    // Set up hardware flow control if needed.
    printer.setDataTerminalReady();
    printer.setRequestToSend();
    // ignore museum hours!
    overRideHours = true;
    // Initialize the printer.
    printer.initialize();
    //ofSetFullscreen(true);
    fileNames[0] = "sheFile";
   // BaseCodes::TextUnderline(3);
    printer.setCharacterSmoothing(true);
    //printer.setFont(BaseCodes::TextFont(0));
    printer.setLineSpacing(0);
    //printer.setCharacterSize(BaseCodes::TextMagnification(0), BaseCodes::TextMagnification(0));
    printer.setCharacterSize(ofx::ESCPOS::BaseCodes::MAGNIFICATION_1X,
                             ofx::ESCPOS::BaseCodes::MAGNIFICATION_1X);
    
                     //ofx::ESCPOS::BaseCodes::TextAlignment(1));
    printer.setRotation(ofx::ESCPOS::BaseCodes::ROTATE_90_CW);
    //printer.setAlign(ofx::ESCPOS::BaseCodes::TextAlignment(1));
    //printer.setUpsideDown(true);
   
    if(bookmark.load("bookmark.xml")){
        string v =bookmark.getValue("place","0");
        ofLog()<< v;
        shePhraseIndex = ofToInt(v);
    }
    else{
        shePhraseIndex =0;
    }
    bookmark.clear();
    
    string loaded ="";
    XML.clear();
    bool getSomething = XML.loadFile(fileNames[0]+".xml");
    // ofLog( OF_LOG_NOTICE, ofToString(getSomething));
    //ofLog( OF_LOG_NOTICE, loaded);
    XML2.loadFile("sheFile.xml");
    XML2.pushTag("root");
    currentString = XML2.getValue("shePhrase","",shePhraseIndex);
    length= XML2.getNumTags("shePhrase");
    string bo = "array";
    //ofLog( OF_LOG_NOTICE, "Trying: "+ofToString(currentString));
    //ofLog( OF_LOG_NOTICE, ofToString(XML.getPushLevel()));
    changesVars(0);
    switcher = 0;
    timeBefore = ofGetSystemTime();
    index = 0;
    cutNum = 0;
    stringIndex =0;
    
    myPlayer.loadMovie("closeUpType.MOV");
    myPlayer.play();
    myPlayer.setVolume(0.0);
    isWait = true;
    waitCount = 0;
    printer.cut();
    blackSecCount = 0;
    blackTime = 20;
    screenWidth = ofGetScreenWidth();
    screenHeight = ofGetScreenHeight();
    vidWidth = myPlayer.getWidth();
    vidHeight = myPlayer.getHeight();
    startTime = ofGetUnixTime();
    ofBackground(0,0,0);
    onOfCycles = true;
    
    newHeight = screenHeight;
    newWidth = (vidWidth * newHeight) / vidHeight;
    
    sentencePlayBack.start();
    
}


void ofApp::write(string thewords, ofBuffer buff){
    string theString = thewords;
    buff.clear();
    buff.set(theString.c_str(), theString.size());
    printer.writeBytes(reinterpret_cast<unsigned char*>(buff.getBinaryBuffer()),buff.size());
}

void ofApp::changesVars(int index){
    timeGap =  1000 * XML.getAttribute("down" , "timeGapBeforeStroke", 2., index);
    timePress= 1000 * XML.getAttribute("down" , "duration", 2., index);
    keyValue= XML.getAttribute("down" , "char", "b", index);
    /*
    ofLog( OF_LOG_NOTICE, ofToString(timeGap));
    ofLog( OF_LOG_NOTICE, ofToString(timePress));
    ofLog( OF_LOG_NOTICE, ofToString(keyValue));
     */
}


//--------------------------------------------------------------
void ofApp::update(){
    myPlayer.update();
    sentencePlayBack.lock();
    float vol =sentencePlayBack.playerVolume;
    sentencePlayBack.unlock();
    myPlayer.setVolume(vol);
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    //printer.getPrinterStatusRealTime();
    
    if(isMuseumHours() || overRideHours){
    sentencePlayBack.draw(9);

    if (((blackSecCount < blackTime + 1)==false)){
        //draw a black screen
        ofSetColor(255);
        myPlayer.draw(0.0,0.0, newWidth, newHeight );
    }
    else {
        ofSetColor(0);
        // make it only happen once.
        if ((((ofGetUnixTime() - startTime)/60)%2 == 0)&& (((ofGetUnixTime() - startTime)/60)>0)){
            startTime = ofGetUnixTime();
            float percentage = ofRandom(0.0, .5);
            // play the video at a random position during the first half of the video
            myPlayer.setPosition(percentage);
            myPlayer.setPaused(false);
        }
    }


    if (ofGetFrameNum()% 10 ==0 ){
        if (shePhraseIndex < length){
        
            if (blackSecCount < blackTime + 1){
                ofSetColor(0);
                //ofRect(0,0,ofGetWindowWidth(),ofGetWindowHeight());
                if (blackSecCount == 0){
                    myPlayer.setPaused(true);
                }
                else if (blackSecCount == blackTime){
                    float percentage = ofRandom(0.0, .5);
                    myPlayer.setPosition(percentage);
                    myPlayer.setPaused(false);
                    sentencePlayBack.newString(currentString);
                }
                else{
                    
                }
                blackSecCount++;
            }

            else if (waitCount < 20){
                //mySound.setPaused(true);
                //myPlayer.setVolume(0.0);
                printer.setCharacterSize(ofx::ESCPOS::BaseCodes::MAGNIFICATION_1X,
                                         ofx::ESCPOS::BaseCodes::MAGNIFICATION_1X);
                write(" \n", buffer);
                waitCount ++;
            }
            else if(currentString.length() == stringIndex)
            {
                // newSentanceTime!
                float percentage = ofRandom(0.6, .9);
                myPlayer.setPosition(percentage);
                printer.cut();
                stringIndex =0;
                shePhraseIndex ++;
                currentString = XML2.getValue("shePhrase","",shePhraseIndex);
                
                //this is what the sentence looks like now please handle the sound.
                waitCount =0;
                blackSecCount =0;
                blackTime = ofRandom(0,80);
            }
            
            else if (ofToString(currentString[stringIndex]) == " "){
                //print the pauses between words
               // myPlayer.setVolume(0.0);
                printer.setCharacterSize(ofx::ESCPOS::BaseCodes::MAGNIFICATION_1X,
                                         ofx::ESCPOS::BaseCodes::MAGNIFICATION_1X);
                write(" \n", buffer);
                write(" \n", buffer);
                write(" \n", buffer);
                stringIndex ++;
                if(currentString.length() == stringIndex)
                {
                    waitCount = 0;
                }
            }
            
            else {
                //print each letter one by one
                //myPlayer.setVolume(.9);
                printer.setCharacterSize(ofx::ESCPOS::BaseCodes::MAGNIFICATION_2X,
                                         ofx::ESCPOS::BaseCodes::MAGNIFICATION_2X);
                string keyValPhrase = ofToString (currentString[stringIndex]);
                write(keyValPhrase + "\n", buffer);
                stringIndex ++;
                if(currentString.length() == stringIndex)
                {
                    waitCount = 0;
                }
            }
        }
        
        
        else{
            if (cutNum < 5){
                printer.cut();
                float percentage = ofRandom(0.5, .8);
                myPlayer.setPosition(percentage);
                cutNum ++;
            }
        }
        
        if ((((ofGetUnixTime() - startTime)/60)%2 == 0) && (((ofGetUnixTime() - startTime)/60)>0) ){
            //ofLog(OF_LOG_NOTICE, "setting it to false!" );
            onOfCycles = false;
            myPlayer.setPaused(true);
            startTime = ofGetUnixTime();
        }
    }
    }
    else{
        myPlayer.setPosition(0);
        myPlayer.setPaused(true);
        myPlayer.draw(0.0,0.0, newWidth, newHeight );
    }
    
}

void ofApp::exit(){
    ofLog()<<"ext";
    sentencePlayBack.stop();
    bookmark.addValue("place", ofToString(shePhraseIndex));
    bookmark.save("bookmark.xml");
}


void ofApp::keyPressed  (int key){
    ofExit();
}

bool ofApp::isMuseumHours(){
    int day = ofGetWeekday();
    int hour = ofGetHours();
    if ( (day == 3) || (day == 4)){
        if(11 <= hour && hour <=18 ){
            return true;
        }
        else {
            return false;
        }
    }
    else if ( (day == 5) || (day == 6)){
        if(11 <= hour && hour <=20 ){
            return true;
        }
        else {
            return false;
        }
    }
    else if ( day == 0){
        if((11 <= hour) && (hour <=17) ){
            return true;
        }
        else {
            return false;
        }
    }
    else{
        return false;
    }
}