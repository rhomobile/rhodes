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

#include <gtest/gtest.h>

#include "hippomocks/hippomocks.h"

#include "sync/SyncEngine.h"
#include "db/DBAdapter.h"
#include "db/DBResult.h"
#include "common/IRhoClassFactory.h"

#include "NetDataStub.h"

using namespace rho;
using namespace rho::sync;
using namespace rho::db;
using namespace rho::net;
using namespace rho::common;

template <>
struct printArg<DBResultPtr&>
{
  static inline void print(std::ostream &os, DBResultPtr& arg, bool withComma)
  {
    if (withComma) os << ",";
    os << arg.operator ->();
  }
};

//bool operator==(const CDBResult& , const CDBResult& ){ return true;}
template<> 
struct comparer<const char*>
{
  static inline bool compare(const char* sz1, const char* sz2)
  {
    if ( sz1 == sz2 )
        return true;

	return sz1 && sz2 && strcmp(sz1,sz2) == 0 ;
  }
};
/*bool operator==(const char* sz1, const char* sz2)
{
    if ( sz1 == sz2 )
        return true;

	return sz1 && sz2 && strcmp(sz1,sz2) == 0 ;
}*/

common::CMutex g_mxDB;

TEST(SyncEngine, loadAllSources0) 
{
    MockRepository mocks;
    CDBAdapter* db = mocks.ClassMock<CDBAdapter>();

    mocks.ExpectCall(db, CDBAdapter::prepareStatement).Return(new CDBResult(g_mxDB));

    CSyncEngine oSyncEngine(*db);
    oSyncEngine.loadAllSources();

    EXPECT_EQ(0, oSyncEngine.getSources().size());
}

TEST(SyncEngine, loadClientID)
{
    MockRepository mocks;
    CDBAdapter* db = mocks.ClassMock<CDBAdapter>();
    CSyncSource* src0 = mocks.ClassMock<CSyncSource>();
    INetRequest* net = mocks.InterfaceMock<INetRequest>();
    IRhoClassFactory* factory = mocks.InterfaceMock<IRhoClassFactory>();
    const char* szclientID = "{\"client\": {\"created_at\": \"2009-05-13T19:28:36Z\", \"session\": null, \"updated_at\": \"2009-05-13T19:28:36Z\", \"user_id\": null, \"client_id\": \"dce301bd-053e-4591-a2c5-4813c201c623\", \"last_sync_token\": null}}";

    mocks.ExpectCall(factory, IRhoClassFactory::createNetRequest).Return(net);
    mocks.ExpectCall(db, CDBAdapter::prepareStatement).With("SELECT client_id from client_info limit 1").Return(new CDBResult(g_mxDB));
    mocks.ExpectCall(net, INetRequest::pullData).
        With(String("/clientcreate")+CSyncEngine::SYNC_SOURCE_FORMAT()).Return(new CNetData(szclientID));
    mocks.ExpectCall(db, CDBAdapter::prepareStatement).With("DELETE FROM client_info").Return(new CDBResult(g_mxDB));
    mocks.ExpectCall(db, CDBAdapter::prepareStatement).With("INSERT INTO client_info (client_id) values (?)").Return(new CDBResult(g_mxDB));

    CSyncEngine oSyncEngine(*db);
    oSyncEngine.setFactory(factory);
    oSyncEngine.getSources().add(src0);
    oSyncEngine.loadClientID();

    EXPECT_EQ("dce301bd-053e-4591-a2c5-4813c201c623", oSyncEngine.getClientID());
    mocks.removeMock(src0);
}

TEST(SyncEngine, syncAllSourcesOK)
{
    MockRepository mocks;
    CSyncSource* src0 = mocks.ClassMock<CSyncSource>();

    mocks.ExpectCall(src0, CSyncSource::isEmptyToken).Return(true);
    mocks.ExpectCall(src0, CSyncSource::sync);

    CDBAdapter db;
    CSyncEngine oSyncEngine(db);
    oSyncEngine.getSources().add(src0);
    oSyncEngine.setLoggedIn(true);
    oSyncEngine.syncAllSources();

    mocks.removeMock(src0);
}

TEST(SyncEngine, syncShouldStopOnFirstError)
{
    MockRepository mocks;
    IRhoClassFactory* factory = mocks.InterfaceMock<IRhoClassFactory>();
    INetRequest* net = mocks.InterfaceMock<INetRequest>();

    mocks.ExpectCall(factory, IRhoClassFactory::createNetRequest).Return(net);
    mocks.ExpectCall(net, INetRequest::pullData).Return(new CNetData(0));
    mocks.NeverCall(net, INetRequest::pullData);

    CDBAdapter db;
    CSyncEngine oSyncEngine(db);
    oSyncEngine.setLoggedIn(true);
    oSyncEngine.setFactory(factory);
    CSyncSource* pSrc1 = new CSyncSource(oSyncEngine);
    CSyncSource* pSrc2 = new CSyncSource(oSyncEngine);

    oSyncEngine.getSources().add(pSrc1);
    oSyncEngine.getSources().add(pSrc2);
    oSyncEngine.syncAllSources();

    EXPECT_EQ(0, oSyncEngine.getStartSource());
    EXPECT_EQ(CSyncEngine::esStop, oSyncEngine.getState());
}

TEST(SyncEngine, syncShouldExit)
{
    MockRepository mocks;
    CSyncSource* src0 = mocks.ClassMock<CSyncSource>();

    mocks.ExpectCall(src0, CSyncSource::isEmptyToken).Return(true);
    mocks.NeverCall(src0, CSyncSource::sync);

    CDBAdapter db;
    CSyncEngine oSyncEngine(db);
    oSyncEngine.setLoggedIn(true);
    oSyncEngine.getSources().add(src0);
    oSyncEngine.setState(CSyncEngine::esExit);
    oSyncEngine.syncAllSources();

    mocks.removeMock(src0);
}

TEST(SyncEngine, syncShouldStartFrom1)
{
    MockRepository mocks;
    CSyncSource* src0 = mocks.ClassMock<CSyncSource>();
    CSyncSource* src1 = mocks.ClassMock<CSyncSource>();

    mocks.ExpectCall(src0, CSyncSource::isEmptyToken).Return(true);
    mocks.ExpectCall(src1, CSyncSource::isEmptyToken).Return(false);
    mocks.NeverCall(src0, CSyncSource::sync);
    mocks.ExpectCall(src1, CSyncSource::sync);

    CDBAdapter db;
    CSyncEngine oSyncEngine(db);
    oSyncEngine.setLoggedIn(true);
    oSyncEngine.getSources().add(src0);
    oSyncEngine.getSources().add(src1);
    oSyncEngine.syncAllSources();

    mocks.removeMock(src0);
    mocks.removeMock(src1);
}

TEST(SyncEngine, loginOK)
{
    MockRepository mocks;

    IRhoClassFactory* factory = mocks.InterfaceMock<IRhoClassFactory>();
    INetRequest* net = mocks.InterfaceMock<INetRequest>();
    CDBAdapter* db = mocks.ClassMock<CDBAdapter>();

    mocks.ExpectCall(factory, IRhoClassFactory::createNetRequest).Return(net);
    mocks.ExpectCall(net, INetRequest::pullCookies).With("http://dev.rhosync.rhohub.com/apps/1/sources/15/client_login",
        "login=name&password=pwd&remember_me=1").Return(true);
    mocks.ExpectCall(db, CDBAdapter::prepareStatement).Return(new CDBResult(g_mxDB));

    CSyncEngine oSyncEngine(*db);
    oSyncEngine.setFactory(factory);
    oSyncEngine.getSources().add(new CSyncSource(15,"http://dev.rhosync.rhohub.com/apps/1/sources/15",0,oSyncEngine));
    oSyncEngine.getSources().add(new CSyncSource(13,"http://dev.rhosync.rhohub.com/apps/1/sources/13",0,oSyncEngine));
    bool bRet = oSyncEngine.doLogin("name","pwd");

    EXPECT_EQ(true, bRet);
}

TEST(SyncEngine, loginNetworkError)
{
    MockRepository mocks;

    IRhoClassFactory* factory = mocks.InterfaceMock<IRhoClassFactory>();
    INetRequest* net = mocks.InterfaceMock<INetRequest>();
    CDBAdapter* db = mocks.ClassMock<CDBAdapter>();

    mocks.ExpectCall(factory, IRhoClassFactory::createNetRequest).Return(net);
    mocks.ExpectCall(net, INetRequest::pullCookies).With("http://dev.rhosync.rhohub.com/apps/1/sources/15/client_login",
        "login=name&password=pwd&remember_me=1").Return(false);
    mocks.NeverCall(db, CDBAdapter::prepareStatement);

    CSyncEngine oSyncEngine(*db);
    oSyncEngine.setFactory(factory);
    oSyncEngine.getSources().add(new CSyncSource(15,"http://dev.rhosync.rhohub.com/apps/1/sources/15",0,oSyncEngine));
    oSyncEngine.getSources().add(new CSyncSource(13,"http://dev.rhosync.rhohub.com/apps/1/sources/13",0,oSyncEngine));
    bool bRet = oSyncEngine.doLogin("name","pwd");

    EXPECT_EQ(false, bRet);
}

TEST(SyncEngine, loginErrorSeveralDomains)
{
    MockRepository mocks;

    IRhoClassFactory* factory = mocks.InterfaceMock<IRhoClassFactory>();
    INetRequest* net = mocks.InterfaceMock<INetRequest>();
    CDBAdapter* db = mocks.ClassMock<CDBAdapter>();

    mocks.ExpectCall(factory, IRhoClassFactory::createNetRequest).Return(net);
    mocks.NeverCall(net, INetRequest::pullCookies);
    mocks.NeverCall(db, CDBAdapter::prepareStatement);

    CSyncEngine oSyncEngine(*db);
    oSyncEngine.setFactory(factory);
    oSyncEngine.getSources().add(new CSyncSource(15,"http://dev.rhosync.rhohub.com/apps/1/sources/15",0,oSyncEngine));
    oSyncEngine.getSources().add(new CSyncSource(13,"http://dev1.rhosync.rhohub.com/apps/1/sources/13",0,oSyncEngine));
    bool bRet = oSyncEngine.doLogin("name","pwd");

    EXPECT_EQ(false, bRet);
}

TEST(SyncEngine, notifications)
{
    MockRepository mocks;
    IRhoClassFactory* factory = mocks.InterfaceMock<IRhoClassFactory>();
    INetRequest* net = mocks.InterfaceMock<INetRequest>();

    mocks.ExpectCall(factory, IRhoClassFactory::createNetRequest).Return(net);
    mocks.ExpectCall(net, INetRequest::resolveUrl).With("url1").Return("http://localhost/url1");
    mocks.ExpectCall(net, INetRequest::pushData).With("http://localhost/url1","status=ok&params1").Return(true);
    mocks.NeverCall(net, INetRequest::resolveUrl);
    mocks.NeverCall(net, INetRequest::pushData);

    CDBAdapter db;
    CSyncEngine oSyncEngine(db);
    oSyncEngine.setFactory(factory);
    oSyncEngine.setNotification(1,"url1","params1");
    oSyncEngine.fireNotification(1,1);
    oSyncEngine.fireNotification(1,1);
}
