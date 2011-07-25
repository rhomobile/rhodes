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

#include "sync/SyncSource.h"
#include "sync/SyncEngine.h"
#include "NetDataStub.h"

using namespace rho;
using namespace rho::sync;
using namespace rho::db;
using namespace rho::net;
using namespace rho::common;

extern common::CMutex g_mxDB;

template <>
struct printArg<DBResultPtr&>
{
  static inline void print(std::ostream &os, DBResultPtr& arg, bool withComma)
  {
    if (withComma) os << ",";
    os << arg.operator ->();
  }
}; 

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

TEST(SyncSource, syncWithoutNetwork)
{
    MockRepository mocks;
    INetRequest* net = mocks.InterfaceMock<INetRequest>();
    IRhoClassFactory* factory = mocks.InterfaceMock<IRhoClassFactory>();

    mocks.ExpectCall(factory, IRhoClassFactory::createNetRequest).Return(net);
    mocks.ExpectCall(net, INetRequest::pullData).With(String("?format=json&client_id=&p_size=")+CSyncEngine::SYNC_PAGE_SIZE()+"&ack_token=6869672616465")
        .Return(new CNetData(0));

    CDBAdapter db;
    CSyncEngine oSyncEngine(db);
    oSyncEngine.setFactory(factory);

    CSyncSource oSrc(oSyncEngine);
    oSrc.setToken(6869672616465ll);
    oSrc.sync();
}

TEST(SyncSource, syncWithoutNetworkFirstTime)
{
    MockRepository mocks;
    INetRequest* net = mocks.InterfaceMock<INetRequest>();
    IRhoClassFactory* factory = mocks.InterfaceMock<IRhoClassFactory>();

    mocks.ExpectCall(factory, IRhoClassFactory::createNetRequest).Return(net);
    mocks.ExpectCall(net, INetRequest::pullData).With(String("?format=json&client_id=&p_size=") + CSyncEngine::SYNC_PAGE_SIZE()).
        Return(new CNetData(0));

    CDBAdapter db;
    CSyncEngine oSyncEngine(db);
    oSyncEngine.setFactory(factory);

    CSyncSource oSrc(oSyncEngine);
    oSrc.sync();
}

TEST(SyncSource, syncOneSourceOK)
{
    MockRepository mocks;
    INetRequest* net = mocks.InterfaceMock<INetRequest>();
    IRhoClassFactory* factory = mocks.InterfaceMock<IRhoClassFactory>();

    mocks.ExpectCall(factory, IRhoClassFactory::createNetRequest).Return(net);
    mocks.ExpectCall(net, INetRequest::pullData).With(String("?format=json&client_id=&p_size=") + CSyncEngine::SYNC_PAGE_SIZE()).
        Return(new CNetData("[{\"count\": 200},{\"token\": \"6869672616465\"},\
            {\"object_value\": {\"attrib\": \"shipping_address_state\", \"update_type\": \"query\", \"id\": -1032404474, \"value\": \"NJ\", \"source_id\": 1, \"object\": \"1460099a-be9d-59f1-16c9-4544f051792a\", \"db_operation\": \"insert\"}}]"));
    mocks.ExpectCall(net, INetRequest::pullData).With(String("?format=json&client_id=&p_size=") + CSyncEngine::SYNC_PAGE_SIZE()+
        "&ack_token=6869672616465").Return(new CNetData("[{\"count\": 0},{\"token\": \"6869672616467\"}]"));

    CDBAdapter db;
    CSyncEngine oSyncEngine(db);
    oSyncEngine.setFactory(factory);

    CSyncSource oSrc(oSyncEngine);
    oSrc.syncServerChanges();

    EXPECT_EQ(CSyncEngine::esNone, oSyncEngine.getState());
    EXPECT_EQ(6869672616467ll, oSrc.getToken());
}

TEST(SyncSource, syncOneSourceServerError)
{
    MockRepository mocks;
    INetRequest* net = mocks.InterfaceMock<INetRequest>();
    IRhoClassFactory* factory = mocks.InterfaceMock<IRhoClassFactory>();

    mocks.ExpectCall(factory, IRhoClassFactory::createNetRequest).Return(net);
    mocks.ExpectCall(net, INetRequest::pullData).Return(new CNetData(0));

    CDBAdapter db;
    CSyncEngine oSyncEngine(db);
    oSyncEngine.setFactory(factory);

    CSyncSource oSrc(oSyncEngine);
    oSrc.syncServerChanges();

    EXPECT_EQ(CSyncEngine::esStop, oSyncEngine.getState());
    EXPECT_EQ(1, oSrc.getToken());
    EXPECT_EQ(0,oSrc.getInsertedCount());
    EXPECT_EQ(0,oSrc.getDeletedCount());
    EXPECT_EQ(0,oSrc.getCurPageCount());
}

TEST(SyncSource, syncOneSourceAsk)
{
    MockRepository mocks;
    INetRequest* net = mocks.InterfaceMock<INetRequest>();
    IRhoClassFactory* factory = mocks.InterfaceMock<IRhoClassFactory>();

    mocks.ExpectCall(factory, IRhoClassFactory::createNetRequest).Return(net);
    mocks.ExpectCall(net, INetRequest::pullData).With(String("/ask?format=json&client_id=&p_size=") + CSyncEngine::SYNC_PAGE_SIZE()+
        String("&question=GetUrl")).
        Return(new CNetData("[{\"count\": 200},{\"token\": \"6869672616465\"},\
        {\"object_value\": {\"attrib\": \"shipping_address_state\", \"update_type\": \"query\", \"id\": -1032404474, \"value\": \"NJ\", \"source_id\": 1, \"object\": \"1460099a-be9d-59f1-16c9-4544f051792a\", \"db_operation\": \"insert\"}}]"));
    mocks.NeverCall(net, INetRequest::pullData);

    CDBAdapter db;
    CSyncEngine oSyncEngine(db);
    oSyncEngine.setFactory(factory);

    CSyncSource oSrc(oSyncEngine);
    oSrc.setAskParams("GetUrl");
    oSrc.syncServerChanges();

    EXPECT_EQ(CSyncEngine::esNone, oSyncEngine.getState());
    EXPECT_EQ(6869672616465ll, oSrc.getToken());

}

TEST(SyncSource, makePushBody)
{
    MockRepository mocks;
    CDBAdapter* db = mocks.ClassMock<CDBAdapter>();
    CDBResult* res = mocks.ClassMock<CDBResult>();

    mocks.ExpectCall(db, CDBAdapter::prepareStatement).With("SELECT attrib, object, value, attrib_type "
					 "FROM object_values where source_id=? and update_type =?").Return(res);
    mocks.ExpectCall(res, CDBResult::isEnd).Return(false);
    mocks.ExpectCall(res, CDBResult::getStringByIdx).With(0).Return("attr1");
    mocks.ExpectCall(res, CDBResult::getStringByIdx).With(1).Return("obj1");
    mocks.ExpectCall(res, CDBResult::getStringByIdx).With(1).Return("obj1");
    mocks.ExpectCall(res, CDBResult::getStringByIdx).With(2).Return("val1");
    mocks.ExpectCall(res, CDBResult::getStringByIdx).With(3).Return("");
    mocks.ExpectCall(res, CDBResult::isEnd).Return(true);

    CSyncEngine oSyncEngine(*db);
    CSyncSource oSrc(oSyncEngine);
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

    mocks.ExpectCall(db, CDBAdapter::prepareStatement).Return(res);
    mocks.ExpectCall(res, CDBResult::isEnd).Return(false);
    mocks.ExpectCall(res, CDBResult::getStringByIdx).With(0).Return("attr1");
    mocks.ExpectCall(res, CDBResult::getStringByIdx).With(1).Return("obj1");
    mocks.ExpectCall(res, CDBResult::getStringByIdx).With(1).Return("obj1");
    mocks.ExpectCall(res, CDBResult::getStringByIdx).With(2).Return("c:/rho/test.jpg");
    mocks.ExpectCall(res, CDBResult::getStringByIdx).With(3).Return("blob.file");
    mocks.ExpectCall(res, CDBResult::isEnd).Return(true);

    CSyncEngine oSyncEngine(*db);
    CSyncSource oSrc(oSyncEngine);
    String body;
    oSrc.makePushBody(body,"create");

    EXPECT_EQ("",body);
    ASSERT_EQ(1,oSrc.getSyncBlobs().size());
    EXPECT_EQ("attrvals[][attrib]=attr1&attrvals[][object]=obj1&attrvals[][value]=test.jpg&attrvals[][attrib_type]=blob", 
        oSrc.getSyncBlobs().get(0)->getBody());
    EXPECT_EQ("c:/rho/test.jpg",oSrc.getSyncBlobs().get(0)->getFilePath());

    mocks.removeMock(res);
}

TEST(SyncSource, syncClientBlobs)
{
    MockRepository mocks;
    INetRequest* net = mocks.InterfaceMock<INetRequest>();
    IRhoClassFactory* factory = mocks.InterfaceMock<IRhoClassFactory>();
    CDBAdapter* db = mocks.ClassMock<CDBAdapter>();

    mocks.ExpectCall(factory, IRhoClassFactory::createNetRequest).Return(net);
    mocks.ExpectCall(net, INetRequest::pushFile).With("?client_id=cl1&Body", "d:/work/BlobTest.png").Return(true);
    mocks.ExpectCall(db, CDBAdapter::prepareStatement).Return(new CDBResult(g_mxDB));

    CSyncEngine oSyncEngine(*db);
    oSyncEngine.setFactory(factory);
    CSyncSource oSrc(oSyncEngine);
    oSrc.getSyncBlobs().add(new CSyncBlob("Body","d:/work/BlobTest.png"));

    oSrc.syncClientBlobs("?client_id=cl1");
}

TEST(SyncSource, processServerData)
{
    MockRepository mocks;

    CDBAdapter db;
    CSyncEngine oSyncEngine(db);
    CSyncSource oSrc(oSyncEngine);
    const char* data = "[{\"count\": 200},{\"token\": \"6869672616465\"},\
    {\"object_value\": {\"attrib\": \"shipping_address_state\", \"update_type\": \"query\", \"id\": -1032404474, \"value\": \"NJ\", \"source_id\": 1, \"object\": \"1460099a-be9d-59f1-16c9-4544f051792a\", \"db_operation\": \"insert\"}}]";
    oSrc.processServerData(data);

    EXPECT_EQ(1,oSrc.getInsertedCount());
    EXPECT_EQ(0,oSrc.getDeletedCount());
    EXPECT_EQ(200,oSrc.getCurPageCount());
    EXPECT_EQ(6869672616465ll,oSrc.getToken());
}

TEST(SyncSource, processServerDataError)
{
    MockRepository mocks;

    CDBAdapter db;
    CSyncEngine oSyncEngine(db);
    CSyncSource oSrc(oSyncEngine);
    const char* data = "[{\"count\": 200},{\"token\": \"6869672616465\"},\
    {\"object_value\": {\"attrib\": \"shipping_address_state\", \"update_type\": \"query\", \"id\": -1032404474, \"value\": \"Model Trimmer Abrasive Wheel 10\\\\\",A\\\\\"\", \"source_id\": 1, \"object\": \"1460099a-be9d-59f1-16c9-4544f051792a\", \"db_operation\": \"insert\"}}]";
    oSrc.processServerData(data);

    EXPECT_EQ(0,oSrc.getInsertedCount());
    EXPECT_EQ(0,oSrc.getCurPageCount());
    EXPECT_EQ(0,oSrc.getToken());
}

TEST(SyncSource, processServerEmptyData)
{
    MockRepository mocks;

    CDBAdapter db;
    CSyncEngine oSyncEngine(db);
    CSyncSource oSrc(oSyncEngine);
    oSrc.processServerData("[{\"count\": 0}]");

    EXPECT_EQ(0,oSrc.getInsertedCount());
    EXPECT_EQ(0,oSrc.getDeletedCount());
    EXPECT_EQ(0,oSrc.getCurPageCount());
    EXPECT_EQ(0,oSrc.getToken());
}
