require 'rhom'

class Product_s
    include Rhom::FixedSchema

    enable :sync
    set :schema_version, '1.0'
    
    property :brand, :string
    property :created_at, :string
    property :name, :string
    property :price, :string
    property :quantity, :string
    property :sku, :string
    property :updated_at,  :string

end