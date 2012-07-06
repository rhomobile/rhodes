#require File.expand_path('../../../../spec_helper', __FILE__)
#require File.expand_path('../shared/random_bytes.rb', __FILE__)
require File.dirname(File.join(__rhoGetCurrentDir(), __FILE__)) + '/shared/random_bytes'

describe "OpenSSL::Random#random_bytes" do
  it_behaves_like :openssl_random_bytes, :random_bytes
end
