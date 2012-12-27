require 'rhom'

class BlobBulkTest
    include Rhom::PropertyBag

    enable :sync
    
	property :image_uri, :blob, :overwrite

    
end