OfPrintSing
==========

![Screenshot](https://github.com/crecord/OfPrintSing/blob/master/installShot.jpg)

![Screenshot](https://github.com/crecord/OfPrintSing/blob/master/receipt.png)

![Screenshot](https://github.com/crecord/OfPrintSing/blob/master/photoProcess.jpg)

Description
-----------

An [openFrameworks](http://www.openframeworks.cc/) project for a singing and printing sculpture.

Features:
- Reads sentences from an XML file 
- Prints a single sentence per receipt with big margins. 
- Plays a video for the duration of printing the sentence. 
- plays audio only while printing words(uses multi-threading) 
- has different possible speeds
- I scraped the novel Anna Karenina for every sentence that started with the word "She." The python code I used to do so is also included and is called "sheScrape.py". 

Dependencies 
------------

First you need [openFrameworks](http://www.openframeworks.cc/). 
And the following ofxAddons: 

ofxXmlSettings
ofxESCPOS
ofxIO
ofxSerial

I used a T20 Epson printer. Special thank you to Christopher Baker for his awesome ofxESCPOS Addon! 

This was last tested on a 10.9.5 mac with Xcode 5.1.1 and OF 0.8.1