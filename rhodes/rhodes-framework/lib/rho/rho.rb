require 'time'
require 'rho/render'
require 'rho/rhoapplication'
require 'rhom'
require 'rhofsconnector'

module Rho
  class RHO
    APPLICATIONS = {}
	
    def initialize(app_manifest_filename=nil)
      puts "Calling RHO.initialize"
      process_rhoconfig
      Rhom::RhomDbAdapter::open(Rho::RhoFSConnector::get_db_fullpathname)
      if app_manifest_filename
        process_model_dirs(app_manifest_filename)
      else
        process_model_dirs(Rho::RhoFSConnector::get_app_manifest_filename)
      end
      init_sources
    end
    
    # make sure we close the database file
    def self.finalize
      Rhom::RhomDbAdapter::close
    end
    
    # Return the directories where we need to load configuration files
    def process_model_dirs(app_manifest_filename=nil)
      File.open(app_manifest_filename).each do |line|
        require File.join(File.dirname(app_manifest_filename), line.chop)
      end
    end
    
    # Return the directories where we need to load configuration files
    def process_rhoconfig
      begin
        File.open(Rho::RhoFSConnector.get_rhoconfig_filename).each do |line|
          parts = line.chop.split('=')
          key = parts[0]
          value = parts[1..parts.length-1].join('=') if parts and parts.length > 1
          Rho::RhoConfig.config[key.strip] = value.strip!.gsub(/'|"/,'') if key and value
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
          if !self.source_initialized?(src_id)
            Rhom::RhomDbAdapter::insert_into_table('sources',
                                                  {"source_id"=>src_id,"source_url"=>url})
          end
        end
      end
    end
    
    def source_initialized?(source_id)
      Rhom::RhomDbAdapter::select_from_table('sources','*', 'source_id'=>source_id).size > 0 ? true : false
    end
	
    def get_app(appname)
      if (APPLICATIONS[appname].nil?)
        require RhoApplication::get_app_path(appname)+'application'
        #APPLICATIONS[appname] = Object.const_get(appname+'Application').new
        APPLICATIONS[appname] = Object.const_get('AppApplication').new
      end
      APPLICATIONS[appname]
    end

    def get_start_path
      Rho::RhoConfig.start_path
    end
    
    def get_options_path
      Rho::RhoConfig.options_path
    end
    
    def get_rhobundle_zip_url
      Rho::RhoConfig.rhobundle_zip_url
    end
    
    def get_rhobundle_zip_pwd
      Rho::RhoConfig.rhobundle_zip_pwd
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
      res['headers']['Content-Length'] = res['request-body'].nil? ? 0 : res['request-body'].length
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
      data << res['request-body']
      data
    end

    def send_response_hash(res)
		resp = Hash.new
		res['headers']['Content-Length'] = res['request-body'].nil? ? 0 : res['request-body'].length
		res['headers'].each{|key, value|
			tmp = key.gsub(/\bwww|^te$|\b\w/){|s| s.upcase }
			resp[tmp] = value
		}
        resp['request-body'] = res['request-body']
        resp['status'] = res['status']        
        resp['message'] = res['message']
        
		resp
    end
	
    def send_error(exception=nil,status=500,hash=false)
      body=''
      body << <<-_HTML_STRING_
		<html>
			<head>
				<title>Server Error</title>
				<meta name="viewport" content="width=320"/>
			</head>
			<body>
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
    @@config = {'start_path' => '/app', 
                'options_path' => '/app/Settings'}
    
    class << self
      def method_missing(name, *args)
        varname = name.to_s.sub(/=/,'')
        @@config[varname]
      end
      
      def sources
        @@sources
      end
      
      def config
        @@config
      end
      
      def add_source(modelname, new_source=nil)
        if new_source
          unless @@sources[new_source]
            @@sources[modelname] = new_source
          end
        end
      end
    end
  end # RhoConfig
end # Rho