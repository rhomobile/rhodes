module Rho

    
class NdefRecord
    
    # 3-bit TNF (Type Name Format) field: Indicates how to interpret the type field
    
    TNF_ABSOLUTE_URI = 3
    TNF_EMPTY = 0
    TNF_EXTERNAL_TYPE = 4
    TNF_MIME_MEDIA = 2
    TNF_UNCHANGED = 6
    TNF_UNKNOWN = 5
    TNF_WELL_KNOWN = 1
    
    
    # RTD Text types. For use with TNF_WELL_KNOWN
    
    RTD_TEXT = [0x54]   # "T"
    RTD_URI = [0x55] # "U"
    RTD_SMART_POSTER = [0x53, 0x70] # "Sp"
    RTD_ALTERNATIVE_CARRIER = [0x61, 0x63]   # "ac"
    RTD_HANDOVER_CARRIER = [0x48, 0x63] # "Hc"
    RTD_HANDOVER_REQUEST = [0x48, 0x72] # "Hr"
    RTD_HANDOVER_SELECT = [0x48, 0x73] # "Hs"
    
   ID = 'id'
   TNF = 'tnf'
   TYPE = 'type'
   PAYLOAD = 'payload' 
    
   def init_from_byte_array(array)
       @byte_array = array
       hash = Nfc.convert_byte_array_to_NdeRecord_hash(@byte_array)
       @id = hash[ID]
       @tnf = hash[TNF]
       @type = hash[TYPE]
       @payload = hash[PAYLOAD]
       @payload_as_string = hash['payload_as_string']
   end
    
   # hash :
   #       'id' - byte[]
   #       'tnf' - int
   #       'type' - byte[]
   #       'payload' - byte[]
   def init_from_hash(hash)
       byte_arr = Nfc.convert_NdeRecord_hash_to_byte_array(hash)
       init_from_byte_array(byte_arr)
   end    
    
   # return byte[] 
   def get_id
       return @id
   end 
    
   # return int 
   def get_tnf
       return @tnf
   end    
   
   # return byte[] 
   def get_type
       return @type
   end    
   
   # return byte[]  
   def get_payload
       return @payload
   end    

   # return string   
   def get_payload_as_string
       return @payload_as_string
   end    

   # return raw byte[]  
   def get_byte_array
       return @byte_array
   end
   
   # return hash 
   # hash :
   #       'id' - byte[]
   #       'tnf' - int
   #       'type' - byte[]
   #       'payload' - byte[]
   #       'payload_as_string' - string
   def make_hash
       hash = { 'id' => @id, 'tnf' => @tnf, 'type' => @type, 'payload' => @payload, 'payload_as_string' => @payload_as_string}
       return hash
   end    
    
  # convert int tnf to string
  def self.convert_Tnf_to_string(tnf)
       res = 'unknown'
       if tnf == TNF_ABSOLUTE_URI
            res = 'TNF_ABSOLUTE_URI'
       end
       if tnf == TNF_EMPTY
            res = 'TNF_EMPTY'
       end
       if tnf == TNF_EXTERNAL_TYPE
            res = 'TNF_EXTERNAL_TYPE'
       end
       if tnf == TNF_MIME_MEDIA
            res = 'TNF_MIME_MEDIA'
       end
       if tnf == TNF_UNCHANGED
            res = 'TNF_UNCHANGED'
       end
       if tnf == TNF_UNKNOWN
            res = 'TNF_UNKNOWN'
       end
       if tnf == TNF_WELL_KNOWN
            res = 'TNF_WELL_KNOWN'
       end
       return res
  end

  # convert byte[] type to string
  def self.convert_RTD_to_string(rtd)
       res = 'unknown'
       if rtd == RTD_TEXT
            res = 'RTD_TEXT'
       end
       if rtd == RTD_URI
            res = 'RTD_URI'
       end
       if rtd == RTD_SMART_POSTER
            res = 'RTD_SMART_POSTER'
       end
       if rtd == RTD_ALTERNATIVE_CARRIER
            res = 'RTD_ALTERNATIVE_CARRIER'
       end
       if rtd == RTD_HANDOVER_CARRIER
            res = 'RTD_HANDOVER_CARRIER'
       end
       if rtd == RTD_HANDOVER_REQUEST
            res = 'RTD_HANDOVER_REQUEST'
       end
       if rtd == RTD_HANDOVER_SELECT
            res = 'RTD_HANDOVER_SELECT'
       end
       return res
  end

end    
    

class NdefMessage
   
   def init_from_byte_array(array)
       
       @byte_array = array
       @records = []
       
       record_array = Nfc.convert_NdeMessage_byte_array_to_NdeRecords_array(@byte_array)
       i = 0
       while i < record_array.size do
           record_ba = record_array[i]
           
           rec = NdefRecord.new
           rec.init_from_byte_array(record_ba)
           
           @records << rec
           
           i = i+1
       end    
       
   end
    
   def init_from_array_of_NdefRecord(array)
       
       record_array = []
       
       i = 0
       
       while i < array.size do
           rec = array[i]
           
           record_array << rec.get_byte_array
       
           i = i+1
       end    
           
       msg_array = Nfc.convert_NdeRecords_array_to_NdeMessage_byte_array(record_array)   
           
       init_from_byte_array(msg_array)    
       
   end 
    
   # return raw byte[]   
   def get_byte_array
       return @byte_array
   end

   # return array of NdefRecord
   def get_records
       return @records
   end    
    
    
end    
    
    

class NFCTagTechnology

  ISODEP = 'IsoDep'
  MIFARE_CLASSIC = 'MifareClassic'
  MIFARE_ULTRALIGHT = 'MifareUltralight'
  NDEF = 'Ndef'
  NDEF_FORMATABLE = 'NdefFormatable'
  NFCA = 'NfcA'
  NFCB = 'NfcB'
  NFCF = 'NfcF'
  NFCV = 'NfcV'

  def initialize(tech_name)
       @techname = tech_name
  end

  # return string    
  def get_name
      return @techname
  end

  # connect - only after it you can make any I/O operation     
  def connect
      Nfc.tech_connect(get_name)
  end

  # close connect - make it after your I/O operations     
  def close
      Nfc.tech_close(get_name)
  end

  # return true if tech connected and ready for I/O operations     
  def is_connected
      res = Nfc.tech_is_connected(@techname)
      resb = false
      if res != 0
          resb = true
      end    
      return resb
  end

end

    
class NFCTagTechnology_NfcA < NFCTagTechnology
    
    def initialize
        super(NFCTagTechnology::NFCA)
    end
    
    # return byte[]
    def get_Atqa
        return Nfc.tech_NfcA_get_Atqa
    end    

    # return int
    def get_Sak
        return Nfc.tech_NfcA_get_Sak
    end
    
    # data - byte[]
    # return byte[]
    def transceive(data)
        return Nfc.tech_NfcA_transceive(data)
    end    
    
end    

class NFCTagTechnology_Ndef < NFCTagTechnology
        
    MIFARE_CLASSIC = 'com.nxp.ndef.mifareclassic'
    NFC_FORUM_TYPE_1 = 'org.nfcforum.ndef.type1'
    NFC_FORUM_TYPE_2 = 'org.nfcforum.ndef.type2'
    NFC_FORUM_TYPE_3 = 'org.nfcforum.ndef.type3'
    NFC_FORUM_TYPE_4 = 'org.nfcforum.ndef.type4'
    
    def initialize
        super(NFCTagTechnology::NDEF)
    end
        
    # return int
    def get_max_size
        return Nfc.tech_Ndef_get_max_size
    end    
    
    # return bool
    def is_writable
        return (Nfc.tech_Ndef_is_writable != 0)
    end    
    
    # return bool
    def can_make_read_only
        return (Nfc.tech_Ndef_can_make_read_only != 0)
    end    
    
    # return bool
    def make_read_only
        return (Nfc.tech_Ndef_make_read_only != 0)
    end    
    
    # return string
    def get_type
        return Nfc.tech_Ndef_get_type
    end    
        
    # return NdefMessage
    def read_NdefMessage
        msg_ar =  Nfc.tech_Ndef_read_Nde_message
        msg = NdefMessage.new
        msg.init_from_byte_array(msg_ar)
        return msg
    end    
    
    # msg - NdefMessage
    def write_NdefMessage(msg)
        Nfc.tech_Ndef_write_Nde_message(msg.get_byte_array)
    end
  
        
end    
    
    
class NFCTagTechnology_MifareClassic < NFCTagTechnology

    KEY_DEFAULT = [0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF]
    
    KEY_MIFARE_APPLICATION_DIRECTORY = [0xA0, 0xA1, 0xA2, 0xA3, 0xA4, 0xA5]
    
    KEY_NFC_FORUM =[0xD3, 0xF7, 0xD3, 0xF7, 0xD3, 0xF7]    
    
    TYPE_CLASSIC = 0
    TYPE_PLUS = 1
    TYPE_PRO = 2
    TYPE_UNKNOWN = -1
    
  def initialize
       super(NFCTagTechnology::MIFARE_CLASSIC)
  end

   # return int type type 
   def get_type
       return Nfc.tech_MifareClassic_get_type
   end 
   
   # return named type (for known types) 
   def self.convert_type_to_string(type)
       res = 'unknown'
       if type == TYPE_CLASSIC
           res = 'TYPE_CLASSIC'
       end
       if type == TYPE_PLUS
           res = 'TYPE_PLUS'
       end
       if type == TYPE_PRO
           res = 'TYPE_PRO'
       end
       if type == TYPE_UNKNOWN
           res = 'TYPE_UNKNOWN'
       end
       return res
   end    
    
   # index - integer
   # block - 16 byte array
   def  write_block(index, block)
       Nfc.tech_MifareClassic_write_block(index, block)
   end

   # index - integer
   # return 16 byte array
   def read_block(index)
       return Nfc.tech_MifareClassic_read_block(index)
   end

   # return size in bytes 
   def get_size
       return Nfc.tech_MifareClassic_get_size
   end

   #return number of blocks
   def get_block_count
       return Nfc.tech_MifareClassic_get_block_count
   end

   # return count of sectors 
   def get_sector_count
       return Nfc.tech_MifareClassic_get_sector_count
   end    
    
   # return count of blocks in sector 
   def get_blocks_in_sector_count(index)
       return Nfc.tech_MifareClassic_get_blocks_in_sector_count(index)
   end    

    # return index of first block in sector 
    def sector_to_block(index)
        return Nfc.tech_MifareClassic_sector_to_block(index)
    end    
           
    # authenticate sector with key
    # key is 6 byte array
    # return true if authenticate was passed
    def authenticate_sector_with_key_A(index, key)
        r = Nfc.tech_MifareClassic_authenticate_sector_with_key_A(index, key)
        return r!=0
    end   
       
    # authenticate sector with key
    # key is 6 byte array
    # return true if authenticate was passed
    def authenticate_sector_with_key_B(index, key)
        r = Nfc.tech_MifareClassic_authenticate_sector_with_key_B(index, key)
        return r!=0
    end   
   
    # send data (byte array) to Tag and receive result - byte array
    def transceive(data)
        return Nfc.tech_MifareClassic_transceive(data)
    end    
    
end

class NFCTagTechnology_MifareUltralight < NFCTagTechnology

   TYPE_ULTRALIGHT = 1
   TYPE_ULTRALIGHT_C = 2
   TYPE_UNKNOWN = -1

   def initialize
       super(NFCTagTechnology::MIFARE_ULTRALIGHT)
   end

   # return int
   def get_type
      return Nfc.tech_MifareUltralight_get_type
   end

   # index - integer
   # block - 4 byte array
   def  write_page(index, block)
       Nfc.tech_MifareUltralight_write_page(index, block)
   end

   # index - integer
   # return 16 byte array
   def read_pages(index)
       return Nfc.tech_MifareUltralight_read_pages(index)
   end

    # send data (byte array) to Tag and receive result - byte array
    def transceive(data)
        return Nfc.tech_MifareUltralight_transceive(data)
    end    

end


class NFCTagTechnology_IsoDep < NFCTagTechnology
    
    def initialize
        super(NFCTagTechnology::ISODEP)
    end
    
    # return byte[]
    def get_hi_layer_responce
        return Nfc.tech_IsoDep_get_hi_layer_responce
    end    

    # return byte[]
    def get_historical_bytes
        return Nfc.tech_IsoDep_get_historical_bytes
    end    


    # timeout - int
    def set_timeout(timeout)
        return Nfc.tech_IsoDep_set_timeout(timeout)
    end
    
    # data - byte[]
    # return byte[]
    def transceive(data)
        return Nfc.tech_IsoDep_transceive(data)
    end    
    
end    


class NFCTagTechnology_NdefFormatable < NFCTagTechnology
        
    
    def initialize
        super(NFCTagTechnology::NDEF_FORMATABLE)
    end
        
    # msg - NdefMessage
    def format(msg)
        Nfc.tech_NdefFormatable_format(msg.get_byte_array)
    end
  
    # msg - NdefMessage
    def format_read_only(msg)
        Nfc.tech_NdefFormatable_format_read_only(msg.get_byte_array)
    end
        
end    


class NFCTagTechnology_NfcB < NFCTagTechnology
    
    def initialize
        super(NFCTagTechnology::NFCB)
    end
    
    # return byte[]
    def get_application_data
        return Nfc.tech_NfcB_get_application_data
    end    

    # return byte[]
    def get_protocol_info
        return Nfc.tech_NfcB_get_protocol_info
    end    


    # data - byte[]
    # return byte[]
    def transceive(data)
        return Nfc.tech_NfcB_transceive(data)
    end    
    
end    


class NFCTagTechnology_NfcF < NFCTagTechnology
    
    def initialize
        super(NFCTagTechnology::NFCF)
    end
    
    # return byte[]
    def get_manufacturer
        return Nfc.tech_NfcF_get_manufacturer
    end    

    # return byte[]
    def get_system_code
        return Nfc.tech_NfcF_get_system_code
    end    


    # data - byte[]
    # return byte[]
    def transceive(data)
        return Nfc.tech_NfcF_transceive(data)
    end    
    
end    


class NFCTagTechnology_NfcV < NFCTagTechnology
    
    def initialize
        super(NFCTagTechnology::NFCV)
    end
    
    # return int
    def get_dsf_id
        return Nfc.tech_NfcV_get_dsf_id
    end    

    # return int
    def get_responce_flags
        return Nfc.tech_NfcV_get_responce_flags
    end    


    # data - byte[]
    # return byte[]
    def transceive(data)
        return Nfc.tech_NfcV_transceive(data)
    end    
    
end    



class NFCTag

  def initialize(tech_list)
      #puts 'Ruby NFCTag.initialize()'
     @techlist = tech_list
  end

  # return array of string
  def get_tech_list
      return @techlist
  end  

  def get_ID
      return Nfc.tag_get_id
  end    
    
  def  get_tech(tech_name)
      if @techlist.include?(tech_name)
          if tech_name == 'MifareClassic'
              return NFCTagTechnology_MifareClassic.new()
          end
          if tech_name == 'MifareUltralight'
              return NFCTagTechnology_MifareULtralight.new()
          end
          if tech_name == 'Ndef'
              return NFCTagTechnology_Ndef.new()
          end
          if tech_name == 'NfcA'
              return NFCTagTechnology_NfcA.new()
          end
          if tech_name == 'IsoDep'
              return NFCTagTechnology_IsoDep.new()
          end
          if tech_name == 'NdefFormatable'
              return NFCTagTechnology_NdefFormatable.new()
          end
          if tech_name == 'NfcB'
              return NFCTagTechnology_NfcB.new()
          end
          if tech_name == 'NfcF'
              return NFCTagTechnology_NfcF.new()
          end
          if tech_name == 'NfcV'
              return NFCTagTechnology_NfcV.new()
          end
      else
          return nil
      end
  end

end




 
class NFCManager
 

  # return true/false
  def self.is_supported
       s = Nfc.is_supported
       return s != 0
  end
  
  def self.enable
      Nfc.enable(1)
  end

  def self.disable
      Nfc.enable(0)
  end

  # return true/false
  def self.is_enabled
      e = Nfc.is_enabled
      return e != 0
  end

  # in callback
  # @params['messages'] - array of messages (each message is hash)
  # message hash items : 
  #       'raw_message' - array of bytes (raw message)
  #       'records' - array of records (each record is hash)
  #              record hash items :
  #                     'raw_record' - array of bytes (raw record)
  #                     'id' - array of bytes
  #                     'payload' - array of bytes
  #                     'tnf' - int
  #                     'type' - array of bytes 
  #                     'payload_as_string' - string, payload prepared to string (support specail formats for URI, TEXT) 
  #                     'subrecords' - message hash (only for SMART_POSTER type)
  
  # set callback for tech events (Tag discovered/lost, tech special events)
  def self.set_nfc_callback(callback_url)
       Nfc.set_callback(callback_url)
  end

  # in callback
  # @params['Tag_event'] - 'discovered'
  def self.set_nfc_tech_callback(callback_url)
       Nfc.set_tech_callback(callback_url)
  end

  # return current discovered Tag or nil if not any Tag discovered now
  def self.get_current_Tag
       tech_list = Nfc.get_tech_list
       if tech_list.size == 0
           return nil
       end
       return NFCTag.new(tech_list)
  end

  # ndef_message - NdefMessage
  # start push NdefMessage to any receivers
  def self.p2p_enable_foreground_nde_push(ndef_message)
      Nfc.p2p_enable_foreground_nde_push(ndef_message.get_byte_array)
  end    

  # stop any push NdefMessage to receivers
  def self.p2p_disable_foreground_nde_push
      Nfc.p2p_disable_foreground_nde_push
  end    


  # make NdefRecord from byte[]
  def self.make_NdefRecord_from_byte_array(array)
     rec = NdefRecord.new
     rec.init_from_byte_array(array) 
     return rec 
  end    

  # make NdefRecord from hash
  # hash :
  #       'id' - byte[]
  #       'tnf' - int
  #       'type' - byte[]
  #       'payload' - byte[]
  def self.make_NdefRecord_from_hash(hash)
     rec = NdefRecord.new
     rec.init_from_hash(hash) 
     return rec 
  end    

  # make NdefMessage from byte[]
  def self.make_NdefMessage_from_byte_array(array)
     msg = NdefMessage.new
     msg.init_from_byte_array(array) 
     return msg 
  end    

  # make NdefMessage from NdefRecord[]
  def self.make_NdefMessage_from_array_of_NdefRecord(array)
     msg = NdefMessage.new
     msg.init_from_array_of_NdefRecord(array) 
     return msg 
  end    

  # make string from byte[] payload
  # tnf - int (from NdefRecord)
  # type - byte[] (from NdefRecord) 
  def self.make_string_from_payload(payload, tnf, type)
     return Nfc.make_string_from_payload(payload, tnf, type)
  end    
  
  # prepare byte[] payload from string with Absolute URI
  def self.make_payload_with_absolute_uri(uri_string)
      return Nfc.make_payload_with_absolute_uri(uri_string)
  end    

  # prepare byte[] payload from int prefix code and string URI
  def self.make_payload_with_well_known_uri(prefix_code, uri_string)
      return Nfc.make_payload_with_well_known_uri(prefix_code, uri_string)
  end    

  # prepare byte[] payload from string language code and string text
  def self.make_payload_with_well_known_text(language, text)
      return Nfc.make_payload_with_well_known_text(language, text)
  end    

 
end




end