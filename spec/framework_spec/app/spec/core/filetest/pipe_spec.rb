require File.dirname(File.join(__rhoGetCurrentDir(), __FILE__)) + '/../../spec_helper'
require File.dirname(File.join(__rhoGetCurrentDir(), __FILE__)) + '/../../shared/file/pipe'

describe "FileTest.pipe?" do
  it_behaves_like :file_pipe, :pipe?, FileTest
end
