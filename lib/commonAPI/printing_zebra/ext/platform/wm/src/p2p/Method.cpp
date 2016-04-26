// P2PClient.cpp : Defines the entry point for the console application.
//

#include "ThreadMessageQueue.h"
#include "method.h"

extern "C" 
{
#include <json/json_object.h>
}

namespace rho
{
namespace printing
{
namespace ipc
{
namespace message
{

const char* ServerParameterConstants::methodName = "Name";
const char* ServerParameterConstants::hashTable = "Dictinary";
const char* ServerParameterConstants::rhoString = "rhoString";
const char* ServerParameterConstants::rhoString2 = "rhoString2";
const char* ServerParameterConstants::rhoInt = "rhoInt";
const char* ServerParameterConstants::rhoInt2= "rhoInt2";
const char* ServerParameterConstants::arrayType = "List";
const char* ServerParameterConstants::printerIdType = "PrinterID";

json_object* constructParameter(const rho::Hashtable<rho::String, rho::String>& hash)
{
    rho::Hashtable<rho::String, rho::String>::const_iterator it = hash.begin();
    rho::Hashtable<rho::String, rho::String>::const_iterator eit = hash.end();

    json_object* val = json_object_new_object();
    for (;it != eit; ++it)
    {       
        json_object_object_add(val, const_cast<char*>(it->first.c_str()), 
            json_object_new_string(const_cast<char*>(it->second.c_str()))); 
    }

    return val;
}

ipc::Message* constructIpcMessage(const rho::String& id, const rho::String& funcName, const rho::Hashtable<rho::String, rho::String>& hash)
{
    json_object* obj      = json_object_new_object();
    json_object* jsonHash = constructParameter(hash);

    json_object_object_add(obj, 
        const_cast<char*>(ServerParameterConstants::methodName), 
        json_object_new_string(const_cast<char*>(funcName.c_str())));

    json_object_object_add(obj, 
        const_cast<char*>(ServerParameterConstants::printerIdType), 
        json_object_new_string(const_cast<char*>(id.c_str())));

    json_object_object_add(obj, 
        const_cast<char*>(ServerParameterConstants::hashTable), 
        jsonHash);

    ipc::Message* message = new ipc::Message(json_object_to_json_string(obj), false);

    return message;
}

ipc::Message* constructIpcMessage(const rho::String& id, const rho::String& funcName, const rho::String& strParam, const rho::Hashtable<rho::String, rho::String>& hash)
{
    json_object* obj      = json_object_new_object();
    json_object* jsonHash = constructParameter(hash);

    json_object_object_add(obj, 
        const_cast<char*>(ServerParameterConstants::methodName), 
        json_object_new_string(const_cast<char*>(funcName.c_str())));

    json_object_object_add(obj, 
        const_cast<char*>(ServerParameterConstants::printerIdType), 
        json_object_new_string(const_cast<char*>(id.c_str())));

    json_object_object_add(obj, 
        const_cast<char*>(ServerParameterConstants::rhoString), 
        json_object_new_string(const_cast<char*>(strParam.c_str())));

    json_object_object_add(obj, 
        const_cast<char*>(ServerParameterConstants::hashTable), 
        jsonHash);

    ipc::Message* message = new ipc::Message(json_object_to_json_string(obj), false);

    return message;
}

ipc::Message* constructIpcMessage(const rho::String& id, const rho::String& funcName)
{
    json_object* obj = json_object_new_object();

    json_object_object_add(obj, 
        const_cast<char*>(ServerParameterConstants::methodName), 
        json_object_new_string(const_cast<char*>(funcName.c_str())));

    json_object_object_add(obj, 
        const_cast<char*>(ServerParameterConstants::printerIdType), 
        json_object_new_string(const_cast<char*>(id.c_str())));

    ipc::Message* message = new ipc::Message(json_object_to_json_string(obj), false);

    return message;
}

ipc::Message* constructIpcMessage(const rho::String& id, const rho::String& funcName, const rho::String& string)
{
    json_object* obj = json_object_new_object();

    json_object_object_add(obj, 
        const_cast<char*>(ServerParameterConstants::methodName), 
        json_object_new_string(const_cast<char*>(funcName.c_str())));

    json_object_object_add(obj, 
        const_cast<char*>(ServerParameterConstants::printerIdType), 
        json_object_new_string(const_cast<char*>(id.c_str())));

    json_object_object_add(obj, 
        const_cast<char*>(ServerParameterConstants::rhoString), 
        json_object_new_string(const_cast<char*>(string.c_str())));

    ipc::Message* message = new ipc::Message(json_object_to_json_string(obj), false);

    return message;
}

ipc::Message* constructIpcMessage(const rho::String& id, const rho::String& funcName, const int integer)
{
    json_object* obj = json_object_new_object();

    json_object_object_add(obj, 
        const_cast<char*>(ServerParameterConstants::methodName), 
        json_object_new_string(const_cast<char*>(funcName.c_str())));

    json_object_object_add(obj, 
        const_cast<char*>(ServerParameterConstants::printerIdType), 
        json_object_new_string(const_cast<char*>(id.c_str())));

    json_object_object_add(obj, 
        const_cast<char*>(ServerParameterConstants::rhoInt), 
        json_object_new_int(integer));

    ipc::Message* message = new ipc::Message(json_object_to_json_string(obj), false);

    return message;
}

ipc::Message* constructIpcMessage(const rho::String& id, const rho::String& funcName, const rho::String& string, const rho::Vector<rho::String>& array)
{
    json_object* obj = json_object_new_object();

    json_object_object_add(obj, 
        const_cast<char*>(ServerParameterConstants::methodName), 
        json_object_new_string(const_cast<char*>(funcName.c_str())));

    json_object_object_add(obj, 
        const_cast<char*>(ServerParameterConstants::printerIdType), 
        json_object_new_string(const_cast<char*>(id.c_str())));

    json_object_object_add(obj, 
        const_cast<char*>(ServerParameterConstants::rhoString), 
        json_object_new_string(const_cast<char*>(string.c_str())));

    json_object* jsonArray = json_object_new_array();

    rho::Vector<rho::String>::const_iterator cit = array.begin();
    rho::Vector<rho::String>::const_iterator eit = array.end();

    for (; cit != eit; ++cit)
    {
        json_object_array_add(jsonArray, 
            json_object_new_string(const_cast<char*>(cit->c_str())));
    }

    json_object_object_add(obj, 
        const_cast<char*>(ServerParameterConstants::arrayType), jsonArray);

    ipc::Message* message = new ipc::Message(json_object_to_json_string(obj), false);

    return message;
}

ipc::Message* constructIpcMessage(const rho::String& id, const rho::String& funcName, const rho::Vector<rho::String>& array)
{
    json_object* obj = json_object_new_object();

    json_object_object_add(obj, 
        const_cast<char*>(ServerParameterConstants::methodName), 
        json_object_new_string(const_cast<char*>(funcName.c_str())));

    json_object_object_add(obj, 
        const_cast<char*>(ServerParameterConstants::printerIdType), 
        json_object_new_string(const_cast<char*>(id.c_str())));

    json_object* jsonArray = json_object_new_array();

    rho::Vector<rho::String>::const_iterator cit = array.begin();
    rho::Vector<rho::String>::const_iterator eit = array.end();

    for (; cit != eit; ++cit)
    {
        json_object_array_add(jsonArray, 
            json_object_new_string(const_cast<char*>(cit->c_str())));
    }

    json_object_object_add(obj, 
        const_cast<char*>(ServerParameterConstants::arrayType), jsonArray);

    ipc::Message* message = new ipc::Message(json_object_to_json_string(obj), false);

    return message;
}

ipc::Message* constructIpcMessage(const rho::String& id, const rho::String& funcName, const rho::String& strParam, int intParam1, int intParam2, const rho::Hashtable<rho::String, rho::String>& hash)
{
    json_object* obj      = json_object_new_object();
    json_object* jsonHash = constructParameter(hash);

    json_object_object_add(obj, 
        const_cast<char*>(ServerParameterConstants::methodName), 
        json_object_new_string(const_cast<char*>(funcName.c_str())));

    json_object_object_add(obj, 
        const_cast<char*>(ServerParameterConstants::printerIdType), 
        json_object_new_string(const_cast<char*>(id.c_str())));

    json_object_object_add(obj, 
        const_cast<char*>(ServerParameterConstants::rhoString), 
        json_object_new_string(const_cast<char*>(strParam.c_str())));

    json_object_object_add(obj, 
        const_cast<char*>(ServerParameterConstants::rhoInt), 
        json_object_new_int(intParam1));

    json_object_object_add(obj, 
        const_cast<char*>(ServerParameterConstants::rhoInt2), 
        json_object_new_int(intParam2));

    json_object_object_add(obj, 
        const_cast<char*>(ServerParameterConstants::hashTable), 
        jsonHash);

    ipc::Message* message = new ipc::Message(json_object_to_json_string(obj), false);

    return message;
}

ipc::Message* constructIpcMessage(const rho::String& id, const rho::String& funcName, const rho::String& strParam, const rho::String& strParam2, int intParam1, int intParam2)
{
    json_object* obj = json_object_new_object();

    json_object_object_add(obj, 
        const_cast<char*>(ServerParameterConstants::methodName), 
        json_object_new_string(const_cast<char*>(funcName.c_str())));

    json_object_object_add(obj, 
        const_cast<char*>(ServerParameterConstants::printerIdType), 
        json_object_new_string(const_cast<char*>(id.c_str())));

    json_object_object_add(obj, 
        const_cast<char*>(ServerParameterConstants::rhoString), 
        json_object_new_string(const_cast<char*>(strParam.c_str())));

    json_object_object_add(obj, 
        const_cast<char*>(ServerParameterConstants::rhoString2), 
        json_object_new_string(const_cast<char*>(strParam2.c_str())));

    json_object_object_add(obj, 
        const_cast<char*>(ServerParameterConstants::rhoInt), 
        json_object_new_int(intParam1));

    json_object_object_add(obj, 
        const_cast<char*>(ServerParameterConstants::rhoInt2), 
        json_object_new_int(intParam2));

    ipc::Message* message = new ipc::Message(json_object_to_json_string(obj), false);

    return message;
}

}
} // end of ipc
} // printing
} // rho