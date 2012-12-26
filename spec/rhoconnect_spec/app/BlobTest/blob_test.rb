require 'rhom'

class BlobTest
    include Rhom::PropertyBag

    enable :sync
    
    property :image_uri, :blob, :overwrite

    
end