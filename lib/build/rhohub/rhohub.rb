module Rhohub
  
    def self.token=(token)
      if token
        ENV['rhohub_token'] = token
      else
        raise "Invalid token"
      end
    end

    def self.token
      if ENV['rhohub_token']
        ENV['rhohub_token']
      else
        raise "You must set your rhohub api token see README file"
      end
    end
    
    def self.resource_url(options = {},resource)
      base_url = url
      if resource == 'apps'
        base_url += "/apps" if options.empty? #POST, INDEX
        base_url +=  "/apps/#{options[:app_id]}" if options.has_key?(:app_id) # DELETE, SHOW
      elsif resource == 'platforms'
        base_url += "/platforms"
      elsif resource == 'subscription'
        base_url += '/subscription'
      else
        base_url += "/app/#{options[:app_id]}/builds" if options.has_key?(:app_id) && !options.has_key?(:id) # POST, INDEX
        base_url += "/app/#{options[:app_id]}/builds/#{options[:id]}"  if options.has_key?(:id) && options.has_key?(:app_id) # DELETE, SHOW
      end
      base_url
    end
    
    def self.url
      ENV['rhohub_api_url']
    end
    
    def self.url=(url)
      if url
        ENV['rhohub_api_url'] = url
      else
        raise "Invalid url"
      end
    end
end