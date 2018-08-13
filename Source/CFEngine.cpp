/*
==============================================================================

Engine.cpp
Created: 2 Apr 2016 11:03:21am
Author:  Martin Hermant

==============================================================================
*/
#include "CFEngine.h"
#include "DroneManager.h"
#include "ControllerManager.h"
#include "NodeManager.h"
#include "CFAssetManager.h"

#include "CFDroneManager.h"
#include "CFRadioManager.h"
#include "CFParam.h"
#include "CFLog.h"

CFEngine::CFEngine() :
	Engine("LaMoucheFolle", ".mouche")
{
	//init here
	Engine::mainEngine = this;
	
	CFRadioManager::getInstance();

	addChildControllableContainer(CFDroneManager::getInstance());
	addChildControllableContainer(ControllerManager::getInstance());
	addChildControllableContainer(NodeManager::getInstance());
}

CFEngine::~CFEngine()
{
	//Application-end cleanup, nothing should be recreated after this

	//delete singletons here
	CFRadioManager::deleteInstance();
	CFDroneManager::deleteInstance(); 
	CFParamToc::tocs.clear();
	CFLogToc::tocs.clear();

	ControllerManager::deleteInstance();
	CFDroneManager::deleteInstance();
	NodeManager::deleteInstance();
	CFAssetManager::deleteInstance();
	
}

void CFEngine::clearInternal()
{
	//clear
	ControllerManager::getInstance()->clear();
	CFDroneManager::getInstance()->clear();
	NodeManager::getInstance()->clear();
}

var CFEngine::getJSONData()
{
	var data = Engine::getJSONData();

	//save here
	data.getDynamicObject()->setProperty("droneManager", CFDroneManager::getInstance()->getJSONData());
	data.getDynamicObject()->setProperty("controllerManager", ControllerManager::getInstance()->getJSONData());
	data.getDynamicObject()->setProperty("nodeManager", NodeManager::getInstance()->getJSONData());

	return data;
}

void CFEngine::loadJSONDataInternalEngine(var data, ProgressTask * loadingTask)
{
	
	//load here
	ProgressTask * droneTask = loadingTask->addTask("Drones");
	droneTask->start();
	CFDroneManager::getInstance()->loadJSONData(data.getProperty("droneManager", var()));
	droneTask->setProgress(1);
	droneTask->end();

	ProgressTask * controllerTask = loadingTask->addTask("Controllers");
	controllerTask->start();
	ControllerManager::getInstance()->loadJSONData(data.getProperty("controllerManager", var()));
	controllerTask->setProgress(1);
	controllerTask->end();

	ProgressTask * nodeTask = loadingTask->addTask("Nodes");
	nodeTask->start();
	NodeManager::getInstance()->loadJSONData(data.getProperty("nodeManager", var()));
	nodeTask->setProgress(1);
	nodeTask->end();

	ControllerManager::getInstance()->sendFullSetup();

}


String CFEngine::getMinimumRequiredFileVersion()
{
	return "1.0.0";
}
