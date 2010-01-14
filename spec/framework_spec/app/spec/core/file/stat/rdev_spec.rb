require File.dirname(File.join(__rhoGetCurrentDir(), __FILE__)) + '/../../../spec_helper'

describe 'File::Stat#rdev' do
  it "returns the number of the device this file represents which the file exists" do
    File.stat('/dev/null').rdev.should be_kind_of(Integer)
  end
end
