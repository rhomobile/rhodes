#include "../../../wp8/rhoruntime/common/RhoConvertWP8.h"
#include "MethodResultImpl.h"

namespace rhoruntime {

CMethodResultImpl::CMethodResultImpl(int64 native):
    oResult((rho::apiGenerator::CMethodResult*)native)
{
}

void CMethodResultImpl::set(bool res)
{
    oResult->set(res);
}

void CMethodResultImpl::set(int64 res)
{
    oResult->set(res);
}

void CMethodResultImpl::set(int res)
{
    oResult->set(res);
}

void CMethodResultImpl::set(double res)
{
    oResult->set(res);
}

void CMethodResultImpl::set(Platform::String^ res)
{
    oResult->set(rho::common::convertStringWFromWP8(res));
}

void CMethodResultImpl::set(Windows::Foundation::Collections::IVectorView<Platform::String^>^ res)
{
    oResult->set(rho::common::convertArrayFromWP8(res));
}

void CMethodResultImpl::set(Windows::Foundation::Collections::IMapView<Platform::String^, Platform::String^>^ res)
{
    oResult->set(rho::common::convertHashFromWP8(res));
}

}
