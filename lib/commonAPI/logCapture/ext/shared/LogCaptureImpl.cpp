#include "common/RhodesApp.h"
#include "common/RhoStd.h"
#include "common/RhoConf.h"
#include "common/RhoFile.h"
#include "common/RhoDefs.h"
#include "common/Tokenizer.h"
#include "logging/RhoLogConf.h"
#include "generated/cpp/LogCaptureBase.h"

namespace rho {
  
    using namespace apiGenerator;
    using namespace common;
    
    class CLogCaptureImpl: public CLogCaptureSingletonBase, public ILogSink
    {
    public:
        
        CLogCaptureImpl(): CLogCaptureSingletonBase(), mMaxLines(1024), mTotalSize(0) {}
        virtual ~CLogCaptureImpl()
        {
            LOGCONF().removeAuxSink(this);
        }
        
        // ILogSink
        virtual void writeLogMessage( String& strMsg )
        {
            if ( mMaxLines > 0 )
            {
                if (mCapturedLog.size() > mMaxLines)
                {
                    mCapturedLog.clear();
                    mTotalSize = 0;
                }
                
                if (mExcludeCategories.size()>0)
                {
                    for(Vector<String>::const_iterator it = mExcludeCategories.begin(); it != mExcludeCategories.end(); ++it)
                    {
                        if (strMsg.find(*it)!=String::npos)
                        {
                            return;
                        }
                    }
                }
                mCapturedLog.push_back(strMsg);
                mTotalSize += strMsg.size() + 2;
            }
        }
        
        virtual int getCurPos()
        {
            return mCapturedLog.size();
        }
        
        virtual void clear()
        {
            
        }
        
        // ILogCaptureSingleto
        virtual void getExcludeCategories(rho::apiGenerator::CMethodResult& oResult)
        {
            const Vector<rho::String>& log_attr = mExcludeCategories;
            
            size_t len = 0;
            for(Vector<rho::String>::const_iterator iter = log_attr.begin(); iter != log_attr.end(); iter++)
            {
                len += (*iter).length() + 1; // including separator
            }
            
            rho::String str;
            str.reserve(len);
            
            for(Vector<rho::String>::const_iterator iter = log_attr.begin(); iter != log_attr.end(); iter++)
            {
                if (iter != log_attr.begin())
                {
                    str += ",";
                }
                str += *iter;
            }
            
            oResult.set( str );
        }
        
        virtual void setExcludeCategories( const rho::String& excludeCategories, rho::apiGenerator::CMethodResult& oResult)
        {
            if ( excludeCategories.length() > 0 )
            {
                rho::common::CTokenizer oTokenizer( excludeCategories, "," );
                while (oTokenizer.hasMoreTokens())
                {
                    String tok = rho::String_trim(oTokenizer.nextToken());
                    if (tok.length() == 0)
                        continue;

                    mExcludeCategories.addElement( tok );
                } 
            }
            else
                mExcludeCategories.removeAllElements();
        }
        
        virtual void getMaxLines(rho::apiGenerator::CMethodResult& oResult)
        {
            oResult.set(mMaxLines);
        }
        
        virtual void setMaxLines( int maxLines, rho::apiGenerator::CMethodResult& oResult)
        {
            mMaxLines = maxLines;
        }
        
        virtual void start(rho::apiGenerator::CMethodResult& oResult)
        {
            LOGCONF().addAuxSink(this);
        }
        
        virtual void stop(rho::apiGenerator::CMethodResult& oResult)
        {
            LOGCONF().removeAuxSink(this);
        }
        
        virtual void clear(rho::apiGenerator::CMethodResult& oResult)
        {
            mCapturedLog.clear();
            mTotalSize = 0;
        }
        
        virtual void numLines(rho::apiGenerator::CMethodResult& oResult)
        {
            oResult.set( static_cast<int>(mCapturedLog.size()) );
        }
        
        virtual void read(rho::apiGenerator::CMethodResult& oResult)
        {
            String str;

            str.reserve(mTotalSize);
            for (Vector<String>::iterator it = mCapturedLog.begin(); it != mCapturedLog.end(); ++it )
            {
                str += *it;
                str += '\n';
            }
            
            oResult.set(str);
        }
            
    private:
        Vector<String> mCapturedLog;
        Vector<String> mExcludeCategories;
        int mMaxLines;
        size_t mTotalSize;
    };
    
    class CLogCaptureFactory: public CLogCaptureFactoryBase
    {
    public:
        ~CLogCaptureFactory(){}
        
        ILogCaptureSingleton* createModuleSingleton()
        { 
            return new CLogCaptureImpl(); 
        }
    };
}

extern "C" void Init_LogCapture_extension()
{
    rho::CLogCaptureFactory::setInstance( new rho::CLogCaptureFactory() );
    rho::Init_LogCapture_API();
}
