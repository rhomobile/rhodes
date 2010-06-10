require File.dirname(File.join(__rhoGetCurrentDir(), __FILE__)) + '/../../spec_helper'
require File.dirname(File.join(__rhoGetCurrentDir(), __FILE__)) + '/shared/modulo'

describe "Float#%" do
  it_behaves_like(:float_modulo, :%)
end

describe "Float#modulo" do
  it_behaves_like(:float_modulo, :modulo)
end
