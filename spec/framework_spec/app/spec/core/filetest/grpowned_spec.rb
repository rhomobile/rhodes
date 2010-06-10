require File.dirname(File.join(__rhoGetCurrentDir(), __FILE__)) + '/../../spec_helper'
require File.dirname(File.join(__rhoGetCurrentDir(), __FILE__)) + '/../../shared/file/grpowned'

describe "FileTest.grpowned?" do
  it_behaves_like :file_grpowned, :grpowned?, FileTest
end
