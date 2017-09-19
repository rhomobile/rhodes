#ifndef NOTIFICATIONSOUND_H
#define NOTIFICATIONSOUND_H
#include "../../platform/shared/common/RhoStd.h"
#include "../iexecutable.h"

class NotificationSound : public IExecutable
{
    Q_OBJECT
private:
    int sFrequency = 0;
    int sDuration = 0;
    int sVolume = 0;
public:
    NotificationSound(int frequency = 0, int duration = 0, int volume = 0);
    void playSound();
    static void beep();
    static void beep(const rho::Hashtable<rho::String, rho::String>& propertyMap);
public slots:
    virtual void execute(){
        playSound();
        deleteLater();
    }
};

#endif // NOTIFICATIONSOUND_H
