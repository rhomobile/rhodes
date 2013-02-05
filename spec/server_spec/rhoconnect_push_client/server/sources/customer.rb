require 'json'
require 'rest_client'

class Customer < SourceAdapter

  def initialize(source)
    @base = 'http://rhostore.herokuapp.com/customers'
    super(source)
  end
 
  def query(params=nil)
	  #	  sleep(100)

    parsed=JSON.parse(RestClient.get("#{@base}.json").body)
    
    @result={}
    parsed.each do |item|
      if not params or (params and params["first"].downcase == item["customer"]["first"].downcase)
        @result[item["customer"]["id"].to_s] = item["customer"]
      end
    end if parsed    
    @result
  end
  
  def search(params)
    query(params)
  end
 
 
  def create(name_value_list)
    res = RestClient.post(@base,:customer => name_value_list)
    
    # after create we are redirected to the new record. 
    # We need to get the id of that record and return it as part of create
    # so rhosync can establish a link from its temporary object on the 
    # client to this newly created object on the server
    JSON.parse(RestClient.get("#{res.headers[:location]}.json").body)["customer"]["id"]
  end
 
  def update(name_value_list)
    obj_id = name_value_list['id']
    name_value_list.delete('id')   
    RestClient.put("#{@base}/#{obj_id}",:customer => name_value_list)
  end
 
  def delete(name_value_list)
    RestClient.delete("#{@base}/#{name_value_list['id']}")
  end
  
end