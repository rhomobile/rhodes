#require 'time'
require 'rho/render'
require 'rho/rhoapplication'
require 'rhom'
require 'rhofsconnector'
require 'rho/rhoerror'
require 'rhom/rhom_source'

module Rho
  def self.get_app
    RHO.get_instance().get_app('app')
  end

  def self.file_exist?(fname)
      return File.exist_injar?(fname) if defined? RHO_ME
            
      File.exist?(fname)
  end
  
  class RHO
    APPLICATIONS = {}
    APPNAME = 'app'
    
    @@rho_framework = nil
    
    def self.get_instance
        @@rho_framework
    end
    
    def initialize(app_manifest_filename=nil)
      puts "Calling RHO.initialize"
      RHO.process_rhoconfig
      
      if app_manifest_filename
        process_model_dirs(app_manifest_filename)
      else
        process_model_dirs(Rho::RhoFSConnector::get_app_manifest_filename)
      end
      
      # Initialize application and sources
      @@rho_framework = self
      init_sources()
    end

    attr_reader :db_partitions
    
    def self.get_src_db(src_name=nil)
        if src_name
            src_partition = Rho::RhoConfig.sources[src_name]['partition']
            @@rho_framework.db_partitions[src_partition]
        else
            @@rho_framework.db_partitions['user']
        end    
    end

    def self.get_user_db
        @@rho_framework.db_partitions['user']
    end

    def self.get_db_partitions
        @@rho_framework.db_partitions
    end
    
    #def self.get_application_db
    #    @@rho_framework.db_application
    #end
    
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
        require 'application' #RhoApplication::get_app_path(appname)+'application'
        APPLICATIONS[appname] = Object.const_get('AppApplication').new
      end
      APPLICATIONS[appname]
    end
    
    # Return the directories where we need to load configuration files
    def process_model_dirs(app_manifest_filename=nil)
      File.open(app_manifest_filename).each do |line|
        str = line.chomp
        if str != nil and str.length > 0 
            #puts "model file: #{str}"
            modelName = File.basename(File.dirname(str))
            Rhom::RhomObjectFactory.init_object(modelName)
            require str

            #puts "model name: #{modelName}"            

            modelClass = Object.const_get(modelName)
            if modelClass
                if modelClass.respond_to?( :get_model_params )
                    Rho::RhoConfig::add_source(modelName,modelClass.get_model_params())
                    modelClass.reset_model_params()
                else
                    puts "ERROR: Invalid model definition. Add 'include Rhom::PropertyBag' or 'include Rhom::FixedSchema' to model class"
                end    
            else
                puts "ERROR: cannot load model : #{modelClass}"
            end    
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

    def load_server_sources(data)
        puts "load_server_sources : #{data}"
        
        begin
            res = Rho::JSON.parse(data)
            if res['partition']
                str_partition = res['partition']
                puts "reload sources for partition: #{str_partition}"
                db = @db_partitions[ str_partition ]

                puts "sources before: #{Rho::RhoConfig::sources()}"
                
                Rho::RhoConfig::sources().delete_if {|key, value| value['partition']==str_partition }
                arSrcs = db.select_from_table('sources','source_id, name, sync_priority, partition, sync_type, schema, schema_version, associations, blob_attribs',
                    {'partition'=>str_partition} )
                arSrcs.each do |src|
                    Rho::RhoConfig::sources()[ src['name'] ] = src
                end
                
                puts "sources after: #{Rho::RhoConfig::sources()}"            
                return
            end
            
            arSrc = res['sources']
            puts "arSrc: #{arSrc}"
            return unless arSrc

            hashSrcs = Rhom::RhomSource::find_all_ashash
            puts "hashSrcs : #{hashSrcs}"
            arSrc.each do |src|
                oldSrc = hashSrcs[src['name']]
                puts "oldSrc: #{oldSrc}"
                if oldSrc
                    oldVer = oldSrc.schema_version
                    newVer = src['schema_version']
                    if ( oldVer != newVer )    
                        get_app(APPNAME).on_migrate_source(oldSrc.schema_version, src)
                    end
                end
                
                Rho::RhoConfig::add_source(src['name'], src)
            end
            
            init_sources()
      rescue Exception => e
        puts "Error load_server_sources: #{e}"
        puts "Trace: #{e.backtrace}"
      end
            
    end

    def find_src_byname(uniq_sources, src_name)
        uniq_sources.each do |source|        
            return source if src_name == source['name']
        end
        
        nil
    end
    
    # setup the sources table and model attributes for all applications
    def init_sources()
        return unless defined? Rho::RhoConfig::sources
    
        @db_partitions = {}
        uniq_sources = Rho::RhoConfig::sources.values
        puts 'init_sources: ' + uniq_sources.inspect

        uniq_sources.each do |source|
            source['str_associations'] = ""    
        end
        
        uniq_sources.each do |source|
          partition = source['partition']
          @db_partitions[partition] = nil
          
          if source['belongs_to']
            source['belongs_to'].each do |attrib, src_name|    
                associationsSrc = find_src_byname(uniq_sources, src_name)
                if !associationsSrc
                    puts ( "Error: belongs_to '#{source['name']}' : source name '#{src_name}' does not exist."  )
                    next
                end
                
                str_associations = associationsSrc['str_associations']
                str_associations = "" unless str_associations
                str_associations += ',' if str_associations.length() > 0
                
                str_associations += source['name'] + ',' + attrib
                associationsSrc['str_associations'] = str_associations
            end
          end
        end
        
        #user partition should alwayse exist
        @db_partitions['user'] = nil
        
        @db_partitions.each_key do |partition|
            db = Rhom::RhomDbAdapter.new(Rho::RhoFSConnector::get_db_fullpathname(partition), partition)
            db.start_transaction
            begin
                init_db_sources(db, uniq_sources, partition)
                db.commit
            rescue Exception => e
                puts "exception when init_db_sources: #{e}"    
                db.rollback
            end
                
            @db_partitions[partition] = db
        end
        
        ::Rho::RHO.init_schema_sources
    end

    def self.processIndexes(index_ar, src_name, is_unique)

        return "" unless index_ar
        
        strUnique = 'UNIQUE' if is_unique
        strRes = ""
        index_ar.each do |index_param|            
        
            if index_param.is_a?( String )
                strRes = index_param
            else
                nInd = 0
                
                index_param.each do |index_name, index_cols|
                    strCols = ""
                    index_cols.each do |col|
                        strCols += ',' if strCols.length() > 0
                        strCols += "\"#{col}\""
                    end

                    #strIndName = "rhoIndex" + (is_unique ? "U" : "" ) + "_#{nInd}"
                    strIndex = "CREATE #{strUnique} INDEX \"#{index_name}\" on #{src_name} (#{strCols});\r\n"
                    strRes += strIndex
                    nInd += 1
                end
            end
        end
        strRes
    end
        
    def self.init_schema_sources
        uniq_sources = Rho::RhoConfig::sources.values
        puts 'init_schema_sources'
        
        uniq_sources.each do |source|
          db = get_src_db(source['name'])
          
          next unless source['schema'] && source['model_type'] == 'fixed_schema' &&
            !db.table_exist?(source['name'])
            
          strCreate = source['schema']['sql']
          if source['schema']['property']
            arCols = source['schema']['property']
            arCols = arCols
            strCols = ""
            arCols.each do |col, type|
                strCols += ',' if strCols.length() > 0
                strCols += "\"#{col}\" varchar default NULL"
                #TODO: support column type
            end

            strCols += ",object varchar(255) PRIMARY KEY"
            strCreate = "CREATE TABLE #{source['name']} ( #{strCols} )"
          end

          strCreate += ";\r\n" if strCreate && strCreate.length() > 0
          strCreate += processIndexes(source['schema']['index'], source['name'], false)
          strCreate += ";\r\n" if strCreate && strCreate.length() > 0
          strCreate += processIndexes(source['schema']['unique_index'], source['name'], true)
        
          db.update_into_table('sources', {"schema"=>strCreate, "schema_version"=>source['schema_version']},{"name"=>source['name']})
        
          db.execute_batch_sql(strCreate)
          
        end
    end

    def process_blob_attribs(source, db)
      return source['str_blob_attribs'] if source['str_blob_attribs']
      source['str_blob_attribs'] = ""
      
      if source['schema'] && source['schema']['property']
        str = ""
        source['schema']['property'].each do |name, type|
            if type && type.is_a?(Hash)
                next unless type[:blob] || type['blob']
                str += ',' if str.length()>0
                str += name.to_s() + ',' + (type[:server_overwrite]||type[:server_overwrite].to_i() ? '1' : '0')
            elsif type && type.to_s == 'blob'
                str += ',' if str.length()>0
                str += name.to_s() + ',0'
            end
        end
        
        source['str_blob_attribs'] = str
      end
            
      source['str_blob_attribs']
      
    end
    
    def init_db_sources(db, uniq_sources, db_partition)
    
        result = db.execute_sql("SELECT MAX(source_id) AS maxid FROM sources")
        #puts 'result: ' + result.inspect
        start_id = result.length > 0 && result[0]['maxid'] ? result[0]['maxid']+2 : 1
        start_id = Rho::RhoConfig.max_config_srcid()+2 if start_id < Rho::RhoConfig.max_config_srcid
        
        uniq_sources.each do |source|
          puts "init_db_sources : #{source}"
          name = source['name']
          sync_priority = source['sync_priority']
          partition = source['partition']
          sync_type = source['sync_type']
          schema_version = source['schema_version']
          model_type = source['model_type']
          associations = source['str_associations']
          blob_attribs = process_blob_attribs(source, db)
          
          attribs = db.select_from_table('sources','sync_priority,source_id,partition, sync_type, schema_version, associations, blob_attribs, model_type', {'name'=>name})

          if attribs && attribs.size > 0 
            if attribs[0]['sync_priority'].to_i != sync_priority.to_i
                db.update_into_table('sources', {"sync_priority"=>sync_priority},{"name"=>name})
            end
            if attribs[0]['sync_type'] != sync_type
                db.update_into_table('sources', {"sync_type"=>sync_type},{"name"=>name})
            end
            if attribs[0]['schema_version'] != schema_version
                db.update_into_table('sources', {"schema_version"=>schema_version},{"name"=>name})
            end
            if attribs[0]['partition'] != partition
                db.update_into_table('sources', {"partition"=>partition},{"name"=>name})
            end
            if attribs[0]['associations'] != associations
                db.update_into_table('sources', {"associations"=>associations},{"name"=>name})
            end
            if attribs[0]['blob_attribs'] != blob_attribs
                db.update_into_table('sources', {"blob_attribs"=>blob_attribs},{"name"=>name})
            end

            if attribs[0]['model_type'] != model_type
                db.update_into_table('sources', {"model_type"=>model_type},{"name"=>name})
            end

            if !source['source_id']
                source['source_id'] = attribs[0]['source_id'].to_i
                Rho::RhoConfig::sources[name]['source_id'] = attribs[0]['source_id'].to_i
            end
            
          else
            if !source['source_id']
                source['source_id'] = start_id
                Rho::RhoConfig::sources[name]['source_id'] = start_id
                
                start_id += 1
            end
          
            db.insert_into_table('sources',
                {"source_id"=>source['source_id'],"name"=>name, "sync_priority"=>sync_priority, "sync_type"=>sync_type, "partition"=>partition,
                "schema_version"=>source['schema_version'], 'associations'=>associations, 'blob_attribs'=>blob_attribs,
                "model_type"=>model_type })
                
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
        err_page = nil unless ::Rho::file_exist?(err_page)
      elsif exception
        err_page = RhoApplication::get_app_path(APPNAME) + 'E500_erb.iseq'
        err_page = nil unless ::Rho::file_exist?(err_page)
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
    @@max_config_srcid = 1
    
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
      
      def exists?(name)
        RhoConf.is_property_exists(name)  
      end
      
      def reload
        @@config = {'start_path' => '/app', 'options_path' => '/app/Settings'}
        Rho::RHO.process_rhoconfig
      end
      
      def max_config_srcid
        @@max_config_srcid
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
        return if !modelname || modelname.length() == 0# || @@sources[modelname]
        
        puts "#{modelname} : #{new_source}"
        @@sources[modelname] = new_source ? new_source.clone() : {}
        @@sources[modelname]['name'] ||= modelname
        
        if @@sources[modelname]['sync_priority']
            @@sources[modelname]['sync_priority'] = @@sources[modelname]['sync_priority'].to_i()
        else
            @@sources[modelname]['sync_priority'] = 1000
        end
        
        if @@sources[modelname]['partition']
            @@sources[modelname]['partition'] = @@sources[modelname]['partition'].to_s
        else    
            @@sources[modelname]['partition'] ||= 'user'
        end    
        
        #@@sources[modelname]['sync_type'] = 'none' if !@@sources[modelname]['sync']
        @@sources[modelname]['sync_type'] ||= 'incremental'
        
        if @@sources[modelname]['source_id']
            @@sources[modelname]['source_id'] = @@sources[modelname]['source_id'].to_i()
        end
        
        @@max_config_srcid = @@sources[modelname]['source_id'] if @@sources[modelname]['source_id'] && @@max_config_srcid < @@sources[modelname]['source_id']
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

        def self.headers(args)
          hdrs = args[:headers]
          hdrs = {} if hdrs.nil?

          if hdrs['User-Agent'].nil?
            platform = System.get_property('platform')
            device = System.get_property('device_name')
            version = System.get_property('os_version')
            hdrs['User-Agent'] = "Mozilla-5.0 (#{platform}; #{device}; #{version})"
          end

          hdrs
        end

	def self.process_result(res, callback)
            return res if callback && callback.length() > 0
            
            _params = ::Rho::RhoSupport::parse_query_parameters res
            ::Rho::RhoController.process_rho_object(_params)
            _params
        end
        
        def self.get(args)
            process_result( 
                AsyncHttp.do_get(args[:url], headers(args),
                    args[:callback], args[:callback_param], ssl_verify_peer(args) ), args[:callback] )
        end
        def self.post(args)
            process_result( 
                AsyncHttp.do_post(args[:url], headers(args), args[:body],
                    args[:callback], args[:callback_param], ssl_verify_peer(args) ), args[:callback] )
        end
        def self.download_file(args)
            process_result( 
                AsyncHttp.do_downloadfile(args[:url], headers(args), args[:filename],
                    args[:callback], args[:callback_param], ssl_verify_peer(args) ), args[:callback] )
        end
        def self.upload_file(args)
            process_result( 
                AsyncHttp.do_uploadfile(args[:url], headers(args), args[:body], args[:filename],
                    args[:callback], args[:callback_param], ssl_verify_peer(args) ), args[:callback] )
        end
    end
  
end # Rho

#at_exit do
	#::Rhom::RhomDbAdapter.close
#end
