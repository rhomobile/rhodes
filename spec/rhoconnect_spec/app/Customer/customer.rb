class Customer
  include Rhom::PropertyBag

  enable :sync

  set :sync_priority, 2

  #set :freezed, true

    property :address, :string
    property :created_at, :string
    property :city, :string
    property :email, :string
    #property :first, :string
    property :last, :string
    property :updated_at,  :string
    property :lat, :string
    property :long, :string
    property :phone, :string
    property :state, :string
    property :zip, :string
  
end