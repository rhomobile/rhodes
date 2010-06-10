require File.dirname(File.join(__rhoGetCurrentDir(), __FILE__)) + '/../../spec_helper'
require File.dirname(File.join(__rhoGetCurrentDir(), __FILE__)) + '/../../shared/file/identical'

describe "FileTest.identical?" do
  it_behaves_like :file_identical, :identical?, FileTest
end
