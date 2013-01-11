require 'image_base'
class Image < ImageBase  
  def initialize(source,credential)
    AWS::S3::Base.establish_connection!(
      :access_key_id      => ENV['AMAZON_ACCESS_KEY_ID'],
      :secret_access_key  => ENV['AMAZON_SECRET_ACCESS_KEY']
    )
    @baseurl = 'http://s3.amazonaws.com/'
    @bucket = 'rhodes-system-samples-images'
    super(source,credential)
  end
end