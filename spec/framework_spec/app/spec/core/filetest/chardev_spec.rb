require File.dirname(File.join(__rhoGetCurrentDir(), __FILE__)) + '/../../spec_helper'
require File.dirname(File.join(__rhoGetCurrentDir(), __FILE__)) + '/../../shared/file/chardev'

describe "FileTest.chardev?" do
  it_behaves_like :file_chardev, :chardev?, FileTest
end 
