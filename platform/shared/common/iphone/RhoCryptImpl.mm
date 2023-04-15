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

#include "common/app_build_capabilities.h"


#ifdef APP_BUILD_CAPABILITY_IOS_CRYPTO_FORCE_AES_GCM
#import <rhoappbaselib-Swift.h>
RhoCryptAESGCM* swift_aes_gcm = nil;
#endif

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

static NSMutableDictionary* ourKeys = [NSMutableDictionary dictionaryWithCapacity:16];

CRhoCryptImpl::CRhoCryptImpl(void) : m_dbKeyData(NULL)
{
	m_dwLastError = 0;
	currentKeySize = kCCKeySizeAES256;
    
    
    
#ifdef APP_BUILD_CAPABILITY_IOS_CRYPTO_FORCE_AES_GCM
    swift_aes_gcm = [[RhoCryptAESGCM alloc] init];
#endif
    
    //[swift_aes_gcm run_crypto_test];
    //NSData* key_data = [swift_aes_gcm generate_new_key];
    //NSString* str = [key_data base64EncodedStringWithOptions:0];;
    //RAWLOG_INFO1("CALL SWIFT Object RESULT = [%s]", [str UTF8String]);
    //int o = 9;
}

CRhoCryptImpl::~CRhoCryptImpl(void)
{
    // keys now stored into ourKeys hash. For restore in case of DB was reset and reopen - new CRhoCryptImpl created during this process and new CRhoCryptImpl forget about key - so we store keys in hash partition -> key
    //if ( m_dbKeyData )
    //    free(m_dbKeyData);
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
    
    RAWLOG_INFO1("getStorageKeyTagOld() TAG = [%s]", [strTagName UTF8String]);
		
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

    RAWLOG_INFO1("getStorageKeyTagOld256() TAG = [%s]", [strTagName UTF8String]);

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
        
        RAWLOG_INFO1("getStorageKeyTagNew() TAG = [%s]", [strTagName UTF8String]);
        //NSLog(@"STORAGE_KEY_TAG = %@", strTagName);
        
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
        
       RAWLOG_INFO1("getStorageKeyTagNew256() TAG = [%s]", [strTagName UTF8String]);
       //NSLog(@"STORAGE_KEY_TAG = %@", strTagName);
        
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
    RAWLOG_INFO("CRhoCryptImpl::readKeyFromStorageOld() START");
	NSData*	storageKeyTag = getStorageKeyTagOld256(m_strDBPartition.c_str());
	OSStatus sanityCheck = noErr;
	NSData * symmetricKeyReturn = nil;
	
	NSMutableDictionary * querySymmetricKey = [[NSMutableDictionary alloc] init];

    
    const char* depEncrypt = get_app_build_config_item("use_deprecated_encryption");
    bool isUseDeprecatedEncryption = (depEncrypt && strcmp(depEncrypt, "1") == 0);
    
    
    if (isUseDeprecatedEncryption) {
        RAWLOG_INFO("CRhoCryptImpl::readKeyFromStorageOld() trye to read old firstly");
        sanityCheck = noErr;
        symmetricKeyReturn = nil;
        storageKeyTag = getStorageKeyTagOld(m_strDBPartition.c_str());
        initSymmetrickKeyOld(querySymmetricKey, storageKeyTag);
        sanityCheck = SecItemCopyMatching((CFDictionaryRef)querySymmetricKey, (CFTypeRef *)&symmetricKeyReturn);
        if(sanityCheck == noErr) {
            currentKeySize = kCCKeySizeAES128;
            RAWLOG_INFO("CRhoCryptImpl::readKeyFromStorageOld() old key is OK - currentKeySize = kCCKeySizeAES128");
        }
        else {
            RAWLOG_INFO("CRhoCryptImpl::readKeyFromStorageOld()     fail - > try to read new");
            storageKeyTag = getStorageKeyTagOld256(m_strDBPartition.c_str());
            sanityCheck = noErr;
            symmetricKeyReturn = nil;
            initSymmetrickKeyOld(querySymmetricKey, storageKeyTag);
            sanityCheck = SecItemCopyMatching((CFDictionaryRef)querySymmetricKey, (CFTypeRef *)&symmetricKeyReturn);
            if(sanityCheck == noErr) {
                RAWLOG_INFO("CRhoCryptImpl::readKeyFromStorageOld()      OK");
            }
        }
    }
    else {
        
        // Set the private key query dictionary.
        initSymmetrickKeyOld(querySymmetricKey, storageKeyTag);
        
        // Get the key bits.
        RAWLOG_INFO("CRhoCryptImpl::readKeyFromStorageOld() trye to read");
        sanityCheck = SecItemCopyMatching((CFDictionaryRef)querySymmetricKey, (CFTypeRef *)&symmetricKeyReturn);
        
        if(sanityCheck != noErr)
        {
            RAWLOG_INFO("CRhoCryptImpl::readKeyFromStorageOld()     fail - > try to read old");
            
            sanityCheck = noErr;
            symmetricKeyReturn = nil;
            storageKeyTag = getStorageKeyTagOld(m_strDBPartition.c_str());
            initSymmetrickKeyOld(querySymmetricKey, storageKeyTag);
            sanityCheck = SecItemCopyMatching((CFDictionaryRef)querySymmetricKey, (CFTypeRef *)&symmetricKeyReturn);
            if(sanityCheck == noErr) {
                currentKeySize = kCCKeySizeAES128;
                RAWLOG_INFO("CRhoCryptImpl::readKeyFromStorageOld() old key is OK - currentKeySize = kCCKeySizeAES128");
            }
        }
        else {
            RAWLOG_INFO("CRhoCryptImpl::readKeyFromStorageOld()      OK");
            
        }
    }

	if (sanityCheck == noErr && symmetricKeyReturn != nil && [symmetricKeyReturn length] == currentKeySize) 
	{
		m_dbKeyData = (uint8_t *)malloc( currentKeySize * sizeof(uint8_t) );
		memcpy( m_dbKeyData, [symmetricKeyReturn bytes], currentKeySize );
        RAWLOG_INFO1("CRhoCryptImpl::readKeyFromStorageOld() alloc and read key with size = %d", (int)currentKeySize);
	}
	if ( symmetricKeyReturn )
		[symmetricKeyReturn release];
	
	[querySymmetricKey release];
	
	//if ( storageKeyTag )
	//	[storageKeyTag release];
    RAWLOG_INFO("CRhoCryptImpl::readKeyFromStorageOld() FINISH");
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
    RAWLOG_INFO("CRhoCryptImpl::readKeyFromStorageNew() START");

        NSData*    storageKeyTag = getStorageKeyTagNew256(m_strDBPartition.c_str());
        OSStatus sanityCheck = noErr;
		
        NSData * symmetricKeyReturn = nil;
        NSArray* keychainItemReturn = nil;
        
        NSMutableDictionary * querySymmetricKey = [[NSMutableDictionary alloc] init];

    
        const char* depEncrypt = get_app_build_config_item("use_deprecated_encryption");
    bool isUseDeprecatedEncryption = (depEncrypt && strcmp(depEncrypt, "1") == 0);


        if (isUseDeprecatedEncryption) {
            // firstly check old key and use it if it exists !!!
            RAWLOG_INFO("CRhoCryptImpl::readKeyFromStorageNew() try to read old key firstly");

            sanityCheck = noErr;
            symmetricKeyReturn = nil;
            keychainItemReturn = nil;
            
            storageKeyTag = getStorageKeyTagNew(m_strDBPartition.c_str());
            initSymmetrickKeyNew(querySymmetricKey, storageKeyTag);
            sanityCheck = SecItemCopyMatching((CFDictionaryRef)querySymmetricKey, (CFTypeRef *)&keychainItemReturn);
            if(sanityCheck == noErr) {
                currentKeySize = kCCKeySizeAES128;
                RAWLOG_INFO("CRhoCryptImpl::readKeyFromStorageNew() old key is OK - currentKeySize = kCCKeySizeAES128");
            }
            else {
                RAWLOG_INFO("CRhoCryptImpl::readKeyFromStorageNew() fail - try to read new key");
                sanityCheck = noErr;
                symmetricKeyReturn = nil;
                keychainItemReturn = nil;
                
                storageKeyTag = getStorageKeyTagNew256(m_strDBPartition.c_str());
                initSymmetrickKeyNew(querySymmetricKey, storageKeyTag);
                sanityCheck = SecItemCopyMatching((CFDictionaryRef)querySymmetricKey, (CFTypeRef *)&keychainItemReturn);
                
            }

        }
        else {
            // firstly check new key and use it if it exists !!!
            RAWLOG_INFO("CRhoCryptImpl::readKeyFromStorageNew() try to read new key firstly");
            initSymmetrickKeyNew(querySymmetricKey, storageKeyTag);
            
            // Get the key bits.
            sanityCheck = SecItemCopyMatching((CFDictionaryRef)querySymmetricKey, (CFTypeRef *)&keychainItemReturn);
            if(sanityCheck != noErr) //try read old key
            {
                RAWLOG_INFO("CRhoCryptImpl::readKeyFromStorageNew() fail - try to read old key");
                
                sanityCheck = noErr;
                symmetricKeyReturn = nil;
                keychainItemReturn = nil;
                
                storageKeyTag = getStorageKeyTagNew(m_strDBPartition.c_str());
                initSymmetrickKeyNew(querySymmetricKey, storageKeyTag);
                sanityCheck = SecItemCopyMatching((CFDictionaryRef)querySymmetricKey, (CFTypeRef *)&keychainItemReturn);
                if(sanityCheck == noErr) {
                    currentKeySize = kCCKeySizeAES128;
                    RAWLOG_INFO("CRhoCryptImpl::readKeyFromStorageNew() old key is OK - currentKeySize = kCCKeySizeAES128");
                }
                
            }
        }
    
    
    
        if (sanityCheck == noErr && keychainItemReturn != nil )
        {
            symmetricKeyReturn = (NSData*)[keychainItemReturn objectAtIndex:0];
            if ([symmetricKeyReturn length] == currentKeySize) {
                RAWLOG_INFO1("CRhoCryptImpl::readKeyFromStorageNew() alloc and read key with size = %d", (int)currentKeySize);
                m_dbKeyData = (uint8_t *)malloc( currentKeySize * sizeof(uint8_t) );
                memcpy( m_dbKeyData, [symmetricKeyReturn bytes], currentKeySize );
            }
            else {
                RAWLOG_ERROR2("STORAGE SecItemCopyMatching() invalid key size = %d, should be = %d", (int)[symmetricKeyReturn length], (int)currentKeySize);
            }
        }
        else 
		{
			RAWLOG_ERROR("STORAGE SecItemCopyMatching() return  ERROR !");
            RAWLOG_ERROR1("STORAGE SecItemCopyMatching() sanityCheck = %d", (int)sanityCheck);
            
            if (sanityCheck == noErr && symmetricKeyReturn != nil) {
                //NSLog(@"STORAGE SecItemCopyMatching() symmetricKeyReturn = %@", symmetricKeyReturn);
                RAWLOG_ERROR1("STORAGE SecItemCopyMatching() symmetricKeyReturn length = %d", (int)[symmetricKeyReturn length]);
                RAWLOG_ERROR1("STORAGE SecItemCopyMatching() currentKeySize = %d", (int)currentKeySize);
            }
            
            
        }
        if ( symmetricKeyReturn )
            [symmetricKeyReturn release];
        
        [querySymmetricKey release];
    RAWLOG_INFO("CRhoCryptImpl::readKeyFromStorageNew() FINISH");

}
    
    

static void deleteKeyFromStorageOld(NSData* storageKeyTag)
{
    RAWLOG_INFO("deleteKeyFromStorageOld()");
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
    RAWLOG_INFO("deleteKeyFromStorageNew()");
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
    RAWLOG_INFO1("CRhoCryptImpl::writeKeyToStorageOld() currentKeySize = %d", (int)currentKeySize);
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
    RAWLOG_INFO1("CRhoCryptImpl::writeKeyToStorageNew() currentKeySize = %d", (int)currentKeySize);

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
    RAWLOG_INFO("CRhoCryptImpl::readKeyFromStorage() START");

    const char* depEncrypt = get_app_build_config_item("use_deprecated_encryption");
    bool isUseDeprecatedEncryption = (depEncrypt && strcmp(depEncrypt, "1") == 0);
    
    if (isUseDeprecatedEncryption) {
        // try to read old scheme key
        readKeyFromStorageOld();
        if ( m_dbKeyData ) {
            // save to new scheme for future
            RAWLOG_INFO("CRhoCryptImpl::readKeyFromStorage() old key loaded");
            writeKeyToStorageNew();
            return;
        }
        else {
            readKeyFromStorageNew();
            if ( m_dbKeyData ) {
                // found new scheme key - use it
                RAWLOG_INFO("CRhoCryptImpl::readKeyFromStorage() new key loaded");
                RAWLOG_INFO("CRhoCryptImpl::readKeyFromStorage() m_dbKeyData != NULL FINISH");
                return;
            }
        }

    }
    else {
        readKeyFromStorageNew();
        if ( m_dbKeyData ) {
            // found new scheme key - use it
            RAWLOG_INFO("CRhoCryptImpl::readKeyFromStorage() new key loaded");
            RAWLOG_INFO("CRhoCryptImpl::readKeyFromStorage() m_dbKeyData != NULL FINISH");
            return;
        }
        else {
            // try to read old scheme key
            readKeyFromStorageOld();
            if ( m_dbKeyData ) {
                // save to new scheme for future
                RAWLOG_INFO("CRhoCryptImpl::readKeyFromStorage() old key loaded");
                writeKeyToStorageNew();
            }
        }
    }
    
    if ( m_dbKeyData ) {
        RAWLOG_INFO("CRhoCryptImpl::readKeyFromStorage() m_dbKeyData != NULL FINISH");
    }
    else {
        RAWLOG_INFO("CRhoCryptImpl::readKeyFromStorage() m_dbKeyData == NULL FINISH");
    }
}

    
void CRhoCryptImpl::generateNewKey()
{
    
#ifdef APP_BUILD_CAPABILITY_IOS_CRYPTO_FORCE_AES_GCM
    
    if ([swift_aes_gcm is_aes_gcm_available]) {
        
        
        NSData* key_data = [swift_aes_gcm generate_new_key];
        uint8_t * symmetricKey = NULL;
        symmetricKey = (uint8_t *)malloc( currentKeySize * sizeof(uint8_t) );
        
        memcpy(symmetricKey, [key_data bytes], currentKeySize);
        
        m_dbKeyData = symmetricKey;
    }
    else
#endif
    {
        RAWLOG_INFO1("CRhoCryptImpl::generateNewKey() currentKeySize = %d", (int)currentKeySize);
        
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
}
	
void CRhoCryptImpl::initContext(const char* szPartition)
{

    RAWLOG_INFO1("CRhoCryptImpl::initContext() Partition = [%s]", szPartition);
    m_strDBPartition = szPartition;
    
    if (!m_dbKeyData) {
        // if DB was reopen again we restore stored key from hash by partition for avoid regenerate key in case of isNewInstallation
        NSObject* obj = [ourKeys objectForKey:[NSString stringWithUTF8String:szPartition]];
        if (obj != nil) {
            NSNumber* nsn = (NSNumber*)obj;
            m_dbKeyData = (unsigned char*)[nsn unsignedLongLongValue];
        }
    }

    RAWLOG_INFO1("CRhoCryptImpl::initContext() START m_strDBPartition = [%s]", m_strDBPartition.c_str());
	if(isNewInstallation && !m_dbKeyData)
	{
        RAWLOG_INFO("CRhoCryptImpl::initContext() (isNewInstallation && !m_dbKeyData) == true");
        RAWLOG_INFO("CRhoCryptImpl::initContext() remove all keys from storage");
	    deleteKeyFromStorageOld(getStorageKeyTagOld256(m_strDBPartition.c_str()));
		deleteKeyFromStorageOld(getStorageKeyTagOld(m_strDBPartition.c_str()));
		deleteKeyFromStorageNew(getStorageKeyTagNew256(m_strDBPartition.c_str()));
		deleteKeyFromStorageNew(getStorageKeyTagNew(m_strDBPartition.c_str()));
	}
    if ( m_dbKeyData ) {
        RAWLOG_INFO("CRhoCryptImpl::initContext() m_dbKeyData != NULL");
        RAWLOG_INFO("CRhoCryptImpl::initContext() FINISH");
        return;
    }

	
	readKeyFromStorage();
    if ( m_dbKeyData ) {
        RAWLOG_INFO("CRhoCryptImpl::initContext() m_dbKeyData != NULL");
        RAWLOG_INFO("CRhoCryptImpl::initContext() FINISH");
        return;
    }

	generateNewKey();
    
    {
        // store key for partition - key will be restored in future in case of DB will reopened
        NSNumber* nsn = [NSNumber numberWithUnsignedLongLong:(unsigned long long)m_dbKeyData];
        [ourKeys setObject:nsn forKey:[NSString stringWithUTF8String:szPartition]];
    }
    
    writeKeyToStorageOld();
	writeKeyToStorageNew();
    RAWLOG_INFO("CRhoCryptImpl::initContext() FINISH");
}

int CRhoCryptImpl::db_encrypt( const char* szPartition, int size, unsigned char* data, unsigned char* dataOut )
{
    initContext(szPartition);

#ifdef APP_BUILD_CAPABILITY_IOS_CRYPTO_FORCE_AES_GCM
    if ([swift_aes_gcm is_aes_gcm_available]) {
        NSData* page = [NSData dataWithBytes:(const void *)data length:sizeof(unsigned char)*size];
        NSData* key = [NSData dataWithBytes:(const void *)m_dbKeyData length:sizeof(unsigned char)*currentKeySize];
        
        NSData* encrypted = [swift_aes_gcm encrypt_page:page :key];
        if (encrypted == nil) {
            return 0;
        }
        
        memcpy(dataOut, [encrypted bytes], size);
        
        return 1;
    }
    else
#endif
    {
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
}

int CRhoCryptImpl::db_decrypt( const char* szPartition, int size, unsigned char* data )
{
    initContext(szPartition);
	
    
#ifdef APP_BUILD_CAPABILITY_IOS_CRYPTO_FORCE_AES_GCM
    
    if ([swift_aes_gcm is_aes_gcm_available]) {
        NSData* page = [NSData dataWithBytes:(const void *)data length:sizeof(unsigned char)*size];
        NSData* key = [NSData dataWithBytes:(const void *)m_dbKeyData length:sizeof(unsigned char)*currentKeySize];
        
        NSData* decrypted = [swift_aes_gcm decrypt_page:page :key];
        if (decrypted == nil) {
            return 0;
        }
        
        memcpy(data, [decrypted bytes], size);
        
        return 1;
    }
    else
#endif
    {
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
