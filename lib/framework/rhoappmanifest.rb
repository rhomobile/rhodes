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

module Rho

  class AppManifest

        def self.enumerate_models(src_dir)
          
          fappManifest = StringIO.new("", "w+")

          Find.find(src_dir) do |path|

            strDir = File.dirname(path)
            next unless src_dir == File.dirname(strDir) #one level only

            if File.basename(path) == "config.rb"
                puts "******ERROR enumerating models***********"
                puts "Model definition has changed and doesn't use config.rb anymore: '#{path}' "
                puts "You should replace config.rb with <model_name>.rb file as described: "
                puts "http://wiki.rhomobile.com/index.php/Rhom#Rhom_Models"
                puts "*****************************************"
                exit 1
            end
            
            fname = File.dirname(path)
            fname = File.basename(fname)
            modelname = fname.split(/(?=[A-Z])/).map{|w| w.downcase}.join("_")
            fname = modelname + ".rb"
          
            if File.basename(path) == fname

              relPath = path[src_dir.length+1, File.dirname(path).length-1]   #relative path
              relPath = relPath[0, relPath.length-3] #remove .rb extension and app
              relPath = File.join(File.dirname(relPath), modelname )
              fappManifest.puts( relPath )

            end
          end

          fappManifest.rewind
          fappManifest
        end
    end        
end