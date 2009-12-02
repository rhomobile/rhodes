require 'find'
require 'erb'

dir = File.dirname(File.expand_path(__FILE__))
ext = ".erb"

Find.find(dir) do |path| 
  if File.extname(path) == ext
    begin
        strFile = IO.read(path)
        strFile.force_encoding('utf-8')
	    rbText = ERB.new( strFile ).src
    	
	    newName = File.basename(path).sub('.erb','_erb.rb')
	    fName = File.join(File.dirname(path), newName)
	    frb = File.new(fName, "wb")
	    frb.write( rbText )
	    frb.close()
	rescue Exception => e
	    puts 'erb compilation failed: ' + path
	    raise
	end 
	    
  end
end
			  