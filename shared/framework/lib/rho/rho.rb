require 'time'
require 'rho/rhoapplication'

module Rho
  class RHO
    APPLICATIONS = {}
	
    def initialize
      puts "Calling RHO.initialize"
      require RhoApplication::get_base_app_path(appname)+'config'
      # setup the sources table and model attributes for this application
      def init_sources
        if defined? RHO_SOURCES
          attribs = {}
          Rhom::execute_sql "delete from sources"
          src_attribs = []
          attribs_empty = false
          RHO_SOURCES.each do |source|
            Rhom::execute_sql "insert into sources (source_id, source_url) values ('#{source['source_id']}','#{source['url']}')"
            src_attribs = Rhom::execute_sql "select distinct attrib from #{TABLE_NAME} \
                                             where source_id=#{id.to_i}"
            attribs[source] = src_attribs
            # there are no records yet, raise a flag so we don't define the constant
            if attribs[source] == 0
              attribs_empty = true
            end
          end
        end
        Object::const_set("SOURCE_ATTRIBS", attribs) unless defined? SOURCE_ATTRIBS or attribs_empty
      end
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
	
    def send_error(msg="",status=500)
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
      send_response(init_response(status,"Server error",body))
    end
  end # RHO
end # Rho
