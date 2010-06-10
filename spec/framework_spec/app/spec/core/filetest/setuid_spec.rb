require File.dirname(File.join(__rhoGetCurrentDir(), __FILE__)) + '/../../spec_helper'
require File.dirname(File.join(__rhoGetCurrentDir(), __FILE__)) + '/../../shared/file/setuid'

describe "FileTest.setuid?" do
  it_behaves_like :file_setuid, :setuid?, FileTest
end
