require File.dirname(File.join(__rhoGetCurrentDir(), __FILE__)) + '/../../spec_helper'
require File.dirname(File.join(__rhoGetCurrentDir(), __FILE__)) + '/shared/new'

describe "IO.for_fd" do
  it_behaves_like :io_new, :for_fd
end
