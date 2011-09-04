#------------------------------------------------------------------------
# (The MIT License)
# 
# Copyright (c) 2008-2011 Rhomobile, Inc.
# 
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
# 
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
# 
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.
# 
# http://rhomobile.com
#------------------------------------------------------------------------

require 'find'

fselfname = File.expand_path(__FILE__)
dir = File.dirname(fselfname)

ext = ".rb"

#startdir = Dir.pwd

#puts "dir : #{dir}"
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

#puts "startdir : #{startdir}"
#Dir.chdir startdir	
