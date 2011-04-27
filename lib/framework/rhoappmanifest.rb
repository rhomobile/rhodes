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