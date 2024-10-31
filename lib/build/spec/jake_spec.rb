require 'RSpec'
require 'FileUtils'
require_relative '../jake'

def temporary_directory
  'spec_temp'
end

describe 'Zipping' do

  before :each do
    if File.exist?(temporary_directory)
      FileUtils.rm_rf(temporary_directory)
    end
    Dir.mkdir(temporary_directory)

  end
  after :each do
    FileUtils.rm_rf(temporary_directory)
  end

  def create_files_for_zipping
    File.open(File.join(temporary_directory, 'abc.txt'), 'w') do |f|
      f.write('abc')
    end
    File.open(File.join(temporary_directory, 'def.txt'), 'w') do |f|
      f.write('def')
    end
  end

  it 'Zipping file to nonexisting zip file' do
    create_files_for_zipping
    archive_filename = File.join(Dir.pwd, temporary_directory, 'archive.zip')
    Jake.zip(temporary_directory, ['abc.txt', 'def.txt'], archive_filename)
    expect(File.exist?(archive_filename)).to eq(true)
  end

  it 'Zipping file to existing file' do
    create_files_for_zipping
    archive_filename = File.join(Dir.pwd, temporary_directory, 'archive.zip')
    File.open(archive_filename, 'w') do |f|
      f.write('Existing archive')
    end
    Jake.zip(temporary_directory, ['abc.txt', 'def.txt'], archive_filename)
    expect(File.exist?(archive_filename)).to eq(true)
  end
end


describe 'Unzipping' do

  before :each do
    if File.exist?(temporary_directory)
      FileUtils.rm_rf(temporary_directory)
    end
    Dir.mkdir(temporary_directory)
  end

  after :each do
    FileUtils.rm_rf(temporary_directory)
  end

  it 'Unzipping archieve with two files to existing directory' do
    Jake.unzip(File.join(Dir.pwd, 'archive.zip'), temporary_directory)
    files = Dir[temporary_directory + '/*.txt']
    expect(files.length).to eq(2)
    expect(files[0]).to eq('spec_temp/abc.txt')
    expect(files[1]).to eq('spec_temp/def.txt')
  end

  it 'Unzipping archieve with two files to unexisting directory' do
    if File.exist?(temporary_directory)
      FileUtils.rm_rf(temporary_directory)
    end
    Jake.unzip(Dir.pwd + '/archive.zip', temporary_directory)
    files = Dir[temporary_directory + '/*.txt']
    expect(files.length).to eq(2)
    expect(files[0]).to eq('spec_temp/abc.txt')
    expect(files[1]).to eq('spec_temp/def.txt')
  end

  it 'Unzipping archieve with block handling' do
    done = 0
    total = 0
    message = ''
    Jake.unzip(Dir.pwd + '/archive.zip', temporary_directory) do |bytes_done, total_bytes, msg|
      done = bytes_done
      total = total_bytes
      message = msg
    end
    expect(done).to eq(total)
    expect(message).to eq('Unpacking files: 100%')
  end
end