require 'image_base'
class BlobBulkTest_s < ImageBase

  def initialize(source)
    AWS::S3::Base.establish_connection!(
      :access_key_id      => ENV['AMAZON_ACCESS_KEY_ID'],
      :secret_access_key  => ENV['AMAZON_SECRET_ACCESS_KEY']
    )
    @baseurl = 'http://s3.amazonaws.com/'
#    @bucket = 'rhodes-system-samples-images-auto'
    @bucket = 'rhodes-store-bulk-blobs-scheme'
    super(source)
  end

  def query(params=nil)
   puts "BlobBulkTest_s query params = #{params}"

   @result={}

    Bucket.find(@bucket).objects.each do |entry|
      new_item = {
        'image_uri' => @baseurl+@bucket+'/'+File.basename(entry.key),
        'name' => entry.key
      }
      puts  "Found: #{new_item.inspect}"
      @result[entry.key] = new_item
    end
    @result
  end

=begin
  def query(params=nil)
    puts "BlobBulkTest_s query params = #{params}"
    super(params)
  end
=end

  def schema
    {
      'version' => '1.0',
      'property' => {
        'name' => 'string',
        'filename' => 'string',
        'image_uri' => 'blob,overwrite'
      }
    }.to_json
  end

end
