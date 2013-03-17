#include <Arduino.h>
#define kResistorReadPin 1
#define kAlarmWritePin 3
#define kDebugLightPin 5
#define kResistorThreshold 250
#define kOpenTimeout (5UL * 60UL * 1000UL)
//#define kOpenTimeout (5UL * 1000UL)
#define kAlarmOff 0
#define kAlarmFreq 2.f
#define k2_PI 6.28
#define log(x) //Serial.println(x)
typedef enum {
    CloseState,
    OpenState,
    AlarmState
}State;
State getCurrentState();
void handleStateChange(State newState, State oldState);
void fromOpenTo(State newState);
void fromCloseTo(State newState);
void fromAlarmTo(State newState);
void closeOpenTransition();
void openCloseTransition();
void openAlarmTransition();
void alarmOpenTransition();
void alarmCloseTransition();
void closeAlarmTransition();
void silenceAlarm();
void soundAlarm();
void stopTimer();
void startTimer();
bool alarmTimerPopped();
bool doorOpen();
void modulateAlarm();
void update(State state);

State currentState = CloseState;
unsigned long alarmSoundStartTime = kAlarmOff;
bool alarmOn = false;

void step()
{
    State newState = getCurrentState();
    handleStateChange(newState, currentState);
    currentState = newState;
    update(currentState);
    delay(100);
}

State getCurrentState()
{
    bool open = doorOpen();
    bool popped = alarmTimerPopped();
    if(open && popped)
    {
        return AlarmState;

    }
    else if(open)
    {
        return OpenState;
    }
    else {
        return CloseState;
    }
}

void handleStateChange(State newState, State oldState)
{
    if (newState == oldState)
        return;
    char buf [100];
    sprintf(buf, "old state %d, new state %d", oldState, newState);
    log(buf);
    switch (oldState)
    {
        case OpenState:
            fromOpenTo(newState);
            break;
        case CloseState:
            fromCloseTo(newState);
            break;
        case AlarmState:
            fromAlarmTo(newState);
            break;
    }
}

void update(State state)
{
    switch (state)
    {
        case  CloseState:
            analogWrite(kDebugLightPin, 0);
            break;
        case OpenState:
            analogWrite(kDebugLightPin, 128);
            break;
        case AlarmState:
            analogWrite(kDebugLightPin, 255);
            break;
    }
    modulateAlarm();
}

void fromOpenTo(State newState)
{
    switch (newState)
    {
        case CloseState:
            openCloseTransition();
            break;
        case AlarmState:
            openAlarmTransition();
            break;
    }

}

void fromCloseTo(State newState)
{
    switch (newState)
    {
        case OpenState:
            closeOpenTransition();
            break;
        case AlarmState:
            closeAlarmTransition();
            break;
    }

}

void fromAlarmTo(State newState)
{
    switch (newState)
    {
        case OpenState:
            alarmOpenTransition();
            break;
        case CloseState:
            alarmCloseTransition();
            break;
    }

}

void closeOpenTransition()
{
    //Start a new timer since the fridge is open
    startTimer();
}

void openCloseTransition()
{
    //Invalidate the timer since the fridge was closed
    stopTimer();
}

void openAlarmTransition()
{
    soundAlarm();
}

void alarmOpenTransition()
{
//shouldn't happen
}

void alarmCloseTransition()
{
    stopTimer();
    silenceAlarm();
}

void closeAlarmTransition()
{
//shouldn't happen
}

void startTimer()
{
    log("start timer");
    alarmSoundStartTime = millis() + kOpenTimeout;
}

void stopTimer()
{
    log("stop timer");
    alarmSoundStartTime = kAlarmOff;
}

bool alarmTimerPopped()
{
    if (alarmSoundStartTime != kAlarmOff)
    {
        if (millis() > alarmSoundStartTime)
        {
            return 1;
        }
    }
    return 0;
}

bool doorOpen()
{
    return analogRead(kResistorReadPin) > kResistorThreshold;
}

void soundAlarm()
{
    log("alarm on");
    alarmOn = true;
}

void silenceAlarm()
{
    log("alarm off");
    alarmOn = false;
}

void modulateAlarm()
{
    static float phase = 0.f;
    int dutyCycle = 0;
    if (alarmOn)
    {
        //sin wave from 0 - 1
        float coeff = (sin(phase) + 1)/2;
        dutyCycle = coeff * 255;
        phase += k2_PI * .1f;

    }else {
        dutyCycle = 0;

    }
    analogWrite(kAlarmWritePin, dutyCycle);
}
