class <%=name%> 
  attr_accessor :client
  
  def initialize(source)
    @source=source
  end

  def login
  end

  def query
  end

  def sync
    @result.entry_list.each do |x|
      x.name_value_list.each do |y|
        o=ObjectValue.new
        o.source_id=@source.id
        o.object=x['id']
        o.attrib=y.name
        o.value=y.value
        o.save
      end
    end
  end

  def create(name_value_list)
  end

  def update(name_value_list)
  end

  def delete(name_value_list)
  end

  def logoff
  end
end