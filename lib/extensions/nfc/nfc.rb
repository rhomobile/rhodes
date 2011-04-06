module Rho
 
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
  
  def self.set_nfc_callback(callback_url)
       Nfc.set_callback(callback_url)
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