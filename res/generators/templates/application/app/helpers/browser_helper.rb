module BrowserHelper
  
  def placeholder(label=nil)
    "placeholder='#{label}'" if platform == 'apple'
  end

  
  def platform
    System::get_property('platform').downcase
  end
  
  
end