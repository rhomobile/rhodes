//
//  DBRequestHelper.cpp
//  rhosynclib
//
//  Created by Alex Epifanoff on 19.06.12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include "DBRequestHelper.h"
#include "common/AutoPointer.h"
#include "common/RhodesApp.h"
#include "common/RhoFilePath.h"

namespace rho {
namespace db {
		

	class BlobsRequest {
	protected:
		CDBAdapter& _db;
		int _srcID;
		Vector<String> _blobAttrs;
		
		virtual void requestBlobsForAttrib(const String& attr, Vector<String>& out ) = 0;
		
	public:
		BlobsRequest(CDBAdapter& db, int srcID):_db(db),_srcID(srcID) {
			_blobAttrs = db.getAttrMgr().getBlobAttrs(_srcID);
		}
		virtual ~BlobsRequest() {}
		void getBlobs(Vector<String>& out) {
			Vector<String> blobs;
			for ( Vector<String>::const_iterator it = _blobAttrs.begin(); it != _blobAttrs.end(); ++it ) {
				requestBlobsForAttrib(*it,out);
			}
		}
	};
	
	class PropertyBagBlobsRequest : public BlobsRequest {
	public:
		PropertyBagBlobsRequest(CDBAdapter& db, int srcID) :
		BlobsRequest(db, srcID) {}
		
		virtual void requestBlobsForAttrib(const String& attr, Vector<String>& out ) {
			IDBResult res = _db.executeSQL( "SELECT value FROM object_values WHERE source_id=? AND attrib=?", _srcID, attr.c_str() );
			for ( ;!res.isEnd();res.next()) {
				out.push_back( /*common::CFilePath::join( RHODESAPP().getBlobsDirPath(), */res.getStringByIdx(0)/*)*/ );
			}
		}
		
	};
	
	class FixedSchemaBlobsRequest : public BlobsRequest {
		const String _srcName;
	public:
		FixedSchemaBlobsRequest(CDBAdapter& db, int srcID, const String& srcName) :
		BlobsRequest(db, srcID), _srcName(srcName) { }
		
		virtual void requestBlobsForAttrib(const String& attr, Vector<String>& out ) {
			String sql = "SELECT " + attr + " FROM " + _srcName;
			IDBResult res = _db.executeSQL(sql.c_str());
			for ( ;!res.isEnd();res.next()) {
				out.push_back( /*common::CFilePath::join( RHODESAPP().getBlobsDirPath(), */res.getStringByIdx(0)/*)*/ );
			}
		}
		
	};
	
	
	Vector<String> CDBRequestHelper::requestBlobs( int srcID ) {
		Vector<String> list;
		
		IDBResult res = _db.executeSQL("SELECT source_id, name, schema FROM sources WHERE source_id=?", srcID);
		for ( ;!res.isEnd();res.next() ) {
			int srcId = res.getIntByIdx(0);
			String srcName = res.getStringByIdx(1);
			boolean isSchemaSource = res.getStringByIdx(2).length() > 0;
			
			common::CAutoPtr<BlobsRequest> request;
			if  (isSchemaSource) {
				request = new FixedSchemaBlobsRequest(_db,srcId,srcName);
			} else {
				request = new PropertyBagBlobsRequest(_db,srcId);
			}
			
			request->getBlobs(list);
		}
		
		return list;
	}
	
	Vector<String> CDBRequestHelper::requestBlobs(  ) {
		Vector<String> list;
		
		IDBResult res = _db.executeSQL("SELECT source_id, name, schema FROM sources");
		for ( ;!res.isEnd();res.next() ) {
			int srcId = res.getIntByIdx(0);
			String srcName = res.getStringByIdx(1);
			boolean isSchemaSource = res.getStringByIdx(2).length() > 0;
			
			common::CAutoPtr<BlobsRequest> request;
			if  (isSchemaSource) {
				request = new FixedSchemaBlobsRequest(_db,srcId,srcName);
			} else {
				request = new PropertyBagBlobsRequest(_db,srcId);
			}
			
			request->getBlobs(list);
		}
		
		return list;
	}
}
}