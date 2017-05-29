#pragma once

#include "ofMain.h"
#include "ofxMidi.h"
#include "ofxOSC.h"
#include "ofxGUI.h"

#define FINGER_COUNT 5

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

        void checkToggles();
        void sendNote(int indexControl, int aValue, int aAddNote = 0);
    
        void sendControlChange(int indexControl, int aValue, bool isX);
        int getControlIndex(int aIndexFinger) {return aIndexFinger + 1;};
    
        int mLastNote[FINGER_COUNT];
private:
    ofxOscReceiver  mOSCReceiver;
    ofxMidiOut      midiOut;
    ofxPanel        mGui;
    ofxToggle       mProgXButton[FINGER_COUNT];
    ofxToggle       mProgYButton[FINGER_COUNT];
    ofxToggle       mNoteButton[FINGER_COUNT];
    
    std::vector<int> mScale;
		
};
