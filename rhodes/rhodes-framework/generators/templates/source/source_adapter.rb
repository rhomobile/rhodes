class <%=name%> < SourceAdapter
  
  def initialize(source)
    super(source)
  end

  def login
    #TODO: Write some code here
    # use the variable @source.login and @source.password
    raise "Please provide some code to perform an authenticated login to the backend application"
  end

  def query
    # TODO: write some code here
    raise "Please provide some code to read records from the backend application"
  end

  def sync
    # usually this generic code does the job
    @result.entry_list.each do |x|
      x.name_value_list.each do |y|
        o=ObjectValue.new
        o.source_id=@source.id
        o.object=x['id']
        o.attrib=y.name
        o.value=y.value
        o.user_id=user_id if user_id
        o.save
      end
    end
  end

  def create(name_value_list)
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