describe :file_readable, :shared => true do
  before :each do
    if System.get_property('platform') == 'ANDROID'
      @file = "#{ENV['TMP']}/i_exist"
    elsif System.get_property('platform') == 'WINDOWS'
      @file = "c:/Temp/i_exist"    
    else
      @file = "/tmp/i_exist"
    end
  end

  after :each do
    File.delete(@file) if File.exists?(@file)
  end
  it "returns true if named file is readable by the effective user id of the process, otherwise false" do
    platform_is_not :windows, :android do
      @object.send(@method, '/etc/passwd').should == true
    end
    platform_is :android do
      @object.send(@method, '/etc/hosts').should == true
    end
    File.open(@file,'w') { @object.send(@method, @file).should == true }
  end
end

describe :file_readable_missing, :shared => true do
  it "returns false if the file does not exist" do
    @object.send(@method, 'fake_file').should == false
  end
end
