//
//  RhoCryptAESGCM.swift
//  RhoAppBaseLib
//
//  Created by Dmitry Soldatenkov on 13.04.2023.
//

import Foundation
import os
import CryptoKit


@objc(RhoCryptAESGCM)
public class RhoCryptAESGCM : NSObject {
    
    @objc(generate_new_key)
    public func generate_new_key() -> Data? {
        if #available(iOS 13.0, *) {
            let key = SymmetricKey(size: .bits256)
            let key_data = key.withUnsafeBytes { Data(Array($0)) }
            return key_data
        }
        return nil;
    }
    
    @objc(encrypt_page:::::)
    public func encrypt_page(page_ptr : UnsafeMutablePointer<UInt8>, page_size : Int, key_ptr : UnsafeMutablePointer<UInt8>, key_size : Int, ptr : UnsafeMutablePointer<UInt8>) -> Bool {
        
        if #available(iOS 13.0, *) {
            
            // encrypt
            do {
                
                let page = Data(bytes: page_ptr, count: page_size)
                let key = Data(bytes: key_ptr, count: key_size)

                let data_to_encrypt : Data = page.subdata(in: 0 ..< (page.count-32) )
                let symm_key = SymmetricKey(data: key)
            
                let sealedBox = try AES.GCM.seal(data_to_encrypt, using: symm_key)
                
                var outputData : Data = sealedBox.ciphertext
                
                let nonce_data : Data = Data(sealedBox.nonce)
                let tag_data : Data = sealedBox.tag
                
                let random_4_bytes_data : Data = Data((0 ..< 4).map { _ in UInt8.random(in: UInt8.min ... UInt8.max) })
                
                outputData.append(nonce_data)
                outputData.append(random_4_bytes_data)
                outputData.append(tag_data)

                outputData.copyBytes(to: ptr, count: outputData.count)
                
                return true
            }
            catch {
                return false
            }

        }
        else {
            return false
        }
        
    }
    
    @objc(decrypt_page:::::)
    public func decrypt_page(page_ptr : UnsafeMutablePointer<UInt8>, page_size : Int, key_ptr : UnsafeMutablePointer<UInt8>, key_size : Int, ptr : UnsafeMutablePointer<UInt8>) -> Bool {
        
        
        if #available(iOS 13.0, *) {
            
            do {
                let page = Data(bytes: page_ptr, count: page_size)
                let key = Data(bytes: key_ptr, count: key_size)
                
                let data_to_decrypt : Data = page.subdata(in: 0 ..< (page.count-32) )
                
                let nonce_data : Data = page.subdata(in: (page.count-32) ..< (page.count-32+12))
                let tag_data : Data = page.subdata(in: (page.count-16) ..< (page.count))
                
                let nonce : AES.GCM.Nonce = try AES.GCM.Nonce(data: nonce_data)
                let symm_key = SymmetricKey(data: key)
                
                
                
                let sealedBoxRestored = try AES.GCM.SealedBox(nonce: nonce, ciphertext: data_to_decrypt, tag: tag_data)
                let decrypted_data : Data = try AES.GCM.open(sealedBoxRestored, using: symm_key)
                
                decrypted_data.copyBytes(to: ptr, count: decrypted_data.count)
                          
                return true
            }
            catch {
                return false
            }
        }
        else {
            return false
        }
        
    }
    
    
    @objc(is_aes_gcm_available)
    public func is_aes_gcm_available() -> Bool {
        if #available(iOS 13.0, *) {
            return true
        }
        return false
    }
    
}
    
