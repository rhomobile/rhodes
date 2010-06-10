require File.dirname(File.join(__rhoGetCurrentDir(), __FILE__)) + '/../../spec_helper'
require File.dirname(File.join(__rhoGetCurrentDir(), __FILE__)) + '/../../shared/file/exist'

describe "FileTest.exist?" do
  it_behaves_like :file_exist, :exist?, FileTest
end
