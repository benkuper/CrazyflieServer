/*
  ==============================================================================

    Drone.h
    Created: 19 Oct 2017 7:33:14pm
    Author:  Ben

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"
#include "Crazyflie.h"
#

class Drone :
	public BaseItem,
	public Thread //thread the cf init to avoid blocking UI
{
public:
	Drone();
	~Drone();

	enum DroneState {DISCONNECTED, CONNECTING, READY , ERROR };
		
	IntParameter * targetRadio;
	IntParameter * channel;
	EnumParameter * speed;
	StringParameter * address;

	EnumParameter * droneState;

	Trigger * connectTrigger;
	Trigger * logParams;
	Trigger * logLogs;
	Trigger * taskDump;
	Trigger * resetKalmanTrigger;

	EnumParameter * lightMode;
	ColorParameter * color;
	BoolParameter * headlight;

	BoolParameter * autoConnect;
	BoolParameter * autoReboot;

	Point3DParameter * targetPosition;
	Point3DParameter * realPosition;

	FloatParameter * linkQuality;
	FloatParameter * battery;

	ScopedPointer<Crazyflie> cf;

	Trigger * inTrigger;
	Trigger * outTrigger;

	String consoleBuffer;

	uint32 lastAckTime;
	uint32 ackTimeout;

	//Setup chronology
	bool droneHasStarted;
	bool droneHasFinishedInit;

	//log blocks
	struct dataLog
	{
		float battery;
		float x;
		float y;
		float z;
	};

	ScopedPointer<LogBlock<dataLog>> dataLogBlock;

	void launchCFThread();
	void stopCFThread();

	void onContainerParameterChangedInternal(Parameter * p) override;
	void onContainerTriggerTriggered(Trigger * t) override;

	bool setupCF();
	
	template<class T>
	bool setParam(String group, String paramID, T value);
	bool setTargetPosition(float x, float y, float z, bool showTrigger = true);
	bool setAnchors(Array<Point3DParameter *> positions);

	void logAllParams();
	void logAllLogs();

	void consoleCallback(const char * c);
	void emptyAckCallback(const crtpPlatformRSSIAck * a);
	void linkQualityCallback(float val);
	void dataLogCallback(uint32_t /*time*/, dataLog * data);

	virtual void run() override;

	String getRadioString() const { return "radio://" + String(targetRadio->intValue()) + "/" + String(channel->intValue()) + "/" + speed->getValueData().toString() + "/" + address->stringValue(); }
	String getTypeString() const override { return "Drone"; }

	void loadJSONDataInternal(var data) override;

private:
	SpinLock cfLock;
};