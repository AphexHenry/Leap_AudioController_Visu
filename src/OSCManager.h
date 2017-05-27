/*
 *  OSCSender.h
 *  SurfaceBasic
 *
 *  Created by Baptiste Bohelay on 12-03-23.
 *  Copyright 2012 Tangible Interaction Design Inc. All rights reserved.
 *
 */

#ifndef OSC_MANAGER_H
#define OSC_MANAGER_H

#include "cinder/app/App.h"

#include "OscSender.h"
#include "OscListener.h"
#include <sstream>

// We'll create a new Cinder Application by deriving from the BasicApp class
class OSCManager
{
public:
	void setup();
	void SendOSC(std::string aAdress, float aValue);
	void SendOSC(std::string aAdress, int aValue);
    void SendNote(int aId, int aValue);
    void SendProgram(int aId, int aX, int aY);
	void update();
	void Listen();
    void GetValueForAdress(std::string aMessage, float &aValue);
    void GetValueForAdress(std::string aMessage, int &aValue);
    void GetValueForAdress(std::string aMessage, bool &aValue);
    void GetValuesForAdress(std::string aMessage, std::vector< std::vector<float> > &aValue);
    
    bool * getSendOSCBool() {return &mSendOSC;};
    bool IsAdressInContainer(std::string aAdress);
    
    // Fonctions de crÈation et destruction du singleton
    static OSCManager * getInstance ();
    static void kill ();
    static OSCManager* _OSCManagersingleton;
    
    ci::osc::Sender sender;
	ci::osc::Listener listener;
	std::string host;
    std::string mMessageAddress;
	int port;
    bool mSendOSC;
	
    std::map<std::string, std::vector< std::vector<float> > > mMessageContainer;
    
private:
    OSCManager();
};

#endif
