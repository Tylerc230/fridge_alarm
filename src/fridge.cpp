#include <Arduino.h>
#define kResistorReadPin 1
#define kResistorThreshold 250
#define kOpenTimeout (5 * 1000)
#define kAlarmOff 0
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

State currentState = CloseState;
long long alarmSoundStartTime = kAlarmOff;

void step()
{
    State newState = getCurrentState();
    handleStateChange(newState, currentState);
    currentState = newState;
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
    //char buf [100];
    //sprintf(buf, "old state %d, new state %d", oldState, newState);
    //Serial.println(buf);
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
    //Serial.println("start timer");
    alarmSoundStartTime = millis() + kOpenTimeout;
}

void stopTimer()
{
    //Serial.println("stop timer");
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
}

void silenceAlarm()
{
}
