if Rho::System.isRhoSimulator || System.get_property('platform') == 'WINDOWS' || System.get_property('platform') == 'WINDOWS_DESKTOP' || System.get_property('platform') == 'WP8' || System.get_property('platform') == 'UWP'|| System.get_property('platform') == 'ANDROID'  || System.get_property('platform') == 'APPLE'


module Rho
class NativeTabbar

  class << self
 
  alias :create_orig create
  

  def create(params, options = {}, callback = nil)
  
    if (params.is_a?(Hash))
        buttons = params[:tabs]
    elsif params.is_a?(Array)
        buttons = params        
        params = options
    else
        raise ArgumentError, 'invalid argument: first parameter should be Hash or Array.'
    end

    if params == nil
        params = {}
    end

    if ( buttons && buttons.is_a?(Array))    
        buttons.each do |btn|
            btn[:coloredIcon] = btn[:colored_icon] if btn && btn[:colored_icon]
            btn[:selectedColor] = btn[:selected_color] if btn && btn[:selected_color]
            btn[:backgroundColor] = btn[:web_bkg_color] if btn && btn[:web_bkg_color]
            btn[:useCurrentViewForTab] = btn[:use_current_view_for_tab] if btn && btn[:use_current_view_for_tab]
            
            params[:backgroundColor] =btn[:background_color] if btn && btn[:background_color]
                        
        end
    end


    params[:backgroundColor] = params[:background_color] if params[:background_color]
    
    if params[:on_change_tab_callback] && params[:on_change_tab_callback].length() > 0
        puts "$$$ TabBar create with callback ["+params[:on_change_tab_callback].to_s+"].["+params[:on_change_tab_callback].class.to_s+"]"
        create_orig( buttons, params, params[:on_change_tab_callback] )
    else
        if callback != nil
             create_orig( buttons, params, callback )
        else 
             create_orig( buttons, params )
        end
    end    
    
  end
  end #self
end
end 
 
end
