//
//  DBImportTransaction.cpp
//  rhosynclib
//
//  Created by Alex Epifanoff on 18.06.12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include "DBImportTransaction.h"
#include "DBRequestHelper.h"
#include "common/RhodesApp.h"
#include "common/RhoFile.h"
#include "common/RhoFilePath.h"
#include "common/Tokenizer.h"
#include "common/RhoAppAdapter.h"
#include "sync/SyncThread.h"

using namespace rho::common;

namespace rho {
namespace db {
	
	IMPLEMENT_LOGCLASS(CDBImportTransaction,"DB");
	
	CDBImportTransaction::CDBImportTransaction( CDBAdapter& db, const String& zipPath ) :
		_db(db), 
		_srcZipPath(zipPath),
		_state(txnInvalid)
		
	{
		_stateFileName = _db.getDBPath() + ".importTxnState";
		_importDirPath = _db.getDBPath() + ".importData";
		_importZipPath = CFilePath::join(_importDirPath, "import.zip");
		_backupDirPath = _db.getDBPath() + ".backupData";
		_backupBlobsDirPath = CFilePath::join(_backupDirPath, "blobs");
		_blobsListPath = CFilePath::join(_backupDirPath,"blobs.list");
	}
	
	bool CDBImportTransaction::commit() {
		determineTxnState();
		
		char sState[8];
		snprintf(sState, 8, "%d",_state);
		LOG(INFO) + "Commiting DB import transaction, starting from state = " + sState;
		
		//DBLock lock(_db);
		
		switch (_state) {
			case txnNotStarted:			
				setTxnState(txnPrepareImportData);
				//no break here is intentional
			case txnPrepareImportData:				
				if (!prepareImportData() ) {
					LOG(ERROR) + "Error in prepare import data";
					return false;
				}
				setTxnState(txnEnumerateOldBlobs);
				//no break here is intentional
			case txnEnumerateOldBlobs:
				if (!enumerateOldBlobs()) {
					LOG(ERROR) + "Can't enumerate old blobs";
					return false;
				}
				setTxnState(txnBackupOldDb);
				//no break here is intentional
			case txnBackupOldDb:
				if (!backupOldDb()) {
					LOG(ERROR) + "Can't backup old DB";
					return false;
				}
				setTxnState(txnBackupOldBlobs);
				//no break here is intentional
			case txnBackupOldBlobs:
				if (!backupOldBlobs()) {
					LOG(ERROR) + "Can't backup old blobs";
					return false;
				}
				setTxnState(txnImportNewDb);
				//no break here is intentional
			case txnImportNewDb:
				if (!importNewDb()) {
					LOG(ERROR) + "Can't import new DB";
					return false;
				}
				setTxnState(txnImportNewBlobs);
				//no break here is intentional
			case txnImportNewBlobs:
				if (!importNewBlobs()) {
					LOG(ERROR) + "Can't import new blobs";
					return false;
				}
				setTxnState(txnCleanup);
				//no break here is intentional
			case txnCleanup:
				cleanup();
				//if (!cleanup()) {
				//	LOG(ERROR) + "Can't cleanup";
				//	return false;
				//}
				break;
				
			default:
				return false;
		}
		
		return true;
	}
	
	bool CDBImportTransaction::rollback() {
		//determineTxnState();
		
		if ( CRhoFile::isDirectory(_backupDirPath.c_str()) ) {
			CFilePath path(_db.getDBPath());
			String filename = path.getBaseName();
			
			String fDataName = CFilePath::join(_backupDirPath, filename);
			if (!CRhoFile::isFileExist(fDataName.c_str())) {
				LOG(ERROR) + "Database file not found: " + fDataName;
			}
			
			_db.setImportDB(fDataName, "");		
			
			RhoAppAdapter.loadServerSources(String("{\"partition\":\"") + _db.getPartitionName() + "\"}");
			rho_db_init_attr_manager();
			
			if ( CRhoFile::isDirectory(_backupBlobsDirPath.c_str())) {
				CRhoFile::moveFoldersContentToAnotherFolder(_backupBlobsDirPath.c_str(), RHODESAPP().getBlobsDirPath().c_str());
			}
		}
		
		cleanup();
				
		return true;
	}
	
	bool CDBImportTransaction::pending() {
		determineTxnState();
		return _state!=txnInvalid;
	}
	
	void CDBImportTransaction::determineTxnState() {
		if ( CRhoFile::isFileExist(_stateFileName.c_str()) ) {
			CRhoFile f;
			f.open(_stateFileName.c_str(),CRhoFile::OpenReadOnly);
			if (f.readData(&_state,0,sizeof(_state)) != sizeof(_state)) {
				_state = txnInvalid;
			}
		} else if (_srcZipPath.length() > 0 ) {
			_state = txnNotStarted;
		} else {
			_state = txnInvalid;
		}
	}
	
	void CDBImportTransaction::setTxnState(enTxnState state) {
		CRhoFile f;
		f.open(_stateFileName.c_str(),CRhoFile::OpenForWrite);
		f.write(&_state,sizeof(state));
		_state = state;
	}
	
	
	/*Implementation of import steps*/
	
	extern "C" int rho_sys_unzip_file(const char* szZipPath, const char* psw);
	
	bool CDBImportTransaction::prepareImportData() {
		if (  (!CRhoFile::isDirectory(_importDirPath.c_str())) && (CRhoFile::createFolder(_importDirPath.c_str())!=0) ) {
			LOG(ERROR) + "Can't create import data dir: " + _importDirPath;
			return false;
		}
		
		if ( CRhoFile::copyFile(_srcZipPath.c_str(), _importZipPath.c_str())!=0 ) {
			LOG(ERROR) + "Can't copy import data file " + _srcZipPath + " to import dir: " + _importZipPath;
			return false;
		}
		
		if ( rho_sys_unzip_file(_importZipPath.c_str(), 0)!=0 ) {
			LOG(ERROR) + "Can't unzip import data";
			return false;
		}
		
		return true;
	}
	
	bool CDBImportTransaction::enumerateOldBlobs() {
		CDBRequestHelper reqHelper(_db);
		Vector<String> blobs = reqHelper.requestBlobs();
		
		//TODO: process only blobs in blobs_dir
		
		if (  (!CRhoFile::isDirectory(_backupDirPath.c_str())) && (CRhoFile::createFolder(_backupDirPath.c_str()) != 0) ) {
			LOG(ERROR) + "Can't create backup dir";
			return false;
		}
		
		CRhoFile f;
		if (!f.open(_blobsListPath.c_str(),CRhoFile::OpenForWrite)) {
			LOG(ERROR) + "Can't open file to write blobs list";
			return false;
		}

		for (Vector<String>::const_iterator it = blobs.begin(); it != blobs.end(); ++it ) {
			String toWrite = *it + "\n";
			f.write((void*)toWrite.c_str(),toWrite.length());
		}
		
		return true;
	}
	
	bool CDBImportTransaction::backupOldDb() {
		String dbPath = _db.getDBPath();
		CFilePath oPath(dbPath);
		//_db.close();
		if (CRhoFile::copyFile(dbPath.c_str(), CFilePath::join(_backupDirPath, oPath.getBaseName()).c_str()) != 0) {
			LOG(ERROR) + "Can't backup database";
			return false;
		}
		return true;
	}
	
	bool CDBImportTransaction::backupOldBlobs() {
		if ( (!CRhoFile::isDirectory(_backupBlobsDirPath.c_str())) && (CRhoFile::createFolder(_backupBlobsDirPath.c_str())!=0 )) {
			LOG(ERROR) + "Can't create backup blobs dir: " + _backupBlobsDirPath;
			return false;
		}
		
		CRhoFile f;
		if (!f.open(_blobsListPath.c_str(),CRhoFile::OpenReadOnly)) {
			LOG(ERROR) + "Can't open file to read blobs list";
			return false;
		}
		
		String sBlobs;
		f.readString(sBlobs);
		CTokenizer tokenizer(sBlobs,"\n");
		while (tokenizer.hasMoreTokens()) 
        {
		    String tok = tokenizer.nextToken();
			if (tok.length() > 0) {
				CFilePath path(tok);
				if ( CRhoFile::renameFile(tok.c_str(), CFilePath::join(_backupBlobsDirPath, path.getBaseName()).c_str()) != 0 ) {
					LOG(ERROR) + "Can't backup blob: " + tok;
					return false;
				}
			}
		}
		
		return true; 
	}
	
	bool CDBImportTransaction::importNewDb() {
		CFilePath path(_db.getDBPath());
		String filename = path.getBaseName();
		
		String fDataName = CFilePath::join(_importDirPath, filename);
		if (!CRhoFile::isFileExist(fDataName.c_str())) {
			LOG(ERROR) + "Database file not found: " + fDataName;
		}
		
		_db.setImportDB(fDataName, "");		

		RhoAppAdapter.loadServerSources(String("{\"partition\":\"") + _db.getPartitionName() + "\", \"skip_schema\":\"1\" }");

		rho_db_init_attr_manager();

		return true; 
	}
	
	bool CDBImportTransaction::importNewBlobs() {
		if (CRhoFile::isDirectory(RHODESAPP().getBlobsDirPath().c_str()) && (CRhoFile::deleteFolder(RHODESAPP().getBlobsDirPath().c_str())!=0)) {
			LOG(ERROR) + "Can't delete blobs dir";
		}
		
		CRhoFile::renameFile(CFilePath::join(_importDirPath, "db-files").c_str(), RHODESAPP().getBlobsDirPath().c_str());
		
		
		return true; 
	}
	
	bool CDBImportTransaction::cleanup() {
		if ( CRhoFile::isDirectory(_importDirPath.c_str()) && (CRhoFile::deleteFolder(_importDirPath.c_str())!=0)) {
			LOG(ERROR) + "Can't delete import folder: " + _importDirPath;
			//return false;
		}
		
		if ( CRhoFile::isDirectory(_backupDirPath.c_str()) && (CRhoFile::deleteFolder(_backupDirPath.c_str())!=0)) {
			LOG(ERROR) + "Can't delete backup folder: " + _backupDirPath;
			//return false;
		}
		
		if ( CRhoFile::isFileExist(_stateFileName.c_str()) && (CRhoFile::deleteFile(_stateFileName.c_str())!=0)) {
			LOG(ERROR) + "Can't delete import state file: " + _stateFileName;
			//return false;
		}
		
		return true; 
	}
}
}