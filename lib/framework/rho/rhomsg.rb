module Rho
  class RhoMessages
    def self.get_message(name)
        LocalMessages[name]
    end
  end # RhoMessages
end # Rho

require 'rholang/localization_simplified'
LocalizationSimplified.requre_loc('rholang/rhomsg_',false)
