require File.dirname(File.join(__rhoGetCurrentDir(), __FILE__)) + '/../../spec_helper'
require File.dirname(File.join(__rhoGetCurrentDir(), __FILE__)) + '/fixtures/common'
require File.dirname(File.join(__rhoGetCurrentDir(), __FILE__)) + '/shared/delete'

describe "Dir.rmdir" do
  it_behaves_like :dir_delete, :rmdir
end
