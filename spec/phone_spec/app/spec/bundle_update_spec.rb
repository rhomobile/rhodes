
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

    app_root = Rho::RhoApplication::get_base_app_path
    public_root = File.join(app_root, 'public')  

    File.exist?(File.join(public_root, 'folder_to_remove', 'switch.png')).should == true   

    File.exist?(File.join(public_root, 'file_for_replace.txt')).should == true   

    content = ''
    fileName = File.join(public_root, 'file_for_replace.txt')
    
    content = File.read(fileName) if File.exist?(fileName)
    #if File.exist?(fileName)
    #   File.open(fileName).each do |line|
    #     content = content + line
    #   end    
    #end
    content.should == 'original'    
  
      
    File.exist?(File.join(public_root, 'file_to_remove.png')).should == true   
    
    File.exist?(File.join(public_root, 'folder_to_remove.png')).should == true   
      
    File.exist?(File.join(public_root, 'added', 'added_image.png')).should == false   


    # do partial bundle update

    bundle_dir = Rho::RhoApplication::get_base_app_path
    bundle_dir = bundle_dir.slice(0, bundle_dir.size - "/apps".size) + 'RhoBundle' 
    Dir.mkdir bundle_dir  
      
    copy_file(File.join(Rho::RhoApplication::get_base_app_path, 'public', 'partial_update_bundle', 'upgrade_bundle_partial.zip'), ::Rho::RhoSupport.rhobundle_getfilename())
    bundle_unziped = (System.unzip_file(::Rho::RhoSupport.rhobundle_getfilename())==0)
    
    bundle_unziped.should == true
    
    puts 'START PARTIAL UPDATE'  
    System.replace_current_bundle( File.dirname(::Rho::RhoSupport.rhobundle_getfilename()), { :do_not_restart_app => true, :not_thread_mode => true } )
    puts 'FINISH PARTIAL UPDATE'  
  
      

    # check updated content

    File.exist?(File.join(public_root, 'folder_to_remove', 'switch.png')).should == false   
        
    File.exist?(File.join(public_root, 'file_for_replace.txt')).should == true   
        
    content = ''
    fileName = File.join(public_root, 'file_for_replace.txt')
    content = File.read(fileName) if File.exist?(fileName)
    #if File.exist?(fileName)
    #   File.open(fileName).each do |line|
    #      content = content + line
    #   end    
    #end
    content.should == 'replaced'    
            
    File.exist?(File.join(public_root, 'file_to_remove.png')).should == false   
            
    File.exist?(File.join(public_root, 'folder_to_remove.png')).should == true   
            
    File.exist?(File.join(public_root, 'added', 'added_image.png')).should == true   


  end

end
