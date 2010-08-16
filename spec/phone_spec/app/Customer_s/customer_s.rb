class Customer_s
    include Rhom::FixedSchema

    enable :sync
    set :sync_priority, 1
    
    set :schema_version, '1.0'

    property :address, :string
    property :created_at, :string
    property :city, :string
    property :email, :string
    property :first, :string
    property :last, :string
    property :updated_at,  :string
    property :lat, :string
    property :long, :string
    property :phone, :string
    property :state, :string
    property :zip, :string
end