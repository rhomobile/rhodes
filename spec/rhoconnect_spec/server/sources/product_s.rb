require 'json'
require 'rest_client'

class Product_s < SourceAdapter
  
  def initialize(source)
    @base = 'http://rhostore.herokuapp.com/products'
    super(source)
  end
 
  def query(params=nil)
    parsed=JSON.parse(RestClient.get("#{@base}.json").body)
    
    @result={}
    parsed.each { |item|@result[item["product"]["id"].to_s]=item["product"] } if parsed    
    @result
  end
 
  def create(name_value_list)
    res = RestClient.post(@base,:product => name_value_list)
    
    # after create we are redirected to the new record. 
    # We need to get the id of that record and return it as part of create
    # so rhosync can establish a link from its temporary object on the 
    # client to this newly created object on the server
    JSON.parse(RestClient.get("#{res.headers[:location]}.json").body)["product"]["id"]
  end
 
  def update(name_value_list) 
    obj_id = name_value_list['id']
    name_value_list.delete('id')   
    RestClient.put("#{@base}/#{obj_id}",:product => name_value_list)
  end
 
  def delete(name_value_list)   
    RestClient.delete("#{@base}/#{name_value_list['id']}")
  end
end