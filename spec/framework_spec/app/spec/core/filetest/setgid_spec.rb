require File.dirname(File.join(__rhoGetCurrentDir(), __FILE__)) + '/../../spec_helper'
require File.dirname(File.join(__rhoGetCurrentDir(), __FILE__)) + '/../../shared/file/setgid'

describe "FileTest.setgid?" do
  it_behaves_like :file_setgid, :setgid?, FileTest
end
