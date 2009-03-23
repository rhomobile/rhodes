require 'find'

fselfname = File.expand_path(__FILE__)
dir = File.dirname(fselfname)

ext = ".rb"

Find.find(dir) do |path| 
  if File.extname(path) == ext && path != fselfname
	seq = RubyVM::InstructionSequence.compile_file(path)
	arr = seq.to_a
	
	newName = File.basename(path).sub('.rb','.iseq')
	fName = File.join(File.dirname(path), newName)
	fseq = File.new(fName, "w")
	
	Marshal.dump(arr,fseq)
	
	fseq.close()
  end
end
			  