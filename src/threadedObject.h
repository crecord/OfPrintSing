#ifndef _THREADED_OBJECT
#define _THREADED_OBJECT

#include "ofMain.h"

// this is not a very exciting example yet
// but ofThread provides the basis for ofNetwork and other
// operations that require threading.
//
// please be careful - threading problems are notoriously hard
// to debug and working with threads can be quite difficult


class threadedObject : public ofThread{

	public:


    string currentSentence;
    int count;
    bool sentenceIsWaiting;
    float playerVolume;
    
    
                    // threaded fucntions that share data need to use lock (mutex)
	                // and unlock in order to write to that data
	                // otherwise it's possible to get crashes.
	                //
	                // also no opengl specific stuff will work in a thread...
	                // threads can't create textures, or draw stuff on the screen
	                // since opengl is single thread safe

		//--------------------------
		threadedObject(){
			count = 0;
            currentSentence= "";
            sentenceIsWaiting = false;
		}

		void start(){
            startThread(false, false);   // blocking, verbose
        }

        void stop(){
            stopThread();
        }
    
        void newString (string sent){
            currentSentence = sent;
            sentenceIsWaiting = true;
        }

		//--------------------------
		void threadedFunction(){

			while( isThreadRunning() != 0 ){
				//if( lock() ){
                    if (sentenceIsWaiting){
                        
                        //ofLog()<< currentSentence;
                        ofSleepMillis(5000);
                        //5485
                        //play;
                        ofLog()<< "play1";
                        if(lock()){
                            playerVolume = .9;
                            unlock();
                        }
                        else{
                            ofLog()<<"sorry, it was locked";
                        }
                        
                        
                        
                        
                        for(int i=0; i < currentSentence.length(); i++){
                            if (ofToString(currentSentence [i]) == " "){
                                //mute
                                //ofLog()<< "mute2";
                                if(lock()){
                                    playerVolume = 0.0;
                                    unlock();
                                }
                                else{
                                    ofLog()<<"sorry, it was locked";
                                }
                                //myPlayer.setVolume(0.0);
                                ofSleepMillis(350);
                                
                                //play
                               // ofLog()<< "play2";
                                if(lock()){
                                    playerVolume = .9;
                                    unlock();
                                }
                                else{
                                    ofLog()<<"sorry, it was locked";
                                }
                            }
                            else{
                                // this is a letter
                                // it is already playing so do nothing but wait
                                ofSleepMillis(200);
                            }
                        }
                        // mute it
                        //ofLog() << "mute1";
                        if(lock()){
                            playerVolume = 0.0;
                            unlock();
                        }
                        else{
                            ofLog()<<"sorry, it was locked";
                        }
                        sentenceIsWaiting = false;
                        
                    }
					//unlock();
				//}
			}
            
		}

		//--------------------------
		void draw(int mils){

			string str = "I am a slowly increasing thread. \nmy current count is: ";

			//if( lock() ){
				str += ofToString(count);
				unlock();
			//}else{
			//	str = "can't lock!\neither an error\nor the thread has stopped";
            
			//}
			ofDrawBitmapString(str, 50, 56);
		}
    
 



};

#endif
