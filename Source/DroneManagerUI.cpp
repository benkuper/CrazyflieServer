/*
  ==============================================================================

    DroneManagerUI.cpp
    Created: 24 Oct 2017 11:19:35am
    Author:  Ben

  ==============================================================================
*/

#include "DroneManagerUI.h"

DroneManagerUI::DroneManagerUI(const String & name, DroneManager * manager) :
	BaseManagerShapeShifterUI(name, manager)
{
	disableAllBT = manager->disableAllTrigger->createButtonUI();
	connectAllBT = manager->connectAllTrigger->createButtonUI();
	connectAllNCBT = manager->connectAllNotConnectedTrigger->createButtonUI();
	resetKalmanBT = manager->resetAllKalman->createButtonUI();

	addAndMakeVisible(connectAllBT);
	addAndMakeVisible(connectAllNCBT);
	addAndMakeVisible(resetKalmanBT);
	addAndMakeVisible(disableAllBT);
	addExistingItems();
}

DroneManagerUI::~DroneManagerUI()
{
}

void DroneManagerUI::resizedInternalHeader(Rectangle<int>& r)
{
	Rectangle<int> h = r.removeFromTop(20).reduced(2); 
	disableAllBT->setBounds(h.removeFromLeft(70));
	h.removeFromLeft(10);
	connectAllBT->setBounds(h.removeFromLeft(70));
	h.removeFromLeft(10);
	connectAllNCBT->setBounds(h.removeFromLeft(130));
	h.removeFromLeft(10);
	resetKalmanBT->setBounds(h.removeFromLeft(100));
}
