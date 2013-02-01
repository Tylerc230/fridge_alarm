#include <Arduino.h>
#include "Metro.h"
#define kResistorReadPin 0
#define kResistorThreshold 250
#define kOpenTimeout (30 * 1000)
typedef enum {
    OpenState,
    CloseState,
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

State currentState = CloseState;
Metro *timer = NULL;

void step()
{
    State newState = getCurrentState();
    handleStateChange(newState, currentState);
    delay(100);
}

State getCurrentState()
{
    if(timer != NULL && timer->check())
    {
        return AlarmState;

    }
    else if(analogRead(kResistorReadPin) > kResistorThreshold )
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
    timer = new Metro(kOpenTimeout);
}

void stopTimer()
{
    delete timer;
    timer = NULL;
}

void soundAlarm()
{
}

void silenceAlarm()
{
}
