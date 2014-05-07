class Rhohub::Subscription < Rhohub::Base
       
  def self.check
    catch_all do
      super({},"subscription")
    end
  end
  
end