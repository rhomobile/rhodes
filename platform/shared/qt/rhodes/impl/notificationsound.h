#ifndef NOTIFICATIONSOUND_H
#define NOTIFICATIONSOUND_H
#include "../../platform/shared/common/RhoStd.h"

#ifndef RHODES_VERSION_LIBRARY
#include "../iexecutable.h"
#endif

class NotificationSound
#ifndef RHODES_VERSION_LIBRARY
        : public IExecutable
#endif
{
#ifndef RHODES_VERSION_LIBRARY
    Q_OBJECT
#endif
private:
    int sFrequency = 0;
    int sDuration = 0;
    int sVolume = 0;
public:
    NotificationSound(int frequency = 0, int duration = 0, int volume = 0);
    void playSound();
    static void beep();
    static void beep(const rho::Hashtable<rho::String, rho::String>& propertyMap);
#ifndef RHODES_VERSION_LIBRARY
public slots:
    virtual void execute(){
        playSound();
        deleteLater();
    }
#endif
};

#endif // NOTIFICATIONSOUND_H
