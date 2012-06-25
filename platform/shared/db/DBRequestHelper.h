//
//  DBRequestHelper.h
//  rhosynclib
//
//  Created by Alex Epifanoff on 19.06.12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include "DBAdapter.h"

namespace rho {
namespace db {
	
	class CDBRequestHelper {
	protected:
		CDBAdapter& _db;
		
	public:
		CDBRequestHelper(CDBAdapter& db): _db(db) {}
		
		Vector<String> requestBlobs( int srcID );
		Vector<String> requestBlobs(  );
	};

	
}
}