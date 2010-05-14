require 'rhom'

class Product
    include Rhom::PropertyBag

    enable :sync
    set :partition, :application

end