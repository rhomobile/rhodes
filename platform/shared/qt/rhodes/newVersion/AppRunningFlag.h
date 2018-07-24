#ifndef APPRUNNINGFLAG_H
#define APPRUNNINGFLAG_H

#include <QString>
#include <Windows.h>

class AppRunningFlag
{
private:
    HANDLE hMutex;
public:
    AppRunningFlag(QString appName){
        HANDLE hMutex = ::CreateMutex(NULL, TRUE, appName.toStdWString().c_str());
    }
    ~AppRunningFlag(){
        ::CloseHandle(hMutex);
    }


};

#endif // APPRUNNINGFLAG_H
