module Rhohub
  @@requester = nil
  @@token = nil
  @@url = nil
  @@cert_store = nil

  def self.cert_store
    @@cert_store
  end

  def self.cert_store=(store)
    @@cert_store = store
    @@requester = nil
  end

  def self.token=(token)
    if token
      @@token = token
      @@requester = nil
    else
      raise "Invalid token"
    end
  end

  def self.token
    if @@token
      @@token
    else
      raise "You must set your rhohub api token see README file"
    end
  end

  def self.url
    @@url
  end

  def self.url=(url)
    if url
      @@url = url
      @@requester = nil
    else
      raise "Invalid url"
    end
  end

  def self.resource_url(options = {},resource)
    base_url = ''
    case resource
    when 'apps'
      base_url += "/apps" if options.empty? #POST, INDEX
      base_url +=  "/apps/#{options[:app_id]}" if options.has_key?(:app_id) # DELETE, SHOW
    when 'platforms'
      base_url += "/platforms"
    when 'subscription'
      base_url += '/subscription'
    when 'token'
      base_url += '/token'
    when 'supported_gems'
      base_url += '/build/supported_gems'
    when 'user_status'
      base_url += '/build/user_status'
    else
      base_url += "/app/#{options[:app_id]}/builds" if options.has_key?(:app_id) && !options.has_key?(:id) # POST, INDEX
      base_url += "/app/#{options[:app_id]}/builds/#{options[:id]}"  if options.has_key?(:id) && options.has_key?(:app_id) # DELETE, SHOW                  
    end
    base_url
  end


  def self.default_request(need_token = true)
    request = {
        :open_timeout => 5,
        :headers => { :content_type => :json, :accept => :json }
    }

    if @@cert_store
      request[:ssl_cert_store] = @@cert_store
    end

    # FIXME: it is a sslv3 issue https://github.com/nahi/httpclient/issues/202
    request[:ssl_version] = :SSLv23

    if need_token
      request[:headers][:AUTHORIZATION] = Rhohub.token
    end

    request
  end

  def self.requester(need_token = true)
    unless need_token
      return RestClient::Resource.new(@@url, default_request(need_token))
    end

    if @@requester.nil?
      @@requester = RestClient::Resource.new(@@url, default_request)
    end

    @@requester
  end
end
