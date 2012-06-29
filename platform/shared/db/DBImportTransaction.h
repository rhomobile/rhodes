#pragma once

#include "DBAdapter.h"

namespace rho {
namespace db {

class CDBImportTransaction {
	CDBAdapter& _db;
	String _srcZipPath;
	String _stateFileName;
	String _importDirPath;
	String _importZipPath;
	String _backupDirPath;
	String _backupBlobsDirPath;
	String _blobsListPath;
	
	enum enTxnState {
		txnInvalid,
		txnNotStarted, 
		txnPrepareImportData,
		txnEnumerateOldBlobs,
		txnBackupOldDb,
		txnBackupOldBlobs,
		txnImportNewDb,
		txnImportNewBlobs,
		txnCleanup
	};
	
	enTxnState _state;
public:
	DEFINE_LOGCLASS;

	CDBImportTransaction( CDBAdapter& db, const String& zipPath = "" );
	~CDBImportTransaction() {}
	
	bool commit();
	bool rollback();
	bool pending();
	
private:
	void determineTxnState();
	void setTxnState( enTxnState state );
	
	bool prepareImportData();
	bool enumerateOldBlobs();
	bool backupOldDb();
	bool backupOldBlobs();
	bool importNewDb();
	bool importNewBlobs();
	bool cleanup();
};

}

}