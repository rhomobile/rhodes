require 'RSpec'
require 'FileUtils'
require_relative '../jake'

describe 'Zip/Unzip' do

  def temporary_directory
    'spec_temp'
  end

  before :each do
    if File.exists?(temporary_directory)
      FileUtils.rm_rf(temporary_directory)
    end
    Dir.mkdir(temporary_directory)
  end
  after :each do
    FileUtils.rm_rf(temporary_directory)
  end


  it 'Zipping file' do
    expect(true).to eq(false)
  end

  it 'Unzipping archieve' do
    Jake.unzip(Dir.pwd + '/archive.zip', temporary_directory)
    files = Dir[temporary_directory + '/*.txt']
    expect(files.length).to eq(2)
    expect(files[0]).to eq('spec_temp/abc.txt')
    expect(files[1]).to eq('spec_temp/def.txt')
  end
end