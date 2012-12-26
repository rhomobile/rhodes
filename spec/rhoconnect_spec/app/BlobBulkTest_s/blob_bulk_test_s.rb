require 'rhom'

class BlobBulkTest_s
    include Rhom::FixedSchema

    enable :sync

    property :name, :string
    property :filename, :string
    property :image_uri, :blob, :overwrite
end