require 'rhom'

class BulkTest
    include Rhom::PropertyBag

    #enable :sync
    set :partition, :app
    
end