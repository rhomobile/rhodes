require 'find'
require 'erb'

dir = File.dirname(File.expand_path(__FILE__))
ext = ".erb"

Find.find(dir) do |path| 
  if File.extname(path) == ext
	rbText = ERB.new( IO.read(path) ).src
	newName = File.basename(path).sub('.erb','_erb.rb')
	fName = File.join(File.dirname(path), newName)
	frb = File.new(fName, "w")
	frb.write( rbText )
	frb.close()
  end
end
			  