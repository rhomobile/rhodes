require 'net/https'
require 'uri'
require 'open-uri'
require 'openssl'

class SiteChecker

	@@was_checked = false
	@@available = false
	@@site = ""
	@@proxy = nil

	class <<self
		def is_available? 
			if !@@was_checked
				@@available = false
				uri = URI.parse(@@site)

				begin
					if !(proxy.nil? || proxy.empty?)
						proxy_uri = URI.parse(proxy)
						http = Net::HTTP.new(uri.host, uri.port, proxy_uri.host, proxy_uri.port, proxy_uri.user, proxy_uri.password )
					else
						http = Net::HTTP.new(uri.host, uri.port)
					end

					http.use_ssl = true if uri.scheme == "https"  # enable SSL/TLS

					http.start {
						http.request_get(uri.path) {|res|}
					}
					@@available = true
				rescue => e
					puts "Error accessing #{uri.inspect}: #{e.inspect}"
					puts e.backtrace
				end
				@@was_checked = true
			end
			@@available
		end

		def site=(site)
			@@site = site
			@@was_checked = false
		end

		def site
			@@site
		end

		def proxy=(proxy)
			@@proxy = proxy
			@@was_checked = false
		end

		def proxy
			@@proxy
		end
	end
end
