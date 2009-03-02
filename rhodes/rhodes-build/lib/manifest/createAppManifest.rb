require 'find'

dir = File.join(File.dirname(File.expand_path(__FILE__)), 'apps')
fname = "config.rb"
fappManifest = File.new( File.join(dir,'app_manifest.txt'), "w")

Find.find(dir) do |path| 
  if File.basename(path) == fname

    relPath = path[dir.length+1, File.dirname(path).length-1]   #relative path
    relPath = relPath[0, relPath.length-3] #remove .rb extension
    fappManifest.puts( relPath )
    
  end
end

fappManifest.close()
			  