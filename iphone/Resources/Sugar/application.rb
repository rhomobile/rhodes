require 'rhoapplication'

class SugarApplication < RhoApplication
  def initialize
    model_name = "Account"
    puts 'in sugar initialize'
  end
end