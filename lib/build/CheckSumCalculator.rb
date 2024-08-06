require 'digest'
#require 'FileUtils'
require 'fileutils'
                  
class CheckSumSerializer
  @@hash_filename = "wm_bin_hash.txt"
  @@hash_filepath = nil

  def initialize(filepath)
    @@hash_filepath = File.join(filepath, @@hash_filename)
  end

  def save(hashes) 
    puts "save hashes for " + hashes.length.to_s + " files"

    FileUtils.rm_f @@hash_filepath

    f = File.new(@@hash_filepath, "w+")

    hashes.each { |file|
      f.puts file["name"].to_s + "|" + file["md5"].to_s
    }

    f.close
  end

  def load()
    loaded_hash = Array.new

    File.readlines(@@hash_filepath).each do |line|
      lines = line.each_line('|').to_a

      next if lines.length < 2

      name = lines[0].chomp('|')
      md5  = lines[1].chomp('|')
      name = name.chomp
      md5  = md5.chomp

      line_hash = Hash.new
      line_hash["name"] = name.to_s
      line_hash["md5"]  = md5.to_s

      loaded_hash << line_hash
    end

    puts "loaded hashes for " + loaded_hash.length.to_s + " files"

    loaded_hash
  end

  def cansave
    return !File.exist?(@@hash_filepath)
  end
end

class CheckSumCalculator

  def calc(src_dir, files_hash = nil)   
    if files_hash.nil?
      files_hash = Array.new 
    end
   
    Dir.foreach(src_dir) do |filename|
      next if filename.eql?('.') || filename.eql?('..')
          
      filepath = File.join(src_dir, filename)

      if File.directory?(filepath)
        calc(filepath, files_hash)
      else
        next if File.extname(filename) != ".obj"

        file = Hash.new
        file["name"] = filepath.to_s
        file["md5"]  = Digest::MD5.hexdigest(File.read(filepath.to_s)).to_s

        files_hash << file
      end
    end  
    
    files_hash
  end

end

class CheckSumComparer
  @@caclulator = CheckSumCalculator.new
  @@serializer = nil
  @@new_folder = nil

  def initialize(hashfile_path, new_folder)
    @@serializer = CheckSumSerializer.new(hashfile_path)
    @@new_folder = new_folder
  end

  def compare
    new_hash = @@caclulator.calc(@@new_folder)

    if @@serializer.cansave
      @@serializer.save(new_hash)
      return true
    end

    old_hash = @@serializer.load()

    old_hash.each { |fhash|
      detect_files = new_hash.detect {|f| 
        f["name"] == fhash["name"]
      }

      return true unless !detect_files.nil?

      if detect_files["md5"].eql?(fhash["md5"])
        next
      else
        return true
      end
    }

    return false
  end
   
end

=begin
#test stuff
ts = Time.now

cs = CheckSumComparer.new("C:/work/temp/a3/bin/tmp", 
	"C:/work/rhodes/platform/wm/bin/Windows Mobile 6.5.3 Professional DTK (ARMV4I)")

puts cs.compare

te = Time.now

puts (te-ts).to_s
=end
