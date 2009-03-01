require 'find'

install_path = "-\"!:\\Data\\Rho"

dir = File.expand_path( File.dirname(File.expand_path(__FILE__)) + "/../RhoBundle")
puts "Rhodes root directory..."
puts dir

pkg_file_path = File.expand_path( File.dirname(File.expand_path(__FILE__)) ) + "/rhodes_tmp.pkg"
puts "Pkg file path..."
puts pkg_file_path

pkg_file = File.open(pkg_file_path, File::WRONLY|File::APPEND)

Find.find(dir) do |path|
    if FileTest.directory?(path)
      next
    else
	rel_file_path = path
	rel_file_path = rel_file_path.gsub(dir, "")

      line = "\"" + path.tr("/", "\\") + "\"      " + install_path + rel_file_path.tr("/", "\\") + "\""

      pkg_file.puts( line )
    end
end