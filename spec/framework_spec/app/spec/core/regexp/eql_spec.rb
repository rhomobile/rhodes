require File.dirname(File.join(__rhoGetCurrentDir(), __FILE__)) + '/../../spec_helper'
require File.dirname(File.join(__rhoGetCurrentDir(), __FILE__)) + '/shared/equal_value'

describe "Regexp#eql?" do
  it_behaves_like(:regexp_eql, :eql?)
end
