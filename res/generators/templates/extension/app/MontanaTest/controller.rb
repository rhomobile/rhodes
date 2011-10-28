require 'rho/rhocontroller'
require '<%= name.downcase %>'

class <%= name.camel_case %>TestController < Rho::RhoController
  @layout = :simplelayout
  
  def index
    render :back => '/app'
  end

  def run_test
    sum = <%= name.camel_case %>.calc_summ(3,7) 
    Alert.show_popup <%= name.camel_case %>Module::<%= name.camel_case %>StringHelper::process_string('test')+sum.to_s
    render :action => :index, :back => '/app'
  end
  
end
