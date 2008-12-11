class <%=name%> < SourceAdapter
  
  def initialize(source)
    super
  end

  def login
    #TODO: Write some code here
  end

  def query
    # TODO: write some code here
  end

  def sync
    # usually the generic base class sync does the job
    super
  end

  def create(name_value_list)
    #TODO: write some code here
  end

  def update(name_value_list)
    #TODO: write some code here
  end

  def delete(name_value_list)
    #TODO: write some code here if applicable
  end

  def logoff
    #TODO: write some code here if applicable
  end
end