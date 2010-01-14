require File.dirname(File.join(__rhoGetCurrentDir(), __FILE__)) + '/../../spec_helper'
require File.dirname(File.join(__rhoGetCurrentDir(), __FILE__)) + '/../../shared/file/zero'

describe "File.zero?" do
  it_behaves_like :file_zero, :zero?, File
  it_behaves_like :file_zero_missing, :zero?, File
end
