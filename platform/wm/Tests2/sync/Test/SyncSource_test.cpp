#include <gtest/gtest.h>
#include "hippomocks/hippomocks.h"

#include "sync/SyncSource.h"
#include "sync/SyncEngine.h"

using namespace rho;
using namespace rho::sync;
using namespace rho::db;
using namespace rho::net;
using namespace rho::common;

TEST(SyncSource, syncWithoutNetwork)
{
    MockRepository mocks;

    INetRequest* net = mocks.InterfaceMock<INetRequest>();
    IRhoClassFactory* factory = mocks.InterfaceMock<IRhoClassFactory>();
    CNetResponse oNetResp;
    CDBResult oRes;

    mocks.ExpectCall(factory, IRhoClassFactory::createNetRequest).Return(net);
    mocks.ExpectCall(net, INetRequest::pushData);
    mocks.ExpectCall(net, INetRequest::pushData);
    mocks.ExpectCall(net, INetRequest::pushData);
    for( int i = 0; i < CSyncEngine::MAX_SYNC_TRY_COUNT()+1; i++ )
        mocks.ExpectCall(net, INetRequest::pullData);

    CDBAdapter db;
    CSyncEngine oSyncEngine(db);
    oSyncEngine.setFactory(factory);

    CSyncSource oSrc(oRes,oSyncEngine);
    oSrc.sync();
}

TEST(SyncSource, makePushBody)
{
    MockRepository mocks;

    CDBAdapter* db = mocks.ClassMock<CDBAdapter>();
    CDBResult* res = mocks.ClassMock<CDBResult>();
    CDBResult oRes;

    mocks.ExpectCall(db, CDBAdapter::executeStatement).Return(res);
    mocks.ExpectCall(res, CDBResult::isEnd).Return(false);
    mocks.ExpectCall(res, CDBResult::getStringByIdx).With(0).Return("attr1");
    mocks.ExpectCall(res, CDBResult::getStringByIdx).With(1).Return("obj1");
    mocks.ExpectCall(res, CDBResult::getStringByIdx).With(1).Return("obj1");
    mocks.ExpectCall(res, CDBResult::getStringByIdx).With(2).Return("val1");
    mocks.ExpectCall(res, CDBResult::getStringByIdx).With(3).Return("");
    mocks.ExpectCall(res, CDBResult::isEnd).Return(true);

    CSyncEngine oSyncEngine(*db);
    CSyncSource oSrc(oRes,oSyncEngine);
    String body;
    oSrc.makePushBody(body,"create");

    EXPECT_EQ("attrvals[][attrib]=attr1&attrvals[][object]=obj1&attrvals[][value]=val1", body);

    mocks.removeMock(res);
}

TEST(SyncSource, makePushBlobBody)
{
    MockRepository mocks;

    CDBAdapter* db = mocks.ClassMock<CDBAdapter>();
    CDBResult* res = mocks.ClassMock<CDBResult>();
    CDBResult oRes;

    mocks.ExpectCall(db, CDBAdapter::executeStatement).Return(res);
    mocks.ExpectCall(res, CDBResult::isEnd).Return(false);
    mocks.ExpectCall(res, CDBResult::getStringByIdx).With(0).Return("attr1");
    mocks.ExpectCall(res, CDBResult::getStringByIdx).With(1).Return("obj1");
    mocks.ExpectCall(res, CDBResult::getStringByIdx).With(1).Return("obj1");
    mocks.ExpectCall(res, CDBResult::getStringByIdx).With(2).Return("c:/rho/test.jpg");
    mocks.ExpectCall(res, CDBResult::getStringByIdx).With(3).Return("blob.file");
    mocks.ExpectCall(res, CDBResult::isEnd).Return(true);

    CSyncEngine oSyncEngine(*db);
    CSyncSource oSrc(oRes,oSyncEngine);
    String body;
    oSrc.makePushBody(body,"create");

    EXPECT_EQ("attrvals[][attrib]=attr1&attrvals[][object]=obj1&attrvals[][value]=test.jpg&attrvals[][attrib_type]=blob", body);

    mocks.removeMock(res);
}

TEST(SyncSource, processServerData)
{
    MockRepository mocks;

    CDBResult oRes;
    CDBAdapter db;

    CSyncEngine oSyncEngine(db);
    CSyncSource oSrc(oRes,oSyncEngine);
    String data = "[{\"count\": 200},{\"token\": \"6869672616465\"},\
    {\"object_value\": {\"attrib\": \"shipping_address_state\", \"update_type\": \"query\", \"id\": -1032404474, \"value\": \"NJ\", \"source_id\": 1, \"object\": \"1460099a-be9d-59f1-16c9-4544f051792a\", \"db_operation\": \"insert\"}}]";
    oSrc.processServerData(data);

    EXPECT_EQ(1,oSrc.getInsertedCount());
    EXPECT_EQ(0,oSrc.getDeletedCount());
    EXPECT_EQ(200,oSrc.getCurPageCount());
    EXPECT_EQ(6869672616465,oSrc.getToken());
}
