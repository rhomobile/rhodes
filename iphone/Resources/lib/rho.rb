#require 'erb'
#require 'sqlite3'
require 'time'
require 'rhoapplication'

class RHO
  APPLICATIONS = {}
	
  SOURCE_ID = "1"
	
  def initialize
    puts "Calling RHO.initialize"
  end
	
  def get_app(appname, modelname)
    if (APPLICATIONS[appname].nil?)
      require RhoApplication::get_app_path(appname)+'application'
      APPLICATIONS[appname] = Object.const_get(appname+'Application').new(modelname, SOURCE_ID)
    end
    APPLICATIONS[appname]
  end
	
  def serve(req)
    begin
      res = init_response
      get_app(req['application'],req['model']).send :serve, req, res
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

  # Return a new hash with all keys converted to symbols.
  def symbolize_keys(hash)
    hash.inject({}) do |options, (key, value)|
      options[key.to_sym || key] = value
      options
    end
    hash
  end
end
