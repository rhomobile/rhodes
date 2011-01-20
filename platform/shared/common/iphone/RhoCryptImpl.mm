#import <Foundation/Foundation.h>

#include "RhoCryptImpl.h"
#include "common/RhodesApp.h"

#import <CommonCrypto/CommonDigest.h>
#import <CommonCrypto/CommonCryptor.h>

#define kChosenCipherBlockSize	kCCBlockSizeAES128
#define kChosenCipherKeySize	kCCKeySizeAES128
#define kChosenDigestLength		CC_SHA1_DIGEST_LENGTH

// (See cssmtype.h and cssmapple.h on the Mac OS X SDK.)

enum {
	CSSM_ALGID_NONE =					0x00000000L,
	CSSM_ALGID_VENDOR_DEFINED =			CSSM_ALGID_NONE + 0x80000000L,
	CSSM_ALGID_AES
};

namespace rho{
namespace common{
IMPLEMENT_LOGCLASS(CRhoCryptImpl,"RhoCrypt");
#define checkError(func) _checkError(func, #func)

CRhoCryptImpl::CRhoCryptImpl(void) : m_dbKeyData(NULL)
{
	m_dwLastError = 0;
}

CRhoCryptImpl::~CRhoCryptImpl(void)
{
    if ( m_dbKeyData ) 
        free(m_dbKeyData);
}
	
bool CRhoCryptImpl::_checkError( int32_t status, const char* szFunc )
{
    m_dwLastError = 0;
    if ( status == 0 )
        return true;
	
	String strErrorText;
	switch (status)
	{
		case kCCParamError:
			strErrorText = "kCCParamError";
			break;
		case kCCBufferTooSmall:
			strErrorText = "kCCBufferTooSmall";
			break;
		case kCCMemoryFailure:
			strErrorText = "kCCMemoryFailure";
			break;
		case kCCAlignmentError:
			strErrorText = "kCCAlignmentError";
			break;
		case kCCDecodeError:
			strErrorText = "kCCDecodeError";
			break;
		case kCCUnimplemented:
			strErrorText = "kCCDecodeError";
			break;
	}			
	
    LOG(ERROR) + szFunc + "failed with code: " + LOGFMT("0x%X") + m_dwLastError + ";Message: " + strErrorText;

    return false;
}

static NSData* getStorageKeyTag(const char* szDBPartition)
{
	NSString* strPartition = [[NSString alloc] initWithUTF8String:szDBPartition];
	NSString* strAppName = [[[NSBundle mainBundle] infoDictionary] objectForKey:@"CFBundleName"];
	NSString* strTagName = [NSString stringWithFormat: @"com.rhomobile.%@.%@.dbkey", strAppName, strPartition];
		
	NSData* resData = [strTagName dataUsingEncoding:NSUTF8StringEncoding];
		
	//[strTagName release];
	[strPartition release];
		
	return resData;
}
	
void CRhoCryptImpl::readKeyFromStorage()
{
	NSData*	storageKeyTag = getStorageKeyTag(m_strDBPartition.c_str());
	OSStatus sanityCheck = noErr;
	NSData * symmetricKeyReturn = nil;
	
	NSMutableDictionary * querySymmetricKey = [[NSMutableDictionary alloc] init];
	
	// Set the private key query dictionary.
	[querySymmetricKey setObject:(id)kSecClassKey forKey:(id)kSecClass];
	[querySymmetricKey setObject:storageKeyTag forKey:(id)kSecAttrApplicationTag];
	[querySymmetricKey setObject:[NSNumber numberWithUnsignedInt:CSSM_ALGID_AES] forKey:(id)kSecAttrKeyType];
	[querySymmetricKey setObject:[NSNumber numberWithBool:YES] forKey:(id)kSecReturnData];
	
	// Get the key bits.
	sanityCheck = SecItemCopyMatching((CFDictionaryRef)querySymmetricKey, (CFTypeRef *)&symmetricKeyReturn);
	if (sanityCheck == noErr && symmetricKeyReturn != nil && [symmetricKeyReturn length] == kChosenCipherKeySize) 
	{
		m_dbKeyData = (uint8_t *)malloc( kChosenCipherKeySize * sizeof(uint8_t) );
		memcpy( m_dbKeyData, [symmetricKeyReturn bytes], kChosenCipherKeySize );
	}
	if ( symmetricKeyReturn )
		[symmetricKeyReturn release];
	
	[querySymmetricKey release];
	
	//if ( storageKeyTag )
	//	[storageKeyTag release];
}

static void deleteKeyFromStorage(NSData* storageKeyTag)
{
	OSStatus sanityCheck = noErr;
	
	NSMutableDictionary * querySymmetricKey = [[NSMutableDictionary alloc] init];
		
	// Set the symmetric key query dictionary.
	[querySymmetricKey setObject:(id)kSecClassKey forKey:(id)kSecClass];
	[querySymmetricKey setObject:storageKeyTag forKey:(id)kSecAttrApplicationTag];
	[querySymmetricKey setObject:[NSNumber numberWithUnsignedInt:CSSM_ALGID_AES] forKey:(id)kSecAttrKeyType];
		
	// Delete the symmetric key.
	sanityCheck = SecItemDelete((CFDictionaryRef)querySymmetricKey);
	if ( sanityCheck != noErr && sanityCheck != errSecItemNotFound)
		LOG(ERROR) + "Error removing key, OSStatus: " + sanityCheck + "; errno: " + errno;
	
	[querySymmetricKey release];
}
	
void CRhoCryptImpl::writeKeyToStorage()
{
	OSStatus sanityCheck = noErr;
	NSData * symmetricKeyRef = NULL;	
	NSData*	storageKeyTag = getStorageKeyTag(m_strDBPartition.c_str());
	
	// First delete current symmetric key.
	deleteKeyFromStorage(storageKeyTag);
	
	// Container dictionary
	NSMutableDictionary *symmetricKeyAttr = [[NSMutableDictionary alloc] init];
	[symmetricKeyAttr setObject:(id)kSecClassKey forKey:(id)kSecClass];
	[symmetricKeyAttr setObject:storageKeyTag forKey:(id)kSecAttrApplicationTag];
	[symmetricKeyAttr setObject:[NSNumber numberWithUnsignedInt:CSSM_ALGID_AES] forKey:(id)kSecAttrKeyType];
	[symmetricKeyAttr setObject:[NSNumber numberWithUnsignedInt:(unsigned int)(kChosenCipherKeySize << 3)] forKey:(id)kSecAttrKeySizeInBits];
	[symmetricKeyAttr setObject:[NSNumber numberWithUnsignedInt:(unsigned int)(kChosenCipherKeySize << 3)]	forKey:(id)kSecAttrEffectiveKeySize];
	[symmetricKeyAttr setObject:(id)kCFBooleanTrue forKey:(id)kSecAttrCanEncrypt];
	[symmetricKeyAttr setObject:(id)kCFBooleanTrue forKey:(id)kSecAttrCanDecrypt];
	[symmetricKeyAttr setObject:(id)kCFBooleanFalse forKey:(id)kSecAttrCanDerive];
	[symmetricKeyAttr setObject:(id)kCFBooleanFalse forKey:(id)kSecAttrCanSign];
	[symmetricKeyAttr setObject:(id)kCFBooleanFalse forKey:(id)kSecAttrCanVerify];
	[symmetricKeyAttr setObject:(id)kCFBooleanFalse forKey:(id)kSecAttrCanWrap];
	[symmetricKeyAttr setObject:(id)kCFBooleanFalse forKey:(id)kSecAttrCanUnwrap];
	
	symmetricKeyRef = [[NSData alloc] initWithBytes:(const void *)m_dbKeyData length:kChosenCipherKeySize];
	
	// Add the wrapped key data to the container dictionary.
	[symmetricKeyAttr setObject:symmetricKeyRef forKey:(id)kSecValueData];
	
	// Add the symmetric key to the keychain.
	sanityCheck = SecItemAdd((CFDictionaryRef) symmetricKeyAttr, NULL);
	if ( sanityCheck != noErr && sanityCheck != errSecDuplicateItem)
		LOG(ERROR) + "Problem storing key in the keychain, OSStatus: " + sanityCheck + "; errno: " + errno;
	
	[symmetricKeyAttr release];
	[symmetricKeyRef release];
	//[storageKeyTag release];
}

void CRhoCryptImpl::generateNewKey()
{
	OSStatus sanityCheck = noErr;
	uint8_t * symmetricKey = NULL;
	
	// Allocate some buffer space. I don't trust calloc.
	symmetricKey = (uint8_t *)malloc( kChosenCipherKeySize * sizeof(uint8_t) );
	memset((void *)symmetricKey, 0x0, kChosenCipherKeySize);
	
	sanityCheck = SecRandomCopyBytes(kSecRandomDefault, kChosenCipherKeySize, symmetricKey);
	if ( sanityCheck != noErr )
	{
		free(symmetricKey);
		LOG(ERROR) + "Problem generating the key, OSStatus: " + sanityCheck + "; errno: " + errno;
	}
	else	
		m_dbKeyData = symmetricKey;
}
	
void CRhoCryptImpl::initContext(const char* szPartition)
{
    if ( m_dbKeyData )
        return;

    m_strDBPartition = szPartition;
	
	readKeyFromStorage();
    if ( m_dbKeyData )
        return;

	generateNewKey();
	writeKeyToStorage();
}

int CRhoCryptImpl::db_encrypt( const char* szPartition, int size, unsigned char* data, unsigned char* dataOut )
{
    initContext(szPartition);

    // Initialization vector; dummy in this case 0's.
    uint8_t iv[kChosenCipherBlockSize];
    memset((void *) iv, 0x0, (size_t) sizeof(iv));
	
	size_t movedBytes = 0;
	
	checkError( CCCrypt(  kCCEncrypt, kCCAlgorithmAES128,
					   0, //kCCOptionPKCS7Padding
					   (const void *)m_dbKeyData,
					   kChosenCipherKeySize,
					   iv,
					   (const void *) data,
					   size,
					   (void *)dataOut,
					   size,
					   &movedBytes
					   ) );
	
    return getErrorCode() == 0 ? 1 : 0;
}

int CRhoCryptImpl::db_decrypt( const char* szPartition, int size, unsigned char* data )
{
    initContext(szPartition);
	
    // Initialization vector; dummy in this case 0's.
    uint8_t iv[kChosenCipherBlockSize];
    memset((void *) iv, 0x0, (size_t) sizeof(iv));
	
	size_t movedBytes = 0;
	
	checkError( CCCrypt(  kCCDecrypt, kCCAlgorithmAES128,
									   0, //kCCOptionPKCS7Padding
									   (const void *)m_dbKeyData,
									   kChosenCipherKeySize,
									   iv,
									   (const void *) data,
									   size,
									   (void *)data,
									   size,
									   &movedBytes
									   ) );
	
    return getErrorCode() == 0 ? 1 : 0;
}

int CRhoCryptImpl::set_db_CryptKey( const char* szPartition, const char* szKey, bool bPersistent )
{
    m_strDBPartition = szPartition;

    int dwBlobLen = rho_base64_decode( szKey, -1, 0 );
    char* pbKeyBlob = (char*)malloc(dwBlobLen);
    rho_base64_decode(szKey, -1, (char*)pbKeyBlob);
	
	if ( m_dbKeyData )
	{
		free(m_dbKeyData);
		m_dbKeyData = NULL;
	}
	
	if ( dwBlobLen != kChosenCipherKeySize )
	{
		LOG(ERROR) + "Incorrect key size : " + dwBlobLen + "; Should be: " + (int)kChosenCipherKeySize;
		return 0;
	}
	m_dbKeyData = (uint8_t *)malloc( kChosenCipherKeySize * sizeof(uint8_t) );
	memcpy(m_dbKeyData, pbKeyBlob, kChosenCipherKeySize);
	
	if ( bPersistent )
		writeKeyToStorage();
	
    return 1;
}

}
}
