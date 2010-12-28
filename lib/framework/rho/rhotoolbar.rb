
module Rho
 
class NativeToolbar
 
  def self.create(params)
    NativeBar.create_native_toolbar(Rho::RhoApplication::TOOLBAR_TYPE, params)
  end
 
  def self.remove()
    NativeBar.remove_native_toolbar()
  end

end
 
 
end
