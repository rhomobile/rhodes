require 'RSpec'
require 'FileUtils'
require_relative '../rhoDevelopment.rb'

describe 'RhofilelistItem' do

  before :each do

  end

  after :each do

  end


  it 'Should convert file extension for *.iseq when creating from string' do
    item = RhoDevelopment::RhofilelistItem.fromString('app/Settings/controller.iseq|file|7221|1409066890|151a40f97f6736e2dff24af38c312a81')
    expect(item.fixed_path).to eq('app/Settings/controller.rb')
  end

  it 'Should convert file extension for *_erb.iseq when creating from string' do
    item = RhoDevelopment::RhofilelistItem.fromString('app/Settings/controller_erb.iseq|file|7221|1409066890|151a40f97f6736e2dff24af38c312a81')
    expect(item.fixed_path).to eq('app/Settings/controller.erb')
  end

  it 'Should not converts file extension for non *_erb.iseq or *.iseq when creating from string' do
    item = RhoDevelopment::RhofilelistItem.fromString('app/Settings/controller.js|file|7221|1409066890|151a40f97f6736e2dff24af38c312a81')
    expect(item.fixed_path).to eq('app/Settings/controller.js')
  end

  it 'Should create instance with directory type from string' do
    item = RhoDevelopment::RhofilelistItem.fromString('app|dir|306|1427184297|')
    expect(item.path).to eq('app')
    expect(item.directory?).to eq(true)
    #expect(item.size).to eq(0)
    expect(item.datetime).to eq(1427184297)
  end

  it 'Should create instance with file type from string' do
    item = RhoDevelopment::RhofilelistItem.fromString('app/Settings/controller.iseq|file|7221|1409066890|151a40f97f6736e2dff24af38c312a81')
    expect(item.path).to eq('app/Settings/controller.iseq')
    expect(item.directory?).to eq(false)
    expect(item.size).to eq(7221)
    expect(item.datetime).to eq(1409066890)
    expect(item.hash).to eq('151a40f97f6736e2dff24af38c312a81')
  end

  context 'Comparing RhofilelistItems' do

    it 'Items with directory are equal if it has same path ' do
      item = RhoDevelopment::RhofilelistItem.fromString('app/Settings|dir|306|1427184297|')
      another_item = RhoDevelopment::RhofilelistItem.fromString('app/Settings|dir|306|1427184297|')
      expect(item == another_item).to be true
    end

    it 'Items with directory are not equal if it has different path ' do
      item = RhoDevelopment::RhofilelistItem.fromString('app/Settings|dir|306|1427184297|')
      another_item = RhoDevelopment::RhofilelistItem.fromString('app|dir|306|1427184297|')
      expect(item == another_item).to be false
    end

    it 'Items with file are equal if it has same hash ' do
      item = RhoDevelopment::RhofilelistItem.fromString('app/Settings/controller.iseq|file|7221|1409066890|151a40f97f6736e2dff24af38c312a81')
      another_item = RhoDevelopment::RhofilelistItem.fromString('app/Settings/controller.iseq|file|7221|1409066890|151a40f97f6736e2dff24af38c312a81')
      expect(item == another_item).to be true
    end

    it 'Items with file are not equal if it has different hash ' do
      item = RhoDevelopment::RhofilelistItem.fromString('app/Settings/controller.iseq|file|7221|1409066890|1')
      another_item = RhoDevelopment::RhofilelistItem.fromString('app/Settings/controller.iseq|file|7221|1409066890|2')
      expect(item == another_item).to be false
    end

  end

  context 'allowedForDetectingChanges' do

    it 'Should be allowed for detecting changes' do
      item = RhoDevelopment::RhofilelistItem.fromString('public/images/iphone/disclosure.png|file|47487|1409066890|e5941140e9bf7a79ce96aa732b850e27')
      expect(item.allowedForDetectingChanges?).to be true
    end

    it 'Should be not allowed for detecting changes if path is equal "rhoconfig.txt"' do
      item = RhoDevelopment::RhofilelistItem.fromString('rhoconfig.txt|file|47487|1409066890|e5941140e9bf7a79ce96aa732b850e27')
      expect(item.allowedForDetectingChanges?).to be false
    end

    it 'Should be not allowed for detecting changes if path is equal "rhoconfig.txt.timestamp"' do
      item = RhoDevelopment::RhofilelistItem.fromString('rhoconfig.txt.timestamp|file|47487|1409066890|e5941140e9bf7a79ce96aa732b850e27')
      expect(item.allowedForDetectingChanges?).to be false
    end

  end

end

describe 'RhofilelistLoader' do

  def temporary_directory
    'spec_temp'
  end

  before :each do
    if File.exist?(temporary_directory)
      FileUtils.rm_rf(temporary_directory)
    end
    Dir.mkdir(temporary_directory)

  end
  after :each do
    FileUtils.rm_rf(temporary_directory)
  end

  it 'Should create array with RhofilelistItem' do
    File.open(File.join(temporary_directory, 'rhofilelist.txt'), 'w') do |f|
      f.write('public/images/iphone/disclosure.png|file|47487|1409066890|e5941140e9bf7a79ce96aa732b850e27')
    end
    result = RhoDevelopment::RhofilelistLoader.loadFromFile(File.join(temporary_directory, 'rhofilelist.txt'))
    item = result[0]
    expect(item.path).to eq('public/images/iphone/disclosure.png')
    expect(item.file?).to be true
    expect(item.size).to eq(47487)
    expect(item.datetime).to eq(1409066890)
    expect(item.hash).to eq('e5941140e9bf7a79ce96aa732b850e27')
  end

  it 'Should reject item if path is equal "rhoconfig.txt"' do
    File.open(File.join(temporary_directory, 'rhofilelist.txt'), 'w') do |f|
      f.write('rhoconfig.txt|file|47487|1409066890|e5941140e9bf7a79ce96aa732b850e27')
    end
    result = RhoDevelopment::RhofilelistLoader.loadFromFile(File.join(temporary_directory, 'rhofilelist.txt'))
    expect(result.empty?).to be true
  end

  it 'Should reject item if path is equal "rhoconfig.txt.timestamp"' do
    File.open(File.join(temporary_directory, 'rhofilelist.txt'), 'w') do |f|
      f.write('rhoconfig.txt.timestamp|file|47487|1409066890|e5941140e9bf7a79ce96aa732b850e27')
    end
    result = RhoDevelopment::RhofilelistLoader.loadFromFile(File.join(temporary_directory, 'rhofilelist.txt'))
    expect(result.empty?).to be true
  end

end


describe 'extract_changes' do

  before :each do
    @added_files = []
    @removed_files = []
    @old_items = []
    @new_items = []
  end

  after :each do

  end

  context 'Defining added files' do

    it 'Should create array with added file' do
      @new_items = [RhoDevelopment::RhofilelistItem.fromString('public/added_file.rb|file|4757|1427184296|3f0a4671fd5cb654de7687b515f07cd0')]
      result = RhoDevelopment::extract_changes(@old_items, @new_items, @added_files, @removed_files)
      expect(@added_files.include?('public/added_file.rb')).to be true
      expect(result).to be true
    end

    it 'Should fix path for added *.iseq file' do
      @new_items = [RhoDevelopment::RhofilelistItem.fromString('public/added_file.iseq|file|4757|1427184296|3f0a4671fd5cb654de7687b515f07cd0')]
      RhoDevelopment::extract_changes(@old_items, @new_items, @added_files, @removed_files)
      expect(@added_files.include?('public/added_file.rb')).to be true
    end

    it 'Should fix path for added *_erb.iseq file' do
      @new_items = [RhoDevelopment::RhofilelistItem.fromString('public/added_file_erb.iseq|file|4757|1427184296|3f0a4671fd5cb654de7687b515f07cd0')]
      RhoDevelopment::extract_changes(@old_items, @new_items, @added_files, @removed_files)
      expect(@added_files.include?('public/added_file.erb')).to be true
    end

  end

  context 'Defining removed files' do

    it 'Should create array with removed file' do
      @old_items = [RhoDevelopment::RhofilelistItem.fromString('public/removed_file.rb|file|4757|1427184296|3f0a4671fd5cb654de7687b515f07cd0')]
      result = RhoDevelopment::extract_changes(@old_items, @new_items, @added_files, @removed_files)
      expect(@removed_files.include?('public/removed_file.rb')).to be true
      expect(result).to be true
    end

    it 'Should fix path for removed *_erb.iseq file' do
      @old_items = [RhoDevelopment::RhofilelistItem.fromString('public/removed_file_erb.iseq|file|4757|1427184296|3f0a4671fd5cb654de7687b515f07cd0')]
      RhoDevelopment::extract_changes(@old_items, @new_items, @added_files, @removed_files)
      expect(@removed_files.include?('public/removed_file.erb')).to be true
    end

    it 'Should fix path for removed *_rb.iseq file' do
      @old_items = [RhoDevelopment::RhofilelistItem.fromString('public/removed_file.iseq|file|4757|1427184296|3f0a4671fd5cb654de7687b515f07cd0')]
      RhoDevelopment::extract_changes(@old_items, @new_items, @added_files, @removed_files)
      expect(@removed_files.include?('public/removed_file.rb')).to be true
    end

  end

  context 'Defining changed files' do

    it 'Should not add file if size is different' do
      @new_items = [RhoDevelopment::RhofilelistItem.fromString('public/changed_file.rb|file|1|1427184296|3f0a4671fd5cb654de7687b515f07cd0')]
      @old_items = [RhoDevelopment::RhofilelistItem.fromString('public/changed_file.rb|file|2|1427184296|3f0a4671fd5cb654de7687b515f07cd0')]
      result = RhoDevelopment::extract_changes(@old_items, @new_items, @added_files, @removed_files)
      expect(@added_files.empty?).to be true
      expect(result).to be false
    end

    it 'Should not add file if datetime is different' do
      @new_items = [RhoDevelopment::RhofilelistItem.fromString('public/changed_file.rb|file|4757|1|3f0a4671fd5cb654de7687b515f07cd0')]
      @old_items = [RhoDevelopment::RhofilelistItem.fromString('public/changed_file.rb|file|4757|2|3f0a4671fd5cb654de7687b515f07cd0')]
      result = RhoDevelopment::extract_changes(@old_items, @new_items, @added_files, @removed_files)
      expect(@added_files.empty?).to be true
      expect(result).to be false
    end

    it 'Should not add file if hash is same' do
      @new_items = [RhoDevelopment::RhofilelistItem.fromString('public/changed_file.rb|file|4757|1427184296|3f0a4671fd5cb654de7687b515f07cd0')]
      @old_items = [RhoDevelopment::RhofilelistItem.fromString('public/changed_file.rb|file|4757|1427184296|3f0a4671fd5cb654de7687b515f07cd0')]
      result = RhoDevelopment::extract_changes(@old_items, @new_items, @added_files, @removed_files)
      expect(@added_files.empty?).to be true
      expect(result).to be false
    end

    it 'Should add file if hash is different' do
      @new_items = [RhoDevelopment::RhofilelistItem.fromString('public/changed_file.rb|file|4757|1427184296|1')]
      @old_items = [RhoDevelopment::RhofilelistItem.fromString('public/changed_file.rb|file|4757|1427184296|2')]
      result = RhoDevelopment::extract_changes(@old_items, @new_items, @added_files, @removed_files)
      expect(@added_files.empty?).to be false
      expect(@added_files.include?('public/changed_file.rb')).to be true
      expect(result).to be true
    end

  end

end