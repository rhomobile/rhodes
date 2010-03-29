#require 'time'
require 'rho/render'
require 'rho/rhoapplication'
require 'rhom'
require 'rhofsconnector'
require 'rho/rhoerror'

module Rho
  class RHO
    APPLICATIONS = {}
    APPNAME = 'app'
    
    @@rho_framework = nil
    def initialize(app_manifest_filename=nil)
      puts "Calling RHO.initialize"
      RHO.process_rhoconfig
      #Rhom::RhomDbAdapter::open(Rho::RhoFSConnector::get_db_fullpathname)
      @db_user = Rhom::RhomDbAdapter.new(Rho::RhoFSConnector::get_db_fullpathname)
      @db_application = Rhom::RhomDbAdapter.new(Rho::RhoFSConnector::get_db_fullpathname('app'))      
      
      if app_manifest_filename
        process_model_dirs(app_manifest_filename)
      else
        process_model_dirs(Rho::RhoFSConnector::get_app_manifest_filename)
      end
      
      # Initialize application and sources
      init_sources(@db_user)
      init_sources(@db_application)
      #get_app(APPNAME)
      
      @@rho_framework = self
    end

    attr_reader :db_application, :db_user

    def self.get_src_db(src_name=nil)
        if src_name
            Rho::RhoConfig.sources[src_name]['partition'] == 'user' ? @@rho_framework.db_user : @@rho_framework.db_application
        else
            @@rho_framework.db_user
        end    
    end

    def self.get_user_db
        @@rho_framework.db_user        
    end
    
    def self.get_application_db
        @@rho_framework.db_application
    end
    
    def init_app
      get_app(APPNAME)
    end

    def activate_app
      get_app(APPNAME).on_activate_app
    end
    
    # make sure we close the database file
    #def self.finalize
      #Rhom::RhomDbAdapter::close
      #puts 'finalize'
      #if @@rho_framework
      #  @@rho_framework.db_application.close()
      #  @@rho_framework.db_user.close()
        
      #  @@rho_framework = nil
      #end
    #end
    
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
    def init_sources(db)
      if defined? Rho::RhoConfig::sources
        
        uniq_sources = Rho::RhoConfig::sources.values
        puts 'init_sources: ' + uniq_sources.inspect

        #result = db.execute_sql("SELECT MAX(source_id) AS maxid FROM sources")
        #start_id = result.length > 0 && result[0]['maxid'] ? result[0]['maxid']+1 : 0
        
        # generate unique source list in database for sync
        uniq_sources.each do |source|
          
          name = source['name']
          priority = source['priority']
          partition = source['partition']
          sync_type = source['sync_type']
          
          attribs = db.select_from_table('sources','priority,source_id,partition, sync_type', 'name'=>name)

          if attribs && attribs.size > 0 
            if attribs[0]['priority'].to_i != priority.to_i
                db.update_into_table('sources', {"priority"=>priority},{"name"=>name})
            end
            if attribs[0]['sync_type'] != sync_type
                db.update_into_table('sources', {"sync_type"=>sync_type},{"name"=>name})
            end
            
            #Rho::RhoConfig::sources[name]['source_id'] = attribs[0]['source_id'].to_i
          else
            db.insert_into_table('sources',
                {"source_id"=>source['source_id'],"name"=>name, "priority"=>priority, "sync_type"=>sync_type, "partition"=>partition})
            #Rho::RhoConfig::sources[name]['source_id'] = start_id
            
            #start_id += 1
          end
        end
      end
    end
    
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
        return send_error(e, 500, true)
      end
    end


    def make_httpdate(t)
      _RFC2822_DAY_NAME = [
        'Sun', 'Mon', 'Tue', 'Wed', 'Thu', 'Fri', 'Sat'
      ]
      _RFC2822_MONTH_NAME = [
        'Jan', 'Feb', 'Mar', 'Apr', 'May', 'Jun',
        'Jul', 'Aug', 'Sep', 'Oct', 'Nov', 'Dec'
      ]
    
        sprintf('%s, %02d %s %d %02d:%02d:%02d GMT',
          _RFC2822_DAY_NAME[t.wday],
          t.day, _RFC2822_MONTH_NAME[t.mon-1], t.year,
          t.hour, t.min, t.sec)
    end
    
    def init_response(status=200,message="OK",body="")
      res = Hash.new
      res['status'] = status
      res['message'] = message
      res['headers'] = 
        {
        'Date' => make_httpdate(Time.now),#.httpdate,
        'Content-Type' => 'text/html; charset=utf-8',
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
      res['headers']['Content-Length'] = !res['request-body'].nil? && res['request-body'].is_a?(String) ? res['request-body'].bytesize : 0
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
      res['headers']['Content-Length'] = !res['request-body'].nil? && res['request-body'].is_a?(String) ? res['request-body'].bytesize : 0
      res['headers'].each{|key, value|
          tmp = key.gsub(/\bwww|^te$|\b\w/){|s| s.upcase }
          resp[tmp] = value
      }
      resp['request-body'] = !res['request-body'].nil? && res['request-body'].is_a?(String) ? res['request-body'] : ""
      resp['status'] = res['status']        
      resp['message'] = res['message']
      
      resp
    end
    
    @@current_exception = nil
    def self.current_exception
        @@current_exception
    end
        
    def send_error(exception=nil,status=500,hash=false)
	  puts "App error: #{exception}" if exception
      body=''
      
      err_page = nil
      if exception && exception.is_a?(::Rhom::RecordNotFound)
        err_page = RhoApplication::get_app_path(APPNAME) + 'E400_erb.iseq'
        err_page = nil unless File.exist?(err_page)
      elsif exception
        err_page = RhoApplication::get_app_path(APPNAME) + 'E500_erb.iseq'
        err_page = nil unless File.exist?(err_page)
      end

      if err_page
          @@current_exception = exception
          puts 'show error: ' + @@current_exception.inspect
          body = RhoController::renderfile(err_page)
          
          if ( hash )
            return send_response_hash(init_response(200,"OK",body))
          end
      
          return send_response(init_response(200,"OK",body))
      end
      
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
      body << "Trace: <br/>\n" << exception.backtrace.join("<br/>\n") if exception
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
        return if !modelname || modelname.length() == 0 || @@sources[modelname]
        
        @@sources[modelname] = new_source ? new_source : {}
        @@sources[modelname]['name'] ||= modelname
        @@sources[modelname]['priority'] ||= 1000
        @@sources[modelname]['partition'] ||= 'user'
        @@sources[modelname]['sync_type'] ||= 'incremental'
        
        if @@sources[modelname]['url'] && @@sources[modelname]['url'].length() == 0
            @@sources[modelname]['sync'] = false
        end

        if @@sources[modelname]['sync'].nil?
            @@sources[modelname]['sync'] = true
        end
        
      end
      
      @@g_base_temp_id = nil
      def generate_id
        @@g_base_temp_id = ((Time.now.to_f - Time.mktime(2009,"jan",1,0,0,0,0).to_f) * 10**6).to_i unless @@g_base_temp_id
        
        @@g_base_temp_id = @@g_base_temp_id + 1
        @@g_base_temp_id
      end
      
    end
  end # RhoConfig
  
    module AsyncHttp
        def self.ssl_verify_peer(args)
            ret = args[:ssl_verify_peer]
            ret = true if !ret.is_a? TrueClass and !ret.is_a? FalseClass
            ret ? 1 : 0
        end

	def self.process_result(res, callback)
            return res if callback && callback.length() > 0
            
            _params = ::Rho::RhoSupport::parse_query_parameters res
            ::Rho::RhoController.process_rho_object(_params)
            _params
        end
        
        def self.get(args)
            process_result( 
                AsyncHttp.do_get(args[:url], args[:headers],
                    args[:callback], args[:callback_param], ssl_verify_peer(args) ), args[:callback] )
        end
        def self.post(args)
            process_result( 
                AsyncHttp.do_post(args[:url], args[:headers], args[:body],
                    args[:callback], args[:callback_param], ssl_verify_peer(args) ), args[:callback] )
        end
        def self.download_file(args)
            process_result( 
                AsyncHttp.do_downloadfile(args[:url], args[:headers], args[:filename],
                    args[:callback], args[:callback_param], ssl_verify_peer(args) ), args[:callback] )
        end
        def self.upload_file(args)
            process_result( 
                AsyncHttp.do_uploadfile(args[:url], args[:headers], args[:filename],
                    args[:callback], args[:callback_param], ssl_verify_peer(args) ), args[:callback] )
        end
    end
  
end # Rho

#at_exit do
	#::Rhom::RhomDbAdapter.close
#end
