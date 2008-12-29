require 'find'

dir = '.'
fname = "rhorunner.app"
fappGuid = File.new( File.join(File.dirname(File.expand_path(__FILE__)),'rhodesguid.properties'), "w")

Find.find(dir) do |path| 
  if File.basename(path) == fname

    guid = File.basename(File.dirname(path))
    
    fappGuid.puts( 'rhodes.guid='+guid )
    
  end
end

fappManifest.close()
			  