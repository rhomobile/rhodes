#include "Barcode1.h"

rho::Hashtable<rho::String,IBarcode1*> CBarcode1FactoryBase::m_hashBarcodes;
rho::String CBarcode1FactoryBase::m_strDefaultID;
rho::common::CAutoPtr<rho::common::CThreadQueue> CBarcode1FactoryBase::m_pCommandQueue;
