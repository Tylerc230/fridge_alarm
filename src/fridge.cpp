#include <Arduino.h>
#define kResistorReadPin 0
#define kResistorThreshold 250
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

State currentState = CloseState;

void step()
{
    State newState = getCurrentState();
    handleStateChange(newState, currentState);
    delay(100);
}

State getCurrentState()
{
    if(analogRead(kResistorReadPin) > kResistorThreshold )
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

}

void fromCloseTo(State newState)
{

}

void fromAlarmTo(State newState)
{

}

void closeOpenTransition()
{

}

void openCloseTransition()
{

}

void openAlarmTransition()
{

}

void alarmOpenTransition()
{
//shouldn't happen
}

void alarmCloseTransition()
{

}

void closeAlarmTransition()
{
//shouldn't happen
}


