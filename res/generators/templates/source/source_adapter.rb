class <%=name.camelize%> < SourceAdapter
  def initialize(source,credential)
    super(source,credential)
  end
 
  def login
    #TODO: Write some code here
    # use the variable @source.login and @source.password
    raise "Please provide some code to perform an authenticated login to the backend application"
  end
 
  def query
    # TODO: write some code here, Query your backend for objects. Put into some variable that is used in sync method below
    raise "Please provide some code to read records from the backend application"
  end
 
  def sync
    # TODO: write code here that converts the data you got back from query into an @result object
    # where @result is a hash of hashes,  each array item representing an object
    # for example: @result={"1"=>{"name"=>"Acme","industry"=>"Electronics"},"2"=>{"name"=>"Best","industry"=>"Software"}}
    # if you have such a hash of hashes, then you can just call "super" as shown below
    super # this creates object value triples from an @result variable that contains a hash of hashes 
  end
 
  def create(name_value_list,blob=nil)
    #TODO: write some code here
    # the backend application will provide the object hash key and corresponding value
    raise "Please provide some code to create a single object in the backend application using the hash values in name_value_list"
  end
 
  def update(name_value_list)
    #TODO: write some code here
    # be sure to have a hash key and value for "object"
    raise "Please provide some code to update a single object in the backend application using the hash values in name_value_list"
  end
 
  def delete(name_value_list)
    #TODO: write some code here if applicable
    # be sure to have a hash key and value for "object"
    # for now, we'll say that its OK to not have a delete operation
    # raise "Please provide some code to delete a single object in the backend application using the hash values in name_value_list"
  end
 
  def logoff
    #TODO: write some code here if applicable
    # no need to do a raise here
  end
end