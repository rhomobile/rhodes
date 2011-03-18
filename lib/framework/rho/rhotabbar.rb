
module Rho
 
class NativeTabbar
 
  def self.create(params)
    NativeBar.create_native_tabbar(Rho::RhoApplication::TABBAR_TYPE, params)
  end

  def self.create_vertical(params)
    NativeBar.create_native_tabbar(Rho::RhoApplication::VTABBAR_TYPE, params)
  end
 
  def self.remove()
    NativeBar.remove_native_tabbar()
  end
 
  def self.switch_tab(tab_index)
    NativeBar.native_tabbar_switch_tab(tab_index)
  end

  def self.set_tab_badge(tab_index, value)
    NativeBar.native_tabbar_set_tab_badge(tab_index, value)
  end

  def self.get_current_tab
      if System::get_property('platform') == 'APPLE' or System::get_property('platform') == 'ANDROID'
          return NativeBar.native_tabbar_get_current_tab
      else 
          return 0
      end
  end

end
 
 
end
