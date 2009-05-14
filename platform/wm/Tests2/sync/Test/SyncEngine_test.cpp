#include <gtest/gtest.h>

#include "hippomocks/hippomocks.h"

#include "sync/SyncEngine.h"
#include "db/DBAdapter.h"
#include "db/DBResult.h"
#include "common/IRhoClassFactory.h"

using namespace rho;
using namespace rho::sync;
using namespace rho::db;
using namespace rho::net;
using namespace rho::common;

TEST(SyncEngine, loadAllSources0) 
{
    MockRepository mocks;

    CDBAdapter* db = mocks.ClassMock<CDBAdapter>();
    mocks.ExpectCall(db, CDBAdapter::executeStatement).Return(new CDBResult());

    CSyncEngine oSyncEngine(*db);
    oSyncEngine.loadAllSources();

    EXPECT_EQ(0, oSyncEngine.getSources().size());
}

void netClientResp(const String& strUrl, const String& strQuery, CNetResponse& oResponse )
{
    oResponse.m_strResponse = "clientID1";
}

TEST(SyncEngine, loadClientID)
{
    MockRepository mocks;

    CDBAdapter* db = mocks.ClassMock<CDBAdapter>();

    INetRequest* net = mocks.InterfaceMock<INetRequest>();
    IRhoClassFactory* factory = mocks.InterfaceMock<IRhoClassFactory>();
    CSyncSource* src0 = mocks.ClassMock<CSyncSource>();
    CNetResponse oNetResp;

    mocks.ExpectCall(factory, IRhoClassFactory::createNetRequest).Return(net);
    mocks.ExpectCall(db, CDBAdapter::executeStatement).Return(new CDBResult());
    mocks.ExpectCall(net, INetRequest::pullData).
        With(String(""),String("clientcreate")+CSyncEngine::SYNC_SOURCE_FORMAT(), oNetResp).Do(netClientResp);
    mocks.ExpectCall(db, CDBAdapter::executeStatement).Return(new CDBResult());

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
    //TODO: syncShouldStopOnFirstError
    /*
    CSyncSource* src0 = mocks.ClassMock<CSyncSource>();
    CSyncSource* src1 = mocks.ClassMock<CSyncSource>();
    mocks.ExpectCall(src0, CSyncSource::isEmptyToken).Return(true);
    mocks.ExpectCall(src1, CSyncSource::isEmptyToken).Return(true);
    mocks.ExpectCall(src0, CSyncSource::sync).Return(false);
    mocks.NeverCall(src1, CSyncSource::sync).Return(true);

    CSyncEngine oSyncEngine;
    oSyncEngine.getSources().add(src0);
    oSyncEngine.getSources().add(src1);
    oSyncEngine.syncAllSources();

    mocks.removeMock(src0);
    mocks.removeMock(src1);
*/
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


