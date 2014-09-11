//
//  Created by Dmitry Soldatenkov on 18.08.14.
//
//

#include <stdio.h>
#include "RhodesApp.h"
#include "common/RhoMutexLock.h"
#include "common/IRhoClassFactory.h"
#include "common/RhoFile.h"
#include "common/RhoConf.h"
#include "common/RhoFilePath.h"
#include "common/Tokenizer.h"
#import "logging/RhoLogConf.h"

#include "ruby/ext/rho/rhoruby.h"
#include <algorithm>


#include "net/INetRequest.h"

#include "LogAccess.h"

#include "../HTTPServer/HTTPServer.h"

void callback_get_log(void *arg, rho::String const &strQuery) {
    rho::String logPath = LOGCONF().getLogFilePath();
    
    //bool bOldSaveToFile = LOGCONF().isLogToFile();
    //LOGCONF().setLogToFile(false);

    // read file
    //rho::String logFileContent = "";
    //rho::common::CRhoFile::readStringFromFile(logPath.c_str(), logFileContent);
    //LOGCONF().setLogToFile(bOldSaveToFile);
    //logFileContent = "$$$$$$$$$$$$$$$>>>>>>>>>>>>>>>>>\n"+logFileContent+"\n<<<<<<<<<<<<<<<<<<<$$$$$$$$$$$$$$$$$$$$$";

    
    //char* logText = rho_logconf_getText();
    //rho_http_sendresponse(arg, logText);
    //rho_logconf_freeString(logText);
    
    
    
    bool bOldSaveToFile = LOGCONF().isLogToFile();
    LOGCONF().setLogToFile(false);
    
    rho::common::CRhoFile oFile;
    if ( oFile.open( LOGCONF().getLogFilePath().c_str(), rho::common::CRhoFile::OpenReadOnly) )
    {
        int nFileSize = oFile.size();
        int nPos = LOGCONF().getLogTextPos();
        int nMaxSize = nFileSize > nPos ? nFileSize : nPos;
        //if ( limit <= 0 || limit > nMaxSize)
        int limit = nMaxSize;
        
        //res = rho_ruby_create_string_withlen(limit);
        char* szStr = new char[limit+1];
        
        if ( limit <= nPos )
        {
            oFile.setPosTo(nPos-limit);
            oFile.readData(szStr,0,limit);
        }else
        {
            oFile.setPosTo(nFileSize-(limit-nPos));
            int nRead = oFile.readData(szStr,0,limit);
            
            oFile.setPosTo(0);
            oFile.readData(szStr,nRead,limit-nRead);
        }
        szStr[limit] = 0;
        rho_http_sendresponse(arg, szStr);
        delete[] szStr;
        
    }
    else {
        rho_http_sendresponse(arg, "ERROR: can not read log file !");
    }
    
    LOGCONF().setLogToFile(bOldSaveToFile);
    
    
    
}

void Init_LogAccess() {
    DevHTTPServer * ds = DevHTTPServer::getInstance();
    ds->getHTTPServer()->register_uri("/development/get_log", callback_get_log);
}

