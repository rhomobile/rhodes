require 'erb'

class RHO

def serve(req)
	begin
		fname = File.join(File.dirname(File.expand_path(__FILE__)), '../Test/hello.erb')
		puts fname
		puts "Passed parameters: " + req.to_s + "\n"
		erb = ERB.new(IO.read(fname))
		return erb.result
	rescue
		return ""
	end	
end

end
