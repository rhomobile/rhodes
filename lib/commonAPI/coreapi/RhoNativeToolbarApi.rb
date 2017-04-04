if Rho::System.isRhoSimulator || System.get_property('platform') == 'WINDOWS' || System.get_property('platform') == 'WINDOWS_DESKTOP' || System.get_property('platform') == 'WP8' || System.get_property('platform') == 'UWP' || System.get_property('platform') == 'ANDROID' || System.get_property('platform') == 'APPLE'

module Rho
class NativeToolbar

  class << self
 
  alias :create_orig create
  
  def create(params, options = {})
  
    if (params.is_a?(Hash))
        buttons = params[:buttons]
    elsif params.is_a?(Array)
        buttons = params        
        params = options
    else
        raise ArgumentError, 'invalid argument: first parameter should be Hash or Array.'
    end

    if params == nil
        params = {}
    end

    params[:backgroundColor] = params[:background_color] if params[:background_color]

    params[:maskColor] = params[:mask_color] if params[:mask_color]
    params[:viewHeight] = params[:view_height] if params[:view_height]
    
    if ( buttons && buttons.is_a?(Array))    
        buttons.each do |btn|
            btn[:coloredIcon] = btn[:colored_icon] if btn && btn[:colored_icon]
        end
    end
    
    create_orig( buttons, params )
    
  end
  end #self
end
end 
 
end
