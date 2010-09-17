require 'rhom'

class Product

    include Rhom::PropertyBag

    enable :sync
    #set :partition, :application
    set :sync_priority, 1  #sync engine should reorder sources
    
    belongs_to :quantity, ['Customer']
    belongs_to :sku, 'Customer'


=begin
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
=end
end