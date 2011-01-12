#include "StdAfx.h"
#include "RhoCryptImpl.h"
#include "common/RhodesApp.h"

namespace rho{
namespace common{
IMPLEMENT_LOGCLASS(CRhoCryptImpl,"RhoCrypt");
#define checkError(func) _checkError(func, #func)

CRhoCryptImpl::CRhoCryptImpl(void) : m_hCryptProv(0), m_hKey(0)
{
}

CRhoCryptImpl::~CRhoCryptImpl(void)
{
    if ( m_hKey ) 
        CryptDestroyKey(m_hKey);

    if ( m_hCryptProv )
        CryptReleaseContext( m_hCryptProv, 0 );
}

bool CRhoCryptImpl::_checkError( BOOL bRes, const char* szFunc )
{
    m_dwLastError = 0;
    if ( !bRes )
        m_dwLastError = GetLastError();

    if ( m_dwLastError == 0 )
        return true;

    LPTSTR pszMessage = NULL;

    DWORD dwLen = FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | 
        //FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_FROM_HMODULE|
        FORMAT_MESSAGE_IGNORE_INSERTS,
#if defined(_WIN32_WCE)
        GetModuleHandle( _T("Coredll.dll") ),
#else
        GetModuleHandle( _T("Advapi32.dll") ),
#endif
        m_dwLastError,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPTSTR)&pszMessage,
        0, NULL );

    LOG(ERROR) + szFunc + "failed with code: " + LOGFMT("0x%X") + m_dwLastError + ";Message: " + (pszMessage ? pszMessage : _T(""));
    if ( pszMessage )
        LocalFree(pszMessage);

    return false;
}

void CRhoCryptImpl::readKeyFromStorage(DATA_BLOB& blobKey)
{
    StringW strKey = L"Software\\Rhomobile\\";
    strKey += RHODESAPP().getAppNameW() + L"\\Storage\\" + m_strDBPartition;

    CRegKey oKey;
    LONG res = oKey.Open(HKEY_LOCAL_MACHINE, strKey.c_str(), KEY_READ);
    if ( res != ERROR_SUCCESS )
        return;

    ULONG nBytes = 0;
    res = oKey.QueryBinaryValue(L"DataKey", 0, &nBytes );
    if ( res != ERROR_SUCCESS )
        return;

    blobKey.cbData = nBytes;
    blobKey.pbData = (BYTE*)LocalAlloc(LMEM_FIXED, nBytes);
    res = oKey.QueryBinaryValue(L"DataKey", blobKey.pbData, &nBytes );
}

void CRhoCryptImpl::saveKeyToStorage(DATA_BLOB& blobKey)
{
    LONG res = 0;
    StringW strKey = L"Software\\Rhomobile\\";
    strKey += RHODESAPP().getAppNameW() + L"\\Storage\\" + m_strDBPartition;

    {
        CRegKey oKey;
        res = oKey.Create(HKEY_LOCAL_MACHINE, strKey.c_str() );
    }

    CRegKey oKey;
    res = oKey.Open(HKEY_LOCAL_MACHINE, strKey.c_str(), KEY_WRITE);
    if ( res != ERROR_SUCCESS )
        return;

    res = oKey.SetBinaryValue(L"DataKey", blobKey.pbData, blobKey.cbData );
    if ( res != ERROR_SUCCESS )
        return;
}

void CRhoCryptImpl::writeKeyToStorage()
{
    BYTE *pbKeyBlob = NULL;
    DWORD dwBlobLen = 0;
    if ( !checkError( CryptExportKey(m_hKey, 0, PLAINTEXTKEYBLOB, 0, NULL, &dwBlobLen)) )
        return;

    pbKeyBlob = (BYTE*)malloc(dwBlobLen);
    checkError( CryptExportKey(m_hKey, 0, PLAINTEXTKEYBLOB, 0, pbKeyBlob, &dwBlobLen) );

    DATA_BLOB unprotectedKey = {dwBlobLen , pbKeyBlob };
    DATA_BLOB protectedKey = {0};
    checkError( CryptProtectData( &unprotectedKey, L"Rhodes data key", 0, 0, 0, 0, &protectedKey ) );

    if (pbKeyBlob)
        free(pbKeyBlob);

    saveKeyToStorage(protectedKey);

    if ( protectedKey.pbData )
        LocalFree(protectedKey.pbData);
}

void CRhoCryptImpl::initContext(const char* szPartition)
{
    if ( m_hCryptProv )
        return;

    convertToStringW(szPartition, m_strDBPartition);

    LPCTSTR szContName = _T("RhodesKeyContainer");  // Name of the key container to be used.
    checkError( CryptAcquireContext( &m_hCryptProv, szContName, NULL, PROV_RSA_AES, 0) ); 

    if ( getErrorCode() == NTE_BAD_KEYSET )
    {
        checkError( CryptAcquireContext( &m_hCryptProv, szContName, NULL, PROV_RSA_AES, CRYPT_NEWKEYSET) );
    }

    if ( getErrorCode() != 0 )
        return;

    DATA_BLOB protectedKey = {0};
    readKeyFromStorage(protectedKey);
    if ( protectedKey.cbData > 0 )
    {
        DATA_BLOB unprotectedKey = {0};
        LPWSTR pDescr =  NULL;
        checkError(CryptUnprotectData( &protectedKey, &pDescr, NULL, NULL, NULL, 0, &unprotectedKey));

        checkError(CryptImportKey(m_hCryptProv, unprotectedKey.pbData, unprotectedKey.cbData, 0, 0, &m_hKey));

        if( pDescr )
            LocalFree(pDescr);
        if ( unprotectedKey.pbData )
            LocalFree(unprotectedKey.pbData);
        if ( protectedKey.pbData )
            LocalFree(protectedKey.pbData);
    }else
    {  
        //generate new key
        if ( checkError(CryptGenKey(m_hCryptProv, CALG_RC4, CRYPT_EXPORTABLE, &m_hKey)) )
        {
            //DWORD dwPadding = ZERO_PADDING;
            //checkError( CryptSetKeyParam( m_hKey, KP_PADDING, (const BYTE*)&dwPadding, 0) ); 

            writeKeyToStorage();
        }
    }
}

int CRhoCryptImpl::db_encrypt( const char* szPartition, int size, unsigned char* data, unsigned char* dataOut )
{
    initContext(szPartition);

    memcpy(dataOut, data, size);

    DWORD dwSize = size;
    if ( m_hKey )
    {
        checkError( CryptEncrypt( m_hKey,
                0,               // No hashing of data
                TRUE,            // Final or only buffer of data
                0,               // Must be zero
                dataOut,             // Data buffer
                &dwSize,         // Size of data
                dwSize           // Size of block
        ));         
    }

    return getErrorCode() == 0 ? 1 : 0;
}

int CRhoCryptImpl::db_decrypt( const char* szPartition, int size, unsigned char* data )
{
    initContext(szPartition);

    DWORD dwSize = size;

    if ( m_hKey )
    {
        checkError( CryptDecrypt( m_hKey,
                0,               // No hashing of data
                TRUE,            // Final or only buffer of data
                0,               // Must be zero
                data,             // Data buffer
                &dwSize         // Size of data
        ));         
    }

    return getErrorCode() == 0 ? 1 : 0;
}

int CRhoCryptImpl::set_db_CryptKey( const char* szPartition, const char* szKey, bool bPersistent )
{
    convertToStringW(szPartition, m_strDBPartition);

    int dwBlobLen = rho_base64_decode( szKey, -1, 0 );
    BYTE* pbKeyBlob = (BYTE*)malloc(dwBlobLen);
    rho_base64_decode(szKey, -1, (char*)pbKeyBlob);

    DATA_BLOB unprotectedKey = {dwBlobLen , pbKeyBlob };
    DATA_BLOB protectedKey = {0};
    checkError( CryptProtectData( &unprotectedKey, L"Rhodes data key", 0, 0, 0, 0, &protectedKey ) );

    if (pbKeyBlob)
        free(pbKeyBlob);

    saveKeyToStorage(protectedKey);

    if ( protectedKey.pbData )
        LocalFree(protectedKey.pbData);

    return 1;
}

}
}
