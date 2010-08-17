class Customer
  include Rhom::PropertyBag

  enable :sync

  set :sync_priority, 2
end