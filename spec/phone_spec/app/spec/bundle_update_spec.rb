
describe "BundleUpdate" do

def copy_file(src, dst)
    if !defined?(RHO_WP7)  
        content = File.binread(src)  
    else
        content = File.read(src)  
    end    
    File.open(dst, "wb"){|f| f.write(content) }
end    
    
  it "should bundle update" do

    # check original content








    # do partial bundle update

    bundle_dir = Rho::RhoApplication::get_base_app_path
    bundle_dir = bundle_dir.slice(0, bundle_dir.size - "/apps".size) + 'RhoBundle' 
    Dir.mkdir bundle_dir  
      
    copy_file(File.join(Rho::RhoApplication::get_base_app_path, 'public', 'partial_update_bundle', 'upgrade_bundle_partial.zip'), ::Rho::RhoSupport.rhobundle_getfilename())
    bundle_unziped = (System.unzip_file(::Rho::RhoSupport.rhobundle_getfilename())==0)
    
    bundle_unziped.should == true
      
    System.replace_current_bundle( File.dirname(::Rho::RhoSupport.rhobundle_getfilename()), { :do_not_restart_app => true, :not_thread_mode => true } )
  
      

    # check updated content



  end

end
