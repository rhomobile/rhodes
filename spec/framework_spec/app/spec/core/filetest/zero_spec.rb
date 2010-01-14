require File.dirname(File.join(__rhoGetCurrentDir(), __FILE__)) + '/../../spec_helper'
require File.dirname(File.join(__rhoGetCurrentDir(), __FILE__)) + '/../../shared/file/zero'

describe "FileTest.zero?" do
  it_behaves_like :file_zero, :zero?, FileTest
  it_behaves_like :file_zero_missing, :zero?, FileTest
end
