
#import "InitMemoryInfoCollector.h"

#include "../shared/common/RhoStd.h"
#include "../shared/common/AutoPointer.h"
#include "../shared/logging/RhoLogConf.h"
#include <sstream>

#import <mach/mach.h>

namespace rho
{
    namespace common
    {
        
        class IPhoneMemoryInfoCollector : public IMemoryInfoCollector
        {
        public:
            virtual String collect()
            {
                struct task_basic_info info;
                mach_msg_type_number_t size = sizeof(info);
                kern_return_t kerr = task_info(mach_task_self(),TASK_BASIC_INFO,(task_info_t)&info,&size);
                if ( KERN_SUCCESS == kerr )
                {
                    std::stringstream s;
                    s << info.resident_size;
                    return "Memory info: \nResident size: " + s.str() + "\n";
                }
                else
                {
                    return "Can't get memory info\n";
                }
                
            }
        };
        
    }
    
}

static rho::common::CAutoPtr<rho::common::IPhoneMemoryInfoCollector> s_memory_info_collector(new rho::common::IPhoneMemoryInfoCollector());

extern "C"
{

void InitMemoryInfoCollector()
{
    LOGCONF().setMemoryInfoCollector(s_memory_info_collector);
}

}