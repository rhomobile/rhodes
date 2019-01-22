#include "../../../shared/generated/cpp/KeyboardBase.h"
#include <windows.h>
#include <shellapi.h>
#include <WinUser.h>


namespace rho {

using namespace apiGenerator;

class CKeyboardImpl: public CKeyboardBase
{
public:
    CKeyboardImpl(const rho::String& strID): CKeyboardBase()
    {
    }

};

class CKeyboardSingleton: public CKeyboardSingletonBase
{
    ~CKeyboardSingleton(){}
    virtual rho::String getInitialDefaultID();
    virtual void enumerate(CMethodResult& oResult);
    virtual void setOpenKeyboard( bool isOpen, rho::apiGenerator::CMethodResult& oResult){
        Hashtable<String, String> hashRes;
        bool isOk = true;
#ifdef OS_WINDOWS_DESKTOP
        if (isOpen){
            void *was;
            Wow64DisableWow64FsRedirection (&was);
            HINSTANCE keyboardProcess = ShellExecuteA (NULL, "open", "osk.exe", NULL, NULL, SW_SHOWDEFAULT);
            Wow64RevertWow64FsRedirection (was);
            if (!keyboardProcess){
                isOk = false;
            }

        }else{
            HANDLE hWnd = FindWindowW (L"OSKMainClass", NULL);
            if (hWnd) {
                PostMessage((HWND)hWnd, WM_SYSCOMMAND, SC_CLOSE, 0);
            }else{
                isOk = false;
            }
            /*void *was;
            Wow64DisableWow64FsRedirection (&was);
            ShellExecuteA(NULL, "open", "tskill", "osk", NULL, SW_HIDE);

            Wow64RevertWow64FsRedirection (was);*/

        }
#endif
        if (isOk) {hashRes["status"] = "ok";}
        else {hashRes["status"] = "error";}
        oResult.set(hashRes);
    }
};

class CKeyboardFactory: public CKeyboardFactoryBase
{
    ~CKeyboardFactory(){}
    virtual IKeyboardSingleton* createModuleSingleton();
    virtual IKeyboard* createModuleByID(const rho::String& strID);
};

extern "C" void Init_Keyboard_extension()
{
    CKeyboardFactory::setInstance( new CKeyboardFactory() );
    Init_Keyboard_API();
}

IKeyboard* CKeyboardFactory::createModuleByID(const rho::String& strID)
{
    return new CKeyboardImpl(strID);
}

IKeyboardSingleton* CKeyboardFactory::createModuleSingleton()
{
    return new CKeyboardSingleton();
}

void CKeyboardSingleton::enumerate(CMethodResult& oResult)
{
    rho::Vector<rho::String> arIDs;
    arIDs.addElement("SC1");
    arIDs.addElement("SC2");

    oResult.set(arIDs);
}

rho::String CKeyboardSingleton::getInitialDefaultID()
{
    CMethodResult oRes;
    enumerate(oRes);

    rho::Vector<rho::String>& arIDs = oRes.getStringArray();
        
    return arIDs[0];
}

}
