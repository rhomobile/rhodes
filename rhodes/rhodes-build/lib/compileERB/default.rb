require 'find'
require 'erb'

dir = File.dirname(File.expand_path(__FILE__))
ext = ".erb"

Find.find(dir) do |path| 
  if File.extname(path) == ext
	rbText = ERB.new( IO.read(path) ).src
	
	#create iseq
	
	seq = __rho_compile(rbText)
	arr = seq.to_a
	
	newName = File.basename(path).sub('.erb','_erb.iseq')
	fName = File.join(File.dirname(path), newName)
	fseq = File.new(fName, "w")
	
	Marshal.dump(arr,fseq)
	
	fseq.close()
	
  end
end
			  