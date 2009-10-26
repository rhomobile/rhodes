require 'time'
require 'rho/render'
require 'rho/rhoapplication'
require 'rhom'
require 'rhofsconnector'
require 'rho/rhoerror'

module Rho
  class RHO
    APPLICATIONS = {}
    APPNAME = 'app'
    
    def initialize(app_manifest_filename=nil)
      puts "Calling RHO.initialize"
      RHO.process_rhoconfig
      Rhom::RhomDbAdapter::open(Rho::RhoFSConnector::get_db_fullpathname)
      if app_manifest_filename
        process_model_dirs(app_manifest_filename)
      else
        process_model_dirs(Rho::RhoFSConnector::get_app_manifest_filename)
      end
      
      # Initialize application and sources
      init_sources
      #get_app(APPNAME)
    end

    def init_app
      get_app(APPNAME)
    end
    
    # make sure we close the database file
    def self.finalize
      Rhom::RhomDbAdapter::close
    end
    
    def raise_rhoerror(errCode)
        raise Rho::RhoError.new(errCode)
    end
    
    def get_app(appname)
      if (APPLICATIONS[appname].nil?)
        require RhoApplication::get_app_path(appname)+'application'
        APPLICATIONS[appname] = Object.const_get('AppApplication').new
      end
      APPLICATIONS[appname]
    end
    
    # Return the directories where we need to load configuration files
    def process_model_dirs(app_manifest_filename=nil)
      File.open(app_manifest_filename).each do |line|
        str = line.chomp
        if str != nil and str.length > 0 
            require File.join(File.dirname(app_manifest_filename), str )
        end    
      end
    end
    
    # Return the directories where we need to load configuration files
    def self.process_rhoconfig
      begin
        File.open(Rho::RhoFSConnector.get_rhoconfig_filename).each do |line|
          # Skip empty or commented out lines
          next if line =~ /^\s*(#|$)/
          parts = line.chomp.split('=')
          key = parts[0]
          value = nil
          if key and defined? RHO_ME
            value = rho_get_app_property(key.strip)
          end
          
          if !value
            value = parts[1] if parts[1]
          end
            
          if key and value
            val = value.strip.gsub(/\'|\"/,'')
            val = val == 'nil' ? nil : val
            puts "rhoconfig: #{key} => #{val}"
            Rho::RhoConfig.add_config(key.strip,val)
          end  
        end
      rescue Exception => e
        puts "Error opening rhoconfig.txt: #{e}, using defaults."
      end
    end
    
    # setup the sources table and model attributes for all applications
    def init_sources
      if defined? Rho::RhoConfig::sources
        
        # quick and dirty way to get unique array of hashes
        uniq_sources = Rho::RhoConfig::sources.values.inject([]) { |result,h| 
          result << h unless result.include?(h); result
        }
        
        # generate unique source list in database for sync
        uniq_sources.each do |source|
          
          src_id = source['source_id']
          url = source['url']
          name = source['name']
          attribs = Rhom::RhomDbAdapter::select_from_table('sources','source_attribs', 'source_id'=>src_id)

          if attribs && attribs.size > 0 
            #Rhom::RhomAttribManager.load(src_id,attribs[0]['source_attribs'])
          else
            Rhom::RhomDbAdapter::insert_into_table('sources',
                                                  {"source_id"=>src_id,"source_url"=>url,"name"=>name})
          end
        end
      end
    end
    
#    def source_initialized?(source_id)
#      Rhom::RhomDbAdapter::select_from_table('sources','*', 'source_id'=>source_id).size > 0 ? true : false
#    end

    def serve(req)
      begin
        puts 'inside RHO.serve...'
        res = init_response
        get_app(req['application']).send :serve, req, res
        return send_response(res)
      rescue Exception => e
        return send_error(e)
      end   
    end

    def serve_hash(req)
      begin
        puts 'inside RHO.serve...'
        res = init_response
        get_app(req['application']).send :serve, req, res
        return send_response_hash(res)
      rescue Exception => e
        return send_error(e,500,true)
      end 
    end
    
    def serve_index(index_name)
    	# TODO: Removed hardcoded appname
    	get_app(APPNAME).set_menu
      begin
        puts 'inside RHO.serve_index: ' + index_name
        res = init_response
        res['request-body'] = RhoController::renderfile(index_name)
        return send_response(res)
      rescue Exception => e
        return send_error(e)
      end
    end

    def serve_index_hash(index_name)
    	# TODO: Removed hardcoded appname
    	get_app(APPNAME).set_menu
      begin
        puts 'inside RHO.serve_index: ' + index_name
        res = init_response
        res['request-body'] = RhoController::renderfile(index_name)
        return send_response_hash(res)
      rescue Exception => e
        return send_error(e.message, 500, true)
      end
    end
    
    def init_response(status=200,message="OK",body="")
      res = Hash.new
      res['status'] = status
      res['message'] = message
      res['headers'] = 
        {
        'Date' => Time.now.httpdate,
        'Content-Type' => 'text/html',
        'Content-Length' => 0,
        'Connection' => 'close' 
      }
      res['request-body'] = body
      res
    end

    CR   = "\x0d"
    LF   = "\x0a"
    CRLF = "\x0d\x0a"
    
    def send_response(res)
      res['headers']['Content-Length'] = !res['request-body'].nil? && res['request-body'].is_a?(String) ? res['request-body'].length : 0
      data = "HTTP/1.1 #{res['status'].to_s} #{res['message']}" + CRLF
      res['headers'].each{|key, value|
        tmp = key.gsub(/\bwww|^te$|\b\w/){|s| s.upcase }
        data << "#{tmp}: #{value}" << CRLF
      }
      data << "Pragma: no-cache" << CRLF
      data << "Cache-Control: must-revalidate" << CRLF
      data << "Cache-Control: no-cache" << CRLF
      data << "Cache-Control: no-store" << CRLF
      data << "Expires: 0" << CRLF

      data << CRLF
      if ( !res['request-body'].nil? && res['request-body'].is_a?(String))
        data << res['request-body']
      end
        
      data
    end

    def send_response_hash(res)
      resp = Hash.new
      res['headers']['Content-Length'] = !res['request-body'].nil? && res['request-body'].is_a?(String) ? res['request-body'].length : 0
      res['headers'].each{|key, value|
          tmp = key.gsub(/\bwww|^te$|\b\w/){|s| s.upcase }
          resp[tmp] = value
      }
      resp['request-body'] = !res['request-body'].nil? && res['request-body'].is_a?(String) ? res['request-body'] : ""
      resp['status'] = res['status']        
      resp['message'] = res['message']
      
      resp
    end
    
    def send_error(exception=nil,status=500,hash=false)
      body=''
      body << <<-_HTML_STRING_
        <html>
            <head>
                <meta name="viewport" content="width=320"/>
            </head>
            <body>
                <h2>Server Error</h2>
                <p>
      _HTML_STRING_
      body << 'Error: ' << exception.message << "<br/>" if exception
      body << 'Trace: ' << exception.backtrace.join("\n") if exception
      body << <<-_HTML_STRING_
                </p>    
            </body>
        </html>
        
      _HTML_STRING_
      if ( hash )
        send_response_hash(init_response(status,"Server error",body))
      else
        send_response(init_response(status,"Server error",body))
      end
    end
  end # RHO
  
  # Generic configuration class which accepts hashes with unique keys
  class RhoConfig
    
    @@sources = {}
    @@config = {'start_path' => '/app', 'options_path' => '/app/Settings'}
    
    class << self
      def method_missing(name, *args)
      	unless name == Fixnum
		      varname = name.to_s.gsub(/\=/,"")
          setting = (name.to_s =~ /=/)
          if setting
            @@config[varname] = args[0]
            # save changes to file
            RhoConf.set_property_by_name(varname,args[0]) 
          else
            @@config[varname]
          end
        end
      end
      
      def reload
        @@config = {'start_path' => '/app', 'options_path' => '/app/Settings'}
        Rho::RHO.process_rhoconfig
      end
        
      def show_log
        RhoConf.show_log
      end
      
      def send_log
        RhoConf.send_log
      end
      
      def sources
        @@sources
      end
      
      def config
        @@config
      end
      
      def add_config(key,value)
        @@config[key] = value if key # allow nil value
      end
      
      def add_source(modelname, new_source=nil)
        if new_source
          unless @@sources[new_source]
            @@sources[modelname] = new_source
            @@sources[modelname]['name'] ||= modelname
          end
        end
      end
    end
  end # RhoConfig
end # Rho
