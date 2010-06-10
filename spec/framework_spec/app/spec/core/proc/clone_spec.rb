require File.dirname(File.join(__rhoGetCurrentDir(), __FILE__)) + '/../../spec_helper'
require File.dirname(File.join(__rhoGetCurrentDir(), __FILE__)) + '/shared/dup'

describe "Proc#clone" do
  it_behaves_like(:proc_dup, :clone)
end