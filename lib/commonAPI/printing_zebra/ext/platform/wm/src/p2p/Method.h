// P2PClient.cpp : Defines the entry point for the console application.
//

#include "P2PMessageQueue.h"

#include <common/RhoStd.h>


namespace rho
{
namespace printing
{
namespace ipc
{
namespace message
{

class ServerParameterConstants
{
public:
    static const char* methodName;
    static const char* hashTable;
    static const char* rhoString;
    static const char* rhoString2;
    static const char* rhoInt;
    static const char* rhoInt2;
    static const char* arrayType;
    static const char* printerIdType;
};

ipc::Message* constructIpcMessage(const rho::String& id, const rho::String& funcName);

ipc::Message* constructIpcMessage(const rho::String& id, const rho::String& funcName, const rho::String& strParam, const rho::Hashtable<rho::String, rho::String>& hash);

ipc::Message* constructIpcMessage(const rho::String& id, const rho::String& funcName, const rho::Hashtable<rho::String, rho::String>& hash);

ipc::Message* constructIpcMessage(const rho::String& id, const rho::String& funcName, const rho::String& string);

ipc::Message* constructIpcMessage(const rho::String& id, const rho::String& funcName, const int integer);

ipc::Message* constructIpcMessage(const rho::String& id, const rho::String& funcName, const rho::Vector<rho::String>& array);

ipc::Message* constructIpcMessage(const rho::String& id, const rho::String& funcName, const rho::String& string, const rho::Vector<rho::String>& array);

ipc::Message* constructIpcMessage(const rho::String& id, const rho::String& funcName, const rho::String& strParam, int intParam1, int intParam2, const rho::Hashtable<rho::String, rho::String>& hash);

ipc::Message* constructIpcMessage(const rho::String& id, const rho::String& funcName, const rho::String& strParam, const rho::String& strParam2, int intParam1, int intParam2);

} // message
} // ipc
} // printing
} // rho