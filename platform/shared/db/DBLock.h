#pragma once

#include <memory>

namespace rho
{
	namespace db
	{
		class CRubyMutexImpl;

		class CRubyMutex
		{
			//make impl opaque to avoid exposure of Ruby headers
            std::auto_ptr<CRubyMutexImpl> m_pImpl;
		public:
            CRubyMutex(bool bIgnore);
		    ~CRubyMutex();

		    void create();
            bool isMainRubyThread();
		    void Lock();
		    void Unlock();

		    void close();
		};

        class CDBAdapter;

		class DBLock {
			CDBAdapter& _db;
		public:
            DBLock(CDBAdapter& db);
            ~DBLock();
		};

	}
}
