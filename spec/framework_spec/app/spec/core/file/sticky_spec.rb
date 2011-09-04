require File.expand_path('../../../spec_helper', __FILE__)
require File.expand_path('../../../shared/file/sticky', __FILE__)

describe "File.sticky?" do
  it_behaves_like :file_sticky, :sticky?, File
  it_behaves_like :file_sticky_missing, :sticky?, File
end

describe "File.sticky?" do
  it "returns false if file does not exist" do
    if System.get_property('platform') == 'WINDOWS'  
        File.sticky?("I_am_a_bogus_file").should == nil
    else
        File.sticky?("I_am_a_bogus_file").should == false
    end    
  end

  it "returns false if the file has not sticky bit set" do
    filename = tmp("i_exist")
    touch(filename)

    if System.get_property('platform') == 'WINDOWS'  
        File.sticky?(filename).should == nil
    else
        File.sticky?(filename).should == false
    end        
    
    rm_r filename
  end

  platform_is :linux, :darwin do
    it "returns true if the file has sticky bit set" do
      filename = tmp("i_exist")
      touch(filename)
      system "chmod +t #{filename}"
    
      File.sticky?(filename).should == false

      rm_r filename
    end
  end

  platform_is :bsd do
    # FreeBSD and NetBSD can't set stiky bit to a normal file
    it "cannot set sticky bit to a normal file" do
      filename = tmp("i_exist")
      touch(filename)
      stat = File.stat(filename)
      mode = stat.mode
      raise_error(Errno::EFTYPE){File.chmod(mode|01000, filename)}
      File.sticky?(filename).should == false

      rm_r filename
    end
  end
end
