require 'find'

fselfname = File.expand_path(__FILE__)
dir = File.dirname(fselfname)

ext = ".rb"

startdir = Dir.pwd
Dir.chdir dir

Find.find(dir) do |path| 
  if File.extname(path) == ext && path != fselfname
    begin
        relPath = path[dir.length+1,path.length - dir.length]
	    seq = RubyVM::InstructionSequence.compile_file(relPath)
	    arr = seq.to_a
    	
	    newName = File.basename(path).sub('.rb','.iseq')
	    fName = File.join(File.dirname(path), newName)
	    fseq = File.new(fName, "w")
    	
	    Marshal.dump(arr,fseq)
    	
	    fseq.close()
	rescue Exception => e
	    puts 'rb compilation failed: ' + path
	    raise
	end 
	    
  end
end

Dir.chdir startdir			  