class Rhohub::App < Rhohub::Base
     
  def self.create(data)
    catch_all do
      raise Exception.new "name must be passed to create ex: {app=>{:name => appname}}" unless data[:app][:name]
      super({},data,"apps")
    end
  end
  
  # options = {:id => app_id}
  def self.delete(options)
    catch_all do
      raise Exception.new "app_id must be passed to show ex: {:app_id => app_id}" unless options[:app_id]
      super(options,"apps")
    end
  end
  
  def self.list
    catch_all do
      super({},"apps")
    end
  end
  
  # options = {:app_id => app_id}
  def self.show(options)
    catch_all do
      raise Exception.new "app_id must be passed to show ex: {:app_id => app_id}" unless options[:app_id]
      super(options,"apps")
    end
  end
  
end