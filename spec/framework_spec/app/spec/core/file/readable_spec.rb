require File.dirname(File.join(__rhoGetCurrentDir(), __FILE__)) + '/../../spec_helper'
require File.dirname(File.join(__rhoGetCurrentDir(), __FILE__)) + '/../../shared/file/readable'

describe "File.readable?" do
  it_behaves_like :file_readable, :readable?, File
  it_behaves_like :file_readable_missing, :readable?, File
end
