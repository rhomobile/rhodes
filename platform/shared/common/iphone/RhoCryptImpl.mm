/*------------------------------------------------------------------------
* (The MIT License)
* 
* Copyright (c) 2008-2011 Rhomobile, Inc.
* 
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
* 
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
* 
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
* THE SOFTWARE.
* 
* http://rhomobile.com
*------------------------------------------------------------------------*/

#import <Foundation/Foundation.h>

#include "RhoCryptImpl.h"
#include "common/RhodesApp.h"

#import <CommonCrypto/CommonDigest.h>
#import <CommonCrypto/CommonCryptor.h>

#define kChosenCipherBlockSize	kCCBlockSizeAES128
#define kChosenCipherKeySize	kCCKeySizeAES256
#define kChosenDigestLength		CC_SHA1_DIGEST_LENGTH

// (See cssmtype.h and cssmapple.h on the Mac OS X SDK.)

extern "C" {
bool isNewInstallation;
};

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
	currentKeySize = kCCKeySizeAES256;
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

static NSData* getStorageKeyTagOld(const char* szDBPartition)
{
	NSString* strPartition = [[NSString alloc] initWithUTF8String:szDBPartition];
	NSString* strAppName = [[[NSBundle mainBundle] infoDictionary] objectForKey:@"CFBundleName"];
	NSString* strTagName = [NSString stringWithFormat: @"com.rhomobile.%@.%@.dbkey", strAppName, strPartition];
		
	NSData* resData = [strTagName dataUsingEncoding:NSUTF8StringEncoding];
		
	//[strTagName release];
	[strPartition release];
		
	return resData;
}

static NSData* getStorageKeyTagOld256(const char* szDBPartition)
{
	NSString* strPartition = [[NSString alloc] initWithUTF8String:szDBPartition];
	NSString* strAppName = [[[NSBundle mainBundle] infoDictionary] objectForKey:@"CFBundleName"];
	NSString* strTagName = [NSString stringWithFormat: @"com.rhomobile.%@.%@.dbkey256", strAppName, strPartition];
		
	NSData* resData = [strTagName dataUsingEncoding:NSUTF8StringEncoding];
		
	//[strTagName release];
	[strPartition release];
		
	return resData;
}
    
static NSData* getStorageKeyTagNew(const char* szDBPartition)
{
        NSString* strPartition = [[NSString alloc] initWithUTF8String:szDBPartition];
        NSString* strAppName = [[[NSBundle mainBundle] infoDictionary] objectForKey:@"CFBundleIdentifier"];
        NSString* strTagName = [NSString stringWithFormat: @"com.tau.%@.%@.dbkey", strAppName, strPartition];
        
        //NSLog(@"$$$ STORAGE_KEY_TAG = %@", strTagName);
        
        NSData* resData = [strTagName dataUsingEncoding:NSUTF8StringEncoding];
        
        //[strTagName release];
        [strPartition release];
        
        return resData;
}

static NSData* getStorageKeyTagNew256(const char* szDBPartition)
{
        NSString* strPartition = [[NSString alloc] initWithUTF8String:szDBPartition];
        NSString* strAppName = [[[NSBundle mainBundle] infoDictionary] objectForKey:@"CFBundleIdentifier"];
        NSString* strTagName = [NSString stringWithFormat: @"com.tau.%@.%@.dbkey256", strAppName, strPartition];
        
        //NSLog(@"$$$ STORAGE_KEY_TAG = %@", strTagName);
        
        NSData* resData = [strTagName dataUsingEncoding:NSUTF8StringEncoding];
        
        //[strTagName release];
        [strPartition release];
        
        return resData;
}

void initSymmetrickKeyOld(NSMutableDictionary* querySymmetricKey, NSData* storageKeyTag)
{
	[querySymmetricKey setObject:(id)kSecClassKey forKey:(id)kSecClass];
	[querySymmetricKey setObject:storageKeyTag forKey:(id)kSecAttrApplicationTag];
	[querySymmetricKey setObject:[NSNumber numberWithUnsignedInt:CSSM_ALGID_AES] forKey:(id)kSecAttrKeyType];
	[querySymmetricKey setObject:[NSNumber numberWithBool:YES] forKey:(id)kSecReturnData];
}

void CRhoCryptImpl::readKeyFromStorageOld()
{
	NSData*	storageKeyTag = getStorageKeyTagOld256(m_strDBPartition.c_str());
	OSStatus sanityCheck = noErr;
	NSData * symmetricKeyReturn = nil;
	
	NSMutableDictionary * querySymmetricKey = [[NSMutableDictionary alloc] init];
	
	// Set the private key query dictionary.
	initSymmetrickKeyOld(querySymmetricKey, storageKeyTag);
	
	// Get the key bits.
	sanityCheck = SecItemCopyMatching((CFDictionaryRef)querySymmetricKey, (CFTypeRef *)&symmetricKeyReturn);

	if(sanityCheck != noErr)
	{
		sanityCheck = noErr;
		symmetricKeyReturn = nil;
		storageKeyTag = getStorageKeyTagOld(m_strDBPartition.c_str());
		initSymmetrickKeyOld(querySymmetricKey, storageKeyTag);
		sanityCheck = SecItemCopyMatching((CFDictionaryRef)querySymmetricKey, (CFTypeRef *)&symmetricKeyReturn);
		if(sanityCheck == noErr)
			    currentKeySize = kCCKeySizeAES128;
	}

	if (sanityCheck == noErr && symmetricKeyReturn != nil && [symmetricKeyReturn length] == currentKeySize) 
	{
		m_dbKeyData = (uint8_t *)malloc( currentKeySize * sizeof(uint8_t) );
		memcpy( m_dbKeyData, [symmetricKeyReturn bytes], currentKeySize );
	}
	if ( symmetricKeyReturn )
		[symmetricKeyReturn release];
	
	[querySymmetricKey release];
	
	//if ( storageKeyTag )
	//	[storageKeyTag release];
}
    

static void initSymmetrickKeyNew(NSMutableDictionary* querySymmetricKey, NSData* storageKeyTag)
{
	[querySymmetricKey setObject:(id)kSecMatchLimitAll forKey:(id)kSecMatchLimit];
        
    [querySymmetricKey       setObject:(__bridge id)kSecClassKey
                                forKey:(__bridge id)kSecClass];
        
    [querySymmetricKey       setObject:(__bridge id)kCFBooleanTrue
                                forKey:(__bridge id)kSecAttrIsPermanent];
        
    [querySymmetricKey       setObject:(__bridge id)kSecAttrAccessibleAfterFirstUnlockThisDeviceOnly
                                forKey:(__bridge id)kSecAttrAccessible];
        
    [querySymmetricKey       setObject:(__bridge id)kSecAttrKeyClassSymmetric
                                forKey:(__bridge id)kSecAttrKeyClass];
        
    [querySymmetricKey       setObject:(__bridge id)storageKeyTag
                                forKey:(__bridge id)kSecAttrApplicationTag];
        
    [querySymmetricKey       setObject:(__bridge id)kCFBooleanTrue
                                forKey:(__bridge id)kSecReturnData];
}

void CRhoCryptImpl::readKeyFromStorageNew()
{
        NSData*    storageKeyTag = getStorageKeyTagNew256(m_strDBPartition.c_str());
        OSStatus sanityCheck = noErr;
		
        NSData * symmetricKeyReturn = nil;
        NSArray* keychainItemReturn = nil;
        
        NSMutableDictionary * querySymmetricKey = [[NSMutableDictionary alloc] init];
        
        initSymmetrickKeyNew(querySymmetricKey, storageKeyTag);
        
        // Get the key bits.
        sanityCheck = SecItemCopyMatching((CFDictionaryRef)querySymmetricKey, (CFTypeRef *)&keychainItemReturn);
		if(sanityCheck != noErr) //try read old key
		{
			sanityCheck = noErr;
			symmetricKeyReturn = nil;
			keychainItemReturn = nil;

			storageKeyTag = getStorageKeyTagNew(m_strDBPartition.c_str());
			initSymmetrickKeyNew(querySymmetricKey, storageKeyTag);
			sanityCheck = SecItemCopyMatching((CFDictionaryRef)querySymmetricKey, (CFTypeRef *)&keychainItemReturn);
			if(sanityCheck == noErr)
			    currentKeySize = kCCKeySizeAES128;
		}
		
        if (sanityCheck == noErr && keychainItemReturn != nil )
        {
            symmetricKeyReturn = (NSData*)[keychainItemReturn objectAtIndex:0];
            if ([symmetricKeyReturn length] == currentKeySize) {
                m_dbKeyData = (uint8_t *)malloc( currentKeySize * sizeof(uint8_t) );
                memcpy( m_dbKeyData, [symmetricKeyReturn bytes], currentKeySize );
            }
            else {
                RAWLOG_ERROR2("$$$ STORAGE SecItemCopyMatching() invalid key size = %d, should be = %d", (int)[symmetricKeyReturn length], (int)currentKeySize);
            }
        }
        else 
		{
			RAWLOG_ERROR("$$$ STORAGE SecItemCopyMatching() return  ERROR !");
            RAWLOG_ERROR1("$$$ STORAGE SecItemCopyMatching() sanityCheck = %d", (int)sanityCheck);
            
            if (sanityCheck == noErr && symmetricKeyReturn != nil) {
                //NSLog(@"$$$ STORAGE SecItemCopyMatching() symmetricKeyReturn = %@", symmetricKeyReturn);
                RAWLOG_ERROR1("$$$ STORAGE SecItemCopyMatching() symmetricKeyReturn length = %d", (int)[symmetricKeyReturn length]);
                RAWLOG_ERROR1("$$$ STORAGE SecItemCopyMatching() currentKeySize = %d", (int)currentKeySize);
            }
            
            
        }
        if ( symmetricKeyReturn )
            [symmetricKeyReturn release];
        
        [querySymmetricKey release];
        
}
    
    

static void deleteKeyFromStorageOld(NSData* storageKeyTag)
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
    
static void deleteKeyFromStorageNew(NSData* storageKeyTag)
{
        OSStatus sanityCheck = noErr;
        
        NSMutableDictionary * querySymmetricKey = [[NSMutableDictionary alloc] init];
    
        [querySymmetricKey       setObject:(__bridge id)kSecClassKey
                                    forKey:(__bridge id)kSecClass];
        
        [querySymmetricKey       setObject:(__bridge id)kCFBooleanTrue
                                    forKey:(__bridge id)kSecAttrIsPermanent];
        
        [querySymmetricKey       setObject:(__bridge id)kSecAttrAccessibleAfterFirstUnlockThisDeviceOnly
                                    forKey:(__bridge id)kSecAttrAccessible];
        
        [querySymmetricKey       setObject:(__bridge id)kSecAttrKeyClassSymmetric
                                    forKey:(__bridge id)kSecAttrKeyClass];
        
        [querySymmetricKey       setObject:(__bridge id)storageKeyTag
                                    forKey:(__bridge id)kSecAttrApplicationTag];
    
        // Delete the symmetric key.
        sanityCheck = SecItemDelete((CFDictionaryRef)querySymmetricKey);
        if ( sanityCheck != noErr && sanityCheck != errSecItemNotFound)
            LOG(ERROR) + "Error removing key, OSStatus: " + sanityCheck + "; errno: " + errno;
        
        [querySymmetricKey release];
}
    
    
	
void CRhoCryptImpl::writeKeyToStorageOld()
{
	OSStatus sanityCheck = noErr;
	NSData * symmetricKeyRef = NULL;	
	NSData*	storageKeyTag = currentKeySize == kCCKeySizeAES256 ? 
	    getStorageKeyTagOld256(m_strDBPartition.c_str()) :
		getStorageKeyTagOld(m_strDBPartition.c_str());
	
	// First delete current symmetric key.
	deleteKeyFromStorageOld(storageKeyTag);
	
	// Container dictionary
	NSMutableDictionary *symmetricKeyAttr = [[NSMutableDictionary alloc] init];
	[symmetricKeyAttr setObject:(id)kSecClassKey forKey:(id)kSecClass];
	[symmetricKeyAttr setObject:storageKeyTag forKey:(id)kSecAttrApplicationTag];
	[symmetricKeyAttr setObject:[NSNumber numberWithUnsignedInt:CSSM_ALGID_AES] forKey:(id)kSecAttrKeyType];
	[symmetricKeyAttr setObject:[NSNumber numberWithUnsignedInt:(unsigned int)(currentKeySize << 3)] forKey:(id)kSecAttrKeySizeInBits];
	[symmetricKeyAttr setObject:[NSNumber numberWithUnsignedInt:(unsigned int)(currentKeySize << 3)]	forKey:(id)kSecAttrEffectiveKeySize];
	[symmetricKeyAttr setObject:(id)kCFBooleanTrue forKey:(id)kSecAttrCanEncrypt];
	[symmetricKeyAttr setObject:(id)kCFBooleanTrue forKey:(id)kSecAttrCanDecrypt];
	[symmetricKeyAttr setObject:(id)kCFBooleanFalse forKey:(id)kSecAttrCanDerive];
	[symmetricKeyAttr setObject:(id)kCFBooleanFalse forKey:(id)kSecAttrCanSign];
	[symmetricKeyAttr setObject:(id)kCFBooleanFalse forKey:(id)kSecAttrCanVerify];
	[symmetricKeyAttr setObject:(id)kCFBooleanFalse forKey:(id)kSecAttrCanWrap];
	[symmetricKeyAttr setObject:(id)kCFBooleanFalse forKey:(id)kSecAttrCanUnwrap];
	
	symmetricKeyRef = [[NSData alloc] initWithBytes:(const void *)m_dbKeyData length:currentKeySize];
	
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

    
void CRhoCryptImpl::writeKeyToStorageNew()
{
        OSStatus sanityCheck = noErr;
        NSData * symmetricKeyRef = NULL;
        NSData*    storageKeyTag = currentKeySize == kCCKeySizeAES256 ? 
		    getStorageKeyTagNew256(m_strDBPartition.c_str()) :
			getStorageKeyTagNew(m_strDBPartition.c_str());
        
        // First delete current symmetric key.
        deleteKeyFromStorageNew(storageKeyTag);
        
        // Container dictionary
        NSMutableDictionary *symmetricKeyAttr = [[NSMutableDictionary alloc] init];
        
        symmetricKeyRef = [[NSData alloc] initWithBytes:(const void *)m_dbKeyData length:currentKeySize];
        
        [symmetricKeyAttr       setObject:(__bridge id)kSecClassKey
                                   forKey:(__bridge id)kSecClass];
        
        [symmetricKeyAttr       setObject:(__bridge id)kCFBooleanTrue
                                   forKey:(__bridge id)kSecAttrIsPermanent];
        
        [symmetricKeyAttr       setObject:(__bridge id)kSecAttrAccessibleAfterFirstUnlockThisDeviceOnly
                                   forKey:(__bridge id)kSecAttrAccessible];
        
        [symmetricKeyAttr       setObject:(__bridge id)kSecAttrKeyClassSymmetric
                                   forKey:(__bridge id)kSecAttrKeyClass];
        
        [symmetricKeyAttr       setObject:(__bridge id)storageKeyTag
                                   forKey:(__bridge id)kSecAttrApplicationTag];
        
        [symmetricKeyAttr       setObject:(__bridge id)symmetricKeyRef
                                   forKey:(__bridge id)kSecValueData];
        
        // Add the symmetric key to the keychain.
        sanityCheck = SecItemAdd((CFDictionaryRef) symmetricKeyAttr, NULL);
        if ( sanityCheck != noErr && sanityCheck != errSecDuplicateItem)
            LOG(ERROR) + "Problem storing key in the keychain, OSStatus: " + sanityCheck + "; errno: " + errno;
        
        [symmetricKeyAttr release];
        [symmetricKeyRef release];
}
    
void CRhoCryptImpl::readKeyFromStorage() {
    readKeyFromStorageNew();
    if ( m_dbKeyData ) {
        // found new scheme key - use it
        return;
    }
    else {
        // try to read old scheme key
        readKeyFromStorageOld();
        if ( m_dbKeyData ) {
            // save to new scheme for future
            writeKeyToStorageNew();
        }
    }
}

    
void CRhoCryptImpl::generateNewKey()
{
	OSStatus sanityCheck = noErr;
	uint8_t * symmetricKey = NULL;
	
	// Allocate some buffer space. I don't trust calloc.
	symmetricKey = (uint8_t *)malloc( currentKeySize * sizeof(uint8_t) );
	memset((void *)symmetricKey, 0x0, currentKeySize);
	
	sanityCheck = SecRandomCopyBytes(kSecRandomDefault, currentKeySize, symmetricKey);
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
	if(isNewInstallation && !m_dbKeyData)
	{
	    deleteKeyFromStorageOld(getStorageKeyTagOld256(m_strDBPartition.c_str()));
		deleteKeyFromStorageOld(getStorageKeyTagOld(m_strDBPartition.c_str()));
		deleteKeyFromStorageNew(getStorageKeyTagNew256(m_strDBPartition.c_str()));
		deleteKeyFromStorageNew(getStorageKeyTagNew(m_strDBPartition.c_str()));
	}
    if ( m_dbKeyData )
        return;

    m_strDBPartition = szPartition;
	
	readKeyFromStorage();
    if ( m_dbKeyData )
        return;

	generateNewKey();
    writeKeyToStorageOld();
	writeKeyToStorageNew();
}

int CRhoCryptImpl::db_encrypt( const char* szPartition, int size, unsigned char* data, unsigned char* dataOut )
{
    initContext(szPartition);

    // Initialization vector; dummy in this case 0's.
    uint8_t iv[kChosenCipherBlockSize];
    memset((void *) iv, 0x0, (size_t) sizeof(iv));
	
	size_t movedBytes = 0;
	
	checkError( CCCrypt(  kCCEncrypt, kCCAlgorithmAES,
					   0, //kCCOptionPKCS7Padding
					   (const void *)m_dbKeyData,
					   currentKeySize,
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
	
	checkError( CCCrypt(  kCCDecrypt, kCCAlgorithmAES,
									   0, //kCCOptionPKCS7Padding
									   (const void *)m_dbKeyData,
									   currentKeySize,
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
	
	if ( dwBlobLen != currentKeySize )
	{
		LOG(ERROR) + "Incorrect key size : " + dwBlobLen + "; Should be: " + (int)currentKeySize;
		return 0;
	}
	m_dbKeyData = (uint8_t *)malloc( currentKeySize * sizeof(uint8_t) );
	memcpy(m_dbKeyData, pbKeyBlob, currentKeySize);
	
    if ( bPersistent ){
        writeKeyToStorageOld();
		writeKeyToStorageNew();
    }
	
    return 1;
}

}
}