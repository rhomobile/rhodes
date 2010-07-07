class RhoSpec

  def run_all_specs
    run_spec('AsyncHttp')
    run_spec('Json')
    run_spec('Xml')
    run_spec('RhoFile')
    run_spec('Bsearch')
    
    run_spec('rho')
    run_spec('rho_controller')
    run_spec('Rhom_Object')            
    
    #fix contacts on android
    run_spec('Contacts') unless System.get_property('device_name') == 'Win32'
    run_spec('MapView')  unless System.get_property('platform') == 'WINDOWS'
    run_spec('NativeBar') if System.get_property('platform') == 'APPLE' || System.get_property('platform') == 'ANDROID'
    run_spec('NavBar') if System.get_property('platform') == 'APPLE' || System.get_property('platform') == 'ANDROID'

if defined? RHO_ME
    run_spec('Xruby')
end
    run_spec('SyncEngine')        
    run_spec('BlobSync')        
    #run_spec('BulkSync')        

  end
  
end
