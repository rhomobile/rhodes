if Rho::System.getProperty('platform') == 'APPLE'  || Rho::System.getProperty('platform') == 'WINDOWS'  || Rho::System.getProperty('platform') == 'ANDROID'
  module Rho
    class SignatureCapture
      class << self
        def convert_options(opt)
          if opt != nil
            res_opt = {}
            opt.each do |key,value|
              if value == true
                value = 'true'
              end
              if value == false
                value = 'false'
              end
              res_opt[key.to_s] = value.to_s
            end 
            return res_opt
          end
          return nil
        end

        def makeDefaultFileName(options)
            return if options[:fileName] && options[:fileName].length() > 0
            
            options[:fileName] = File.join( Rho::Application.databaseBlobFolder(), "/Image_" + Time.now.to_i.to_s() )
        end
        
        def take(callback, options = nil)
          if options == nil
            options = {}
          end
          makeDefaultFileName(options)
if Rho::System.getProperty('platform') == 'ANDROID'         
          options = self.convert_options(options)
end          
          Rho::Signature.takeFullScreen(options, callback)
        end

        def capture(callback)
          Rho::Signature.capture(callback)
        end

        def clear()
          Rho::Signature.clear()
        end

        def visible(isvisible, options = nil)
          if (isvisible)
            if options == nil
              options = {}
            end
            
            makeDefaultFileName(options)
if Rho::System.getProperty('platform') == 'ANDROID'                    
            options = self.convert_options(options)
end            
            Rho::Signature.show(options)
          else
            Rho::Signature.hide()
          end
        end
      end
    end
  end
end