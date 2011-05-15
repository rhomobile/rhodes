module Rho


class NFCTagTechnology

  MIFARE_CLASSIC = 'MifareClassic'
  MIFARE_ULTRALIGHT = 'MifareUltralight'

  def initialize(tech_name)
       @techname = tech_name
  end

  def get_name
      return @techname
  end

  def connect
      Nfc.tech_connect(get_name)
  end

  def close
      Nfc.tech_close(get_name)
  end

  def is_connected
      return (  Nfc.tech_is_connected(get_name) != 0)
  end

end

class NFCTagTechnology_MifareClassic < NFCTagTechnology

  def initialize
       super(NFCTagTechnology::MIFARE_CLASSIC)
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

end

class NFCTagTechnology_MifareUltralight < NFCTagTechnology

  def initialize
       super(NFCTagTechnology::MIFARE_ULTRALIGHT)
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

end


class NFCTag

  def initialize(tech_list)
     puts 'Ruby NFCTag.initialize()'
     @techlist = tech_list
  end

  # return array of string
  def get_tech_list
      return @techlist
  end  

  def  get_tech(tech_name)
      if @techlist.include?(tech_name)
          if tech_name == 'MifareClassic'
              return NFCTagTechnology_MifareClassic.new()
          end
          if tech_name == 'MifareUltralight'
              return NFCTagTechnology_MifareULtralight.new()
          end
      else
          return nil
      end
  end

end




 
class NFCManager
 

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
  # @params['Tag_event'] - 'discovered', 'lost'
  def self.set_nfc_tech_callback(callback_url)
       Nfc.set_tech_callback(callback_url)
  end

  # set self.list of Techs for listen (array of strings)
  #def set_listen_tech_list(tech_list)
  #    Nfc.set_listen_tech_list(tech_list)
  #end

  # return current discovered Tag or nil if not any Tag discovered now
  def self.get_current_Tag
       tech_list = Nfc.get_tech_list
       if tech_list.size == 0
           return nil
       end
       return NFCTag.new(tech_list)
  end

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




end