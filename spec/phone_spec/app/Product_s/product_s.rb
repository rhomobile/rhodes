class Product_s
    include Rhom::FixedSchema

    enable :sync
    set :schema_version, '1.0'
    
    property :billing_address_country, :string

end