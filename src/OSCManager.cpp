/*
 *  OSCSender.cpp
 *  SurfaceBasic
 *
 *  Created by Baptiste Bohelay on 12-03-23.
 *  Copyright 2012 Tangible Interaction Design Inc. All rights reserved.
 *
 */

#include "OSCManager.h"
#include "Tools.h"
//#include "ThreeSixtyInteractiveApp.h"

using namespace std;
using namespace ci;
using namespace ci::app;

OSCManager *OSCManager::_OSCManagersingleton = NULL;

// Fonctions de crÈation et destruction du singleton
OSCManager * OSCManager::getInstance ()
{
    if (NULL == _OSCManagersingleton)
    {
        _OSCManagersingleton =  new OSCManager;
        _OSCManagersingleton->setup();
    }
    
    return _OSCManagersingleton;
}

void OSCManager::kill ()
{
    if (NULL != _OSCManagersingleton)
    {
        delete _OSCManagersingleton;
        _OSCManagersingleton = NULL;
    }
}

OSCManager::OSCManager()
{
//    setup();
}

void OSCManager::setup()
{
    host = "localhost";//ThreeSixtyInteractiveApp::getIp();
	port = 1315;
//    mMessageAddress = ThreeSixtyInteractiveApp::isSecondaryComputer() ? "/second" : "";
	sender.setup(host, port);
    
//	listener.setup(1314);
}

void OSCManager::GetValueForAdress(string aMessage, int &aValue)
{
	map<string, vector< vector<float> > >::iterator it =  mMessageContainer.find(aMessage);
	if(it == mMessageContainer.end())
	{
		return;
	}
	else
	{
		vector< vector<float> > lReturn = it->second;
		mMessageContainer.erase(it);
        aValue = lReturn[0][0];
		return;
	}
}

void OSCManager::GetValueForAdress(string aMessage, float &aValue)
{    
	map<string, vector< vector<float> > >::iterator it =  mMessageContainer.find(aMessage);
	if(it == mMessageContainer.end())
	{
		return;
	}
	else
	{
		vector< vector<float> > lReturn = it->second;
		mMessageContainer.erase(it);
        aValue = lReturn[0][0];
		return;
	}
}

void OSCManager::GetValueForAdress(string aMessage, bool &aValue)
{
	map<string, vector< vector<float> > >::iterator it =  mMessageContainer.find(aMessage);
	if(it == mMessageContainer.end())
	{
		return;
	}
	else
	{
		vector< vector<float> > lReturn = it->second;
		mMessageContainer.erase(it);
        aValue = (lReturn[0][0] > 0.001f);
		return;
	}
}

void OSCManager::GetValuesForAdress(string aMessage, vector< vector<float> > &aValue)
{
	map<string, vector< vector<float> > >::iterator it =  mMessageContainer.find(aMessage);
	if(it == mMessageContainer.end())
	{
		return;
	}
	else
	{
		aValue = it->second;
		mMessageContainer.erase(it);
		return;
	}
}

void OSCManager::update()
{
	Listen();
}

void OSCManager::Listen()
{
	while (listener.hasWaitingMessages()) 
	{
		osc::Message message;
		listener.getNextMessage(&message);
		
		vector<float> lContainer;
		for (int i = 0; i < message.getNumArgs(); i++) 
		{
			if (message.getArgType(i) == osc::TYPE_INT32)
			{
				try 
				{
					lContainer.push_back((float)message.getArgAsInt32(i));
				}
				catch (...) 
				{
					console() << "Exception reading argument as int32" << std::endl;
				}
				
			}else if (message.getArgType(i) == osc::TYPE_FLOAT)
			{
				try 
				{
					lContainer.push_back(message.getArgAsFloat(i));
				}
				catch (...) 
				{
					console() << "Exception reading argument as float" << std::endl;
				}
			}
			else if (message.getArgType(i) == osc::TYPE_STRING)
			{
				console() << "OSC: Receive a string argument. Don't know what to do out of that" << std::endl;
			}
		}
        
        string lAdress = message.getAddress();
//        console() << "Received at" << lAdress << endl;
//        for(int i = 0; i < lContainer.size(); i++)
//        {
//            console() << lContainer[i] << endl;
//        }
        
//        if(Tools::sStackOSC)
//        {
            if(IsAdressInContainer(lAdress))
            {
                mMessageContainer[lAdress].push_back(lContainer);
            }
            else
            {
                vector< vector<float> > lvector;
                lvector.push_back(lContainer);
                mMessageContainer[lAdress] = lvector;
            }
//        }
//        else
//        {
//            vector< vector<float> > lvector;
//            lvector.push_back(lContainer);
//            mMessageContainer[lAdress] = lvector;
//        }
	}
}

bool OSCManager::IsAdressInContainer(string aAdress)
{
    map<string, vector< vector<float> > >::const_iterator it = mMessageContainer.find(aAdress);
    return it!=mMessageContainer.end();
}

void OSCManager::SendNote(int aId, int aX, int aY)
{
	osc::Message message;
	message.addIntArg(aX);
    message.addIntArg(aY);
	message.setAddress("/" + to_string(aId));
	message.setRemoteEndpoint(host, port);
	sender.sendMessage(message);
}

void OSCManager::SendOSC(string aAdress, int aValue)
{
	osc::Message message;
	message.addIntArg(aValue);
	stringstream lTemp;
	lTemp << mMessageAddress << aAdress;
	message.setAddress(lTemp.str());
	message.setRemoteEndpoint(host, port);
	sender.sendMessage(message);
}

void OSCManager::SendOSC(string aAdress, float aValue)
{
	osc::Message message;
	message.addFloatArg(aValue);
	stringstream lTemp;
	lTemp << mMessageAddress << aAdress;
	message.setAddress(lTemp.str());
	message.setRemoteEndpoint(host, port);
	sender.sendMessage(message);
}
