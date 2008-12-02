require 'time'
if defined? RHO_ME
	require 'rho/renderME'
else
	require 'rho/render'
end
require 'rho/rhoapplication'
require 'rhom'
require 'rhofsconnector'

module Rho
  class RHO
    APPLICATIONS = {}
	
    def initialize(app_base_path=nil)
      puts "Calling RHO.initialize"
      Rhom::RhomDbAdapter::open(Rho::RhoFSConnector::get_db_fullpathname)
      if app_base_path
        process_model_dirs(app_base_path)
      else
        process_model_dirs(RhoApplication::get_base_app_path)
      end
      init_sources
    end
    
    # make sure we close the database file
    def self.finalize
      Rhom::RhomDbAdapter::close
    end
    
    # Return the directories where we need to load configuration files
    def process_model_dirs(app_base_path=nil)
      Rho::RhoFSConnector::enum_files(app_base_path,'config.rb') do |path|
        require path
      end
    end
    
    # setup the sources table and model attributes for all applications
    def init_sources
      puts 'sources initialized? ' + self.sources_initialized?.inspect
      if defined? Rho::RhoConfig::sources and not self.sources_initialized?
        Rhom::RhomDbAdapter::delete_all_from_table('sources')
        src_attribs = []
        
        # quick and dirty way to get unique array of hashes
        uniq_sources = Rho::RhoConfig::sources.values.inject([]) { |result,h| 
          result << h unless result.include?(h); result
        }
		
        # generate unique source list in databse for sync
        uniq_sources.each do |source|
          src_id = source['source_id']
          url = source['url']
          Rhom::RhomDbAdapter::insert_into_table('sources',
                                                 {"source_id"=>src_id,"source_url"=>url})
        end
      end
    end
    
    def sources_initialized?
      Rhom::RhomDbAdapter::select_from_table('sources','*').size > 0 ? true : false
    end
	
    def get_app(appname)
      if (APPLICATIONS[appname].nil?)
        require RhoApplication::get_app_path(appname)+'application'
        APPLICATIONS[appname] = Object.const_get(appname+'Application').new
      end
      APPLICATIONS[appname]
    end

    def serve(req)
      begin
	    puts 'inside RHO.serve...'
        res = init_response
        get_app(req['application']).send :serve, req, res
        return send_response(res)
      rescue Exception => e
        return send_error(e.message)
      end	
    end

    def serve_hash(req)
		begin
			puts 'inside RHO.serve...'
			res = init_response
			get_app(req['application']).send :serve, req, res
			return send_response_hash(res)
		rescue Exception => e
			return send_error(e.message,500,true)
		end	
    end
	
	def serve_index(index_name)
		begin
			puts 'inside RHO.serve_index: ' + index_name
			res = init_response
			res['request-body'] = RhoController::renderfile(index_name)
			return send_response(res)
		rescue Exception => e
			return send_error(e.message,500,true)
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
	
    def send_error(msg="",status=500,hash=false)
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
      body << 'Error message: ' << msg		
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
    class << self
      def sources
        @@sources
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