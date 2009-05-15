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

TEST(SyncEngine, loadAllSources0) 
{
    MockRepository mocks;
    CDBAdapter* db = mocks.ClassMock<CDBAdapter>();

    mocks.ExpectCall(db, CDBAdapter::prepareStatement).Return(new CDBResult());

    CSyncEngine oSyncEngine(*db);
    oSyncEngine.loadAllSources();

    EXPECT_EQ(0, oSyncEngine.getSources().size());
}

TEST(SyncEngine, loadClientID)
{
    MockRepository mocks;
    CDBAdapter* db = mocks.ClassMock<CDBAdapter>();
    INetRequest* net = mocks.InterfaceMock<INetRequest>();
    IRhoClassFactory* factory = mocks.InterfaceMock<IRhoClassFactory>();
    CSyncSource* src0 = mocks.ClassMock<CSyncSource>();

    mocks.ExpectCall(factory, IRhoClassFactory::createNetRequest).Return(net);
    mocks.ExpectCall(db, CDBAdapter::prepareStatement).Return(new CDBResult());
    mocks.ExpectCall(net, INetRequest::pullData).
        With(String("clientcreate")+CSyncEngine::SYNC_SOURCE_FORMAT()).Return(new CNetData("clientID1"));
    mocks.ExpectCall(db, CDBAdapter::prepareStatement).Return(new CDBResult());

    CSyncEngine oSyncEngine(*db);
    oSyncEngine.setFactory(factory);
    oSyncEngine.getSources().add(src0);
    oSyncEngine.loadClientID();

    EXPECT_EQ("clientID1", oSyncEngine.getClientID());

    mocks.removeMock(src0);
}

TEST(SyncEngine, syncAllSourcesOK)
{
    MockRepository mocks;
    CSyncSource* src0 = mocks.ClassMock<CSyncSource>();

    mocks.ExpectCall(src0, CSyncSource::isEmptyToken).Return(true);
    mocks.ExpectCall(src0, CSyncSource::sync);

    CSyncEngine oSyncEngine;
    oSyncEngine.getSources().add(src0);
    oSyncEngine.syncAllSources();

    mocks.removeMock(src0);
}

TEST(SyncEngine, syncShouldStopOnFirstError)
{
    MockRepository mocks;
    IRhoClassFactory* factory = mocks.InterfaceMock<IRhoClassFactory>();
    INetRequest* net = mocks.InterfaceMock<INetRequest>();

    mocks.ExpectCall(factory, IRhoClassFactory::createNetRequest).Return(net);
    mocks.ExpectCall(net, INetRequest::pushData).Return(false);
    mocks.NeverCall(net, INetRequest::pushData);
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

    CSyncEngine oSyncEngine;
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

    CSyncEngine oSyncEngine;
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
    mocks.ExpectCall(db, CDBAdapter::prepareStatement).Return(new CDBResult());

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

    CSyncEngine oSyncEngine;
    oSyncEngine.setFactory(factory);
    oSyncEngine.setNotification(1,"url1","params1");
    oSyncEngine.fireNotification(1,1);
    oSyncEngine.fireNotification(1,1);
}
