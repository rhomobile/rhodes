#pragma once

#include "DBAdapter.h"

namespace rho {
namespace db {
	
class CDBImportTransaction {
	CDBAdapter& _db;
	const String _srcZipPath;
	const String _stateFileName;
	const String _importDirPath;
	const String _importZipPath;
	const String _backupDirPath;
	const String _backupBlobsDirPath;
	const String _blobsListPath;
	
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