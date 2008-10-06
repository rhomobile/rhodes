require 'erb'

class RhoTest

def initialize
end

def respond
	 fname = File.join(File.dirname(File.expand_path(__FILE__)), 'hello.erb')
	 puts fname
	 # puts IO.read(fname)
	 # File.open(fname) {|f| puts f.read }
	 # bodys = body.to_s
	 # puts body
	 erb = ERB.new(IO.read(fname))
     return erb.result
end

end
