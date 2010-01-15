require File.dirname(File.join(__rhoGetCurrentDir(), __FILE__)) + '/../spec_helper'

# specs for File.join(__rhoGetCurrentDir(), __FILE__)

describe "The __FILE__ constant" do
  it "equals the current filename" do
    File.basename(File.join(__rhoGetCurrentDir(), __FILE__)).should == "file_spec.rb"
  end

# XXX eval not supported
#  it "equals (eval) inside an eval" do
#    eval("File.join(__rhoGetCurrentDir(), __FILE__)").should == "(eval)"
#  end
  
  it "equals a relative path when required using a relative path" do
    base_path = File.dirname(File.dirname(fixture(File.join(__rhoGetCurrentDir(), __FILE__), "file.rb")))
    path = "fixtures/file.rb"
    Dir.chdir(base_path) do
      require path
      ScratchPad.recorded.should == File.join(".",path)
    end
  end
  
  it "equals the full path when required using a full path" do
    path = fixture(File.join(__rhoGetCurrentDir(), __FILE__), "file.rb")
    require path
    ScratchPad.recorded.should == path
  end
end


describe "The File.join(__rhoGetCurrentDir(), __FILE__) constant" do
  before(:each) do
    path = fixture(File.join(__rhoGetCurrentDir(), __FILE__),"file.rb")
    #puts "@@@@ Path is #{path} for fixture(#{File.join(__rhoGetCurrentDir(), __FILE__)},'file.rb')"
    $:.unshift File.dirname(path)
  end
  after(:each) do
    $:.shift
  end
  
  it "equals the full path to the file when required" do
    require 'file.rb'
    ScratchPad.recorded.should == fixture(File.join(__rhoGetCurrentDir(), __FILE__), 'file.rb')
  end
  
end
