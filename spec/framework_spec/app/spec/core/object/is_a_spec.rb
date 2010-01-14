require File.dirname(File.join(__rhoGetCurrentDir(), __FILE__)) + '/../../spec_helper'
require File.dirname(File.join(__rhoGetCurrentDir(), __FILE__)) + '/shared/kind_of'

describe "Object#is_a?" do
  it_behaves_like(:object_kind_of , :is_a?)
end
