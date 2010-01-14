require File.dirname(File.join(__rhoGetCurrentDir(), __FILE__)) + '/../../spec_helper'
require File.dirname(File.join(__rhoGetCurrentDir(), __FILE__)) + '/../../shared/file/blockdev'

describe "File.blockdev?" do
  it_behaves_like :file_blockdev, :blockdev?, File
end
