require 'rhom'

class Account
    include Rhom::PropertyBag

    #set :source_id, 23
    
    property :my_text,  :blob    
end