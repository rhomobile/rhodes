require 'aws/s3'

class ImageBase < SourceAdapter
  include AWS::S3
  
  def query(params=nil)
    @result={}
    Bucket.find(@bucket).objects.each do |entry|
      new_item = {
        'image_uri-rhoblob' => @baseurl+@bucket+'/'+File.basename(entry.key),
        'name' => entry.key
      }
      puts  "Found: #{new_item.inspect}"
      @result[entry.key] = new_item
    end
    @result
  end
 
  def create(name_value_list)
    puts "Creating: #{name_value_list.inspect}"
    name = name_value_list["image_uri"]
    basename = name_value_list["filename"]
    S3Object.store(
      basename,
      open(name),
      @bucket,
      :access => :public_read
    )
    basename
  end
 
  def delete(name_value_list)
    puts "Removing: #{name_value_list.inspect}"
    S3Object.delete name_value_list['id'], @bucket
  end
end