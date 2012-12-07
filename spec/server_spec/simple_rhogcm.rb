require 'rest_client'
require 'json'

module RhoPush
  class Gcm
    class InvalidApiKey < Exception; end
    class GCMPingError < Exception; end

    def self.send_ping_to_device(api_key,params)
      RestClient.post(
        'https://android.googleapis.com/gcm/send', gcm_message(params).to_json,
        :authorization => "key=#{api_key}",
        :content_type => :json
      ) do |response, request, result, &block|
        # return exceptions based on response code & body
        case response.code
        when 200
          if response.body =~ /^Error=(.*)$/
            raise GCMPingError.new("GCM ping error: #{$1 || ''}")
          end
          response.return!(request, result, &block)
        when 401, 403
          raise InvalidApiKey.new("Invalid GCM api key. Obtain new api key from GCM service.")
        end
      end
    end

    def self.gcm_message(params)
      params.reject! {|k,v| v.nil? || v.length == 0}
      data = {}
      data['collapse_key'] = (rand * 100000000).to_i.to_s
      data['data'] = {}
      data['data']['sources'] = ''
      
      params.each do |k,v|
        if k == 'device_pin'
          data['registration_ids'] = [v.to_s]
          next
        end
        data['data'][k] = v
      end

      puts "Data to send: #{data}"
      
      data
    end
  end
end