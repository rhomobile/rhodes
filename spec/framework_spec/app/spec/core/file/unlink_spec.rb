require File.dirname(File.join(__rhoGetCurrentDir(), __FILE__)) + '/../../spec_helper'
require File.dirname(File.join(__rhoGetCurrentDir(), __FILE__)) + '/shared/unlink'

describe "File.unlink" do
  it_behaves_like(:file_unlink, :unlink)
end
