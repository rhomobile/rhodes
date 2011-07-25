/*------------------------------------------------------------------------
* (The MIT License)
* 
* Copyright (c) 2008-2011 Rhomobile, Inc.
* 
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
* 
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
* 
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
* THE SOFTWARE.
* 
* http://rhomobile.com
*------------------------------------------------------------------------*/

#include "../RhoLog.h"
#include "common/RhoConf.h"

void runTests();
extern "C" void testPlainLog();

void runAllLogTests() {
    runTests();

    LOGCONF().setEnabledCategories( "*" );

    testPlainLog();
}

void setup(){
    LOGCONF().setMinSeverity( 0 );
    LOGCONF().setLogToOutput(true);
    LOGCONF().setLogToFile(true);
    //LOGCONF().setLogFilePath("MyLog.txt");
    LOGCONF().setLogPrefix(true);
}

void shouldLogInfo(){
    LOG(INFO) + "Hex: " + LOGFMT("%X") + 10 + "; Dec: " + 10;
}

void shouldNotLogInfo(){
    LOGCONF().setMinSeverity( L_WARNING );

    LOG(INFO) + "ERROR: shouldNotLogInfo";

    LOGCONF().setMinSeverity( 0 );
}

void shouldNotLogPrefix(){
    LOGCONF().setLogPrefix(false);

    LOG(INFO) + "NO PREFIX: shouldNotLogPrefix";

    LOGCONF().setLogPrefix(true);
}

void shouldLogMsgCategory(){
    static rho::LogCategory cat_MsgClass = "MsgClass";

    LOGCONF().setEnabledCategories( "MsgClass" );

    LOGC(INFO,cat_MsgClass) + "Category name should be instead of file name and line";
}

void shouldNotLogMsgCategory(){
    static rho::LogCategory cat_Disabled( "Disabled" );

    LOGCONF().setEnabledCategories( "MsgClass" );

    LOGC(INFO,cat_Disabled) + "ERROR: Category disabled";
}

void shouldLogListOfCategories(){
    static rho::LogCategory cat_MsgClass1( "MsgClass1" );
    static rho::LogCategory cat_MsgClass2( "MsgClass2" );
    static rho::LogCategory cat_MsgClass3( "MsgClass3" );

    LOGCONF().setEnabledCategories( "MsgClass1,MsgClass2,MsgClass3" );

    LOGC(INFO,cat_MsgClass1) + "Log for MsgClass1";
    LOGC(INFO,cat_MsgClass2) + "Log for MsgClass2";
    LOGC(INFO,cat_MsgClass3) + "Log for MsgClass3";

    LOGCONF().setEnabledCategories( "MsgClass1,MsgClass3" );
    LOGC(INFO,cat_MsgClass2) + "ERROR : Log for MsgClass2 disabled";
}

void shouldLogAllOfCategories(){
    static rho::LogCategory cat_MsgClass1( "MsgClass11" );
    static rho::LogCategory cat_MsgClass2( "MsgClass22" );
    static rho::LogCategory cat_MsgClass3( "MsgClass33" );

    LOGCONF().setEnabledCategories( "*" );

    LOGC(INFO,cat_MsgClass1) + "Log for MsgClass11";
    LOGC(INFO,cat_MsgClass2) + "Log for MsgClass22";
    LOGC(INFO,cat_MsgClass3) + "Log for MsgClass33";

    LOGCONF().setDisabledCategories( "MsgClass33" );
    LOGC(INFO,cat_MsgClass3) + "ERROR: Disabled - Log for MsgClass33";

}

class CLogTest{
    DEFINE_LOGCLASS;
public:

    CLogTest(){
        LOG(INFO)+"Constructor called.";
    }
};
IMPLEMENT_LOGCLASS(CLogTest,"CLogTest");

void shouldLogClassCategory(){

    LOGCONF().setEnabledCategories( "CLogTest" );

    CLogTest oTest;

}

void shouldLogFileCycle(){

    LOGCONF().setMaxLogFileSize(20);

    LOG(INFO) + "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA";
    LOG(INFO) + "BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB";

    LOGCONF().setMaxLogFileSize(0);

}

void shouldSaveLogConfToFile(){
    LOGCONF().saveToFile();//"logconf2.txt");
}

void shouldLoadLogConfFromString(){
    rho::common::RhoSettings oRhoConf;

    oRhoConf.loadFromString(
        "MinSeverity  = 2  \n"
        "LogToOutput=1\r\n"
        "LogToFile=1\r\n"
        "LogFilePath=mylog.txt\r\n"
        "MaxLogFileSize=20\r\n"
        "LogPrefix=1\r\n"
        "LogCategories=MyClass1,MyClass2\r\n"
        "ExcludeLogCategories=MyClass3\r\n"
    );
    rho::LogSettings oConf;
    oConf.loadFromConf(oRhoConf);
    //oConf.loadFromFile("logconf.txt");

    if ( oConf.getMinSeverity() != L_WARNING )
        LOG(INFO) + "ERROR: oConf.getMinSeverity() != WARNING";

    if ( !oConf.isLogToOutput() )
        LOG(INFO) + "ERROR: !oConf.isLogToOutput()";

    if ( !oConf.isLogToFile() )
        LOG(INFO) + "ERROR: !oConf.isLogToFile()";

    if ( oConf.getLogFilePath().compare("mylog.txt") != 0  )
        LOG(INFO) + "ERROR: oConf.getLogFilePath().compare(\"mylog.txt\") != 0";

    if ( oConf.getMaxLogFileSize() != 20  )
        LOG(INFO) + "ERROR: oConf.getMaxLogFileSize() != 20";

    if ( !oConf.isLogPrefix()  )
        LOG(INFO) + "ERROR: !oConf.isLogPrefix()";

    if ( oConf.getEnabledCategories().compare("MyClass1,MyClass2") != 0  )
        LOG(INFO) + "ERROR: oConf.getEnabledCategories().compare(\"MyClass1,MyClass2\") != 0";

    if ( oConf.getDisabledCategories().compare("MyClass3") != 0  )
        LOG(INFO) + "ERROR: oConf.getDisabledCategories().compare(\"MyClass3\") != 0";

}

void shouldRaiseAssert(){
    RHO_ASSERT(0);
}

void shouldTrace(){
    LOG(TRACE) + "This is Debug trace";
    int  i = 1;
    TRACE_IF(i==1) + "Conditional trace";

    i = 0;
    TRACE_IF(i==1) + "ERROR: Conditional trace";
}

void runTests(){

    setup();

    shouldNotLogInfo();
    shouldLogInfo();
    shouldNotLogPrefix();
    shouldLogMsgCategory();
    shouldNotLogMsgCategory();
    shouldLogListOfCategories();

    shouldLogAllOfCategories();

    shouldLogClassCategory();

    shouldLogFileCycle();

    shouldLoadLogConfFromString();
    shouldSaveLogConfToFile();

    shouldTrace();
//    shouldRaiseAssert();

}
