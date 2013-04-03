describe 'Log' do

  # 
  before(:all) do
    @default_level = Rho::Log.level

    # test contstants
    @categories = [ 
      'alpha', 'beta', 'gamma', 'delta',
      'epsilon', 'zeta', 'eta', 'theta',
      'iota', 'kappa', 'lambda', 'mu', 
      'nu', 'xi', 'omicron', 'pi', 
      'rho', 'sigma', 'tau', 'upsilon', 
      'phi', 'chi', 'psi', 'omega'
    ]; 

    # available severity levels
    @levels = [
      Rho::Log::LEVEL_TRACE,
      Rho::Log::LEVEL_INFO,
      Rho::Log::LEVEL_WARNING,
      Rho::Log::LEVEL_ERROR,
      Rho::Log::LEVEL_FATAL
    ];

    # one should not invoke fatal level traces
    @tested_levels = [
      Rho::Log::LEVEL_TRACE,
      Rho::Log::LEVEL_INFO,
      Rho::Log::LEVEL_WARNING,
      Rho::Log::LEVEL_ERROR
    ];

    # severity to function name map
    @severetyFuncMap = { 
      Rho::Log::LEVEL_TRACE => :trace,
      Rho::Log::LEVEL_INFO => :info,
      Rho::Log::LEVEL_WARNING => :warning,
      Rho::Log::LEVEL_ERROR => :error,
      Rho::Log::LEVEL_FATAL => :fatalError,
    };

    # for setting/reseting
    @categoriesDefaults = [
      ['includeCategories','*'], 
      ['excludeCategories',''],
      ['excludeFilter',''] ];
  end

  # reset to default values after each test
  after(:each) do
    @categoriesDefaults.each do |propTest|
      property = propTest[0]
      default = propTest[1]
      Rho::Log.send("#{property}=", default) 
    end
    Rho::Log.level = @default_level
  end

  it 'should read write properties' do
    Rho::Log.should_not be_nil

    #level
    Rho::Log.level.should_not be_nil

    Rho::Log.level = 5
    Rho::Log.level.should == 5
    Rho::Log.level = 0
    Rho::Log.level.should == 0

    Rho::Log.level = @default_level

    #category filters
    @categoriesDefaults.each do |propTest|
      property = propTest[0]

      default  = Rho::Log.send(property)

      default.should_not be_nil
      default.should == propTest[1]

      Rho::Log.send("#{property}=", '')
      Rho::Log.send(property).should == ''
      Rho::Log.send("#{property}=", '*')
      Rho::Log.send(property).should == '*'

      Rho::Log.send("#{property}=", default) 
    end
  end

  #constatn tests
  it 'should have severity level constants' do
    Rho::Log::LEVEL_TRACE.should == 0
    Rho::Log::LEVEL_INFO.should == 1
    Rho::Log::LEVEL_WARNING.should == 2
    Rho::Log::LEVEL_ERROR.should == 3
    Rho::Log::LEVEL_FATAL.should == 4
  end

  it 'should have destination constants' do
    Rho::Log::DEST_FILE.should == 'file'
    Rho::Log::DEST_OUTPUT.should == 'stdio'
    Rho::Log::DEST_URI.should == 'uri'
  end

  # log tests
  it 'should clean log' do
    Rho::Log.cleanLogFile
    log = Rho::Log.readLogFile(1024*1024*16)
    log.should == nil
  end

  it 'should write log' do
    Rho::Log.level = Rho::Log::LEVEL_TRACE
    Rho::Log.trace('test message','APP')
    Rho::Log.readLogFile(1024*1024*16).length.should_not == 0
  end

  # filter tests
  it 'should filter by severity level' do
    Rho::Log.cleanLogFile
    messageCounter = 0
    # test for each availabale trace level
    @levels.each do |trace_level|
      Rho::Log.level = trace_level
      # trace messages with this level
      @tested_levels.each do |test_level|
        shouldBeVisible = test_level >= trace_level
        messageCounter += 1 if shouldBeVisible
        Rho::Log.send(@severetyFuncMap[test_level], shouldBeVisible ? 'log_ok' : "log_fail", 'APP')
      end
    end

    #read log and check if there are any exeptions
    log = Rho::Log.readLogFile(1024*1024*16)
    log.length.should_not == 0
    log.index("log_fail").should == nil
    log.scan(/log_ok/).length.should == messageCounter
  end

  it 'should filter by include categories' do
    Rho::Log.level = Rho::Log::LEVEL_TRACE
    Rho::Log.cleanLogFile

    includeCats =  @categories.values_at(* @categories.each_index.select {|i| i.even?})
    Rho::Log.includeCategories = includeCats.join(',')

    messageCounter = 0

    # test for each availabale trace level
    @categories.each do |category|
      shouldBeVisible = includeCats.include?(category)
      messageCounter += 1 if shouldBeVisible
      Rho::Log.trace(shouldBeVisible ? 'log_ok' : "log_fail", category)
    end

    #read log and check if there are any exeptions
    log = Rho::Log.readLogFile(1024*1024*16)
    log.length.should_not == 0
    log.index("log_fail").should == nil
    log.scan(/log_ok/).length.should == messageCounter
  end

  it 'should filter by exclude categories' do
    Rho::Log.level = Rho::Log::LEVEL_TRACE
    Rho::Log.cleanLogFile

    excludeCats =  @categories.values_at(* @categories.each_index.select {|i| i.even?})
    Rho::Log.excludeCategories = excludeCats.join(',')

    messageCounter = 0

    # test for each availabale trace level
    @categories.each do |category|
      shouldBeVisible = !(excludeCats.include?(category))
      messageCounter += 1 if shouldBeVisible
      Rho::Log.trace(shouldBeVisible ? 'log_ok' : "log_fail", category)
    end

    #read log and check if there are any exeptions
    log = Rho::Log.readLogFile(1024*1024*16)
    log.length.should_not == 0
    log.index("log_fail").should == nil
    log.scan(/log_ok/).length.should == messageCounter
  end

  it 'should filter by exclude and include categories' do
    Rho::Log.level = Rho::Log::LEVEL_TRACE
    Rho::Log.cleanLogFile

    includeCats =  @categories.values_at(* @categories.each_index.select {|i| i.even?})
    excludeCats =  @categories.values_at(* @categories.each_index.select {|i| (i>>1).odd?})
    Rho::Log.includeCategories = includeCats.join(',')
    Rho::Log.excludeCategories = excludeCats.join(',')

    messageCounter = 0

    # test for each availabale trace level
    @categories.each do |category|
      shouldBeVisible = (includeCats.include?(category) && (!excludeCats.include?(category)))
      messageCounter += 1 if shouldBeVisible
      Rho::Log.trace(shouldBeVisible ? 'log_ok' : "log_fail", category)
    end

    #read log and check if there are any exeptions
    log = Rho::Log.readLogFile(1024*1024*16)
    log.length.should_not == 0
    log.index("log_fail").should == nil
    log.scan(/log_ok/).length.should == messageCounter
  end

  it 'should filter by excludeFilter' do
    Rho::Log.level = Rho::Log::LEVEL_TRACE
    Rho::Log.cleanLogFile

    puts Rho::Log.excludeFilter

    Rho::Log.excludeFilter = ",,,password ,key, zombie ,,,  ,    "
    filter_value = Rho::Log.excludeFilter
    filter_value.should == "password,key,zombie,  ,    "

    puts "Filter = '#{Rho::Log.excludeFilter}'"

    struct = {
      "alpha" => "log_ok",
      "beta" => "log_ok",
      "password" => "log_fail",
      "key" => "log_fail",
      "secretkey" => "log_fail",
      "zombie" => "log_fail",
      "key" => 128,
      "PaSsWoRd" => "log_ok",
      "zombies" => "wtf"
    };

    ok_counter = 0

    struct.each do |key, val|
      Rho::Log.trace("#{key}=#{val}","APP")
      ok_counter += 1 if val == "log_ok"
    end

    Rho::Log.trace(struct.to_s,"APP")
    
    messageCounter = ok_counter*2 #key=val and struct.to_s

    #read log and check if there are any exeptions
    log = Rho::Log.readLogFile(1024*1024*16)
    log.length.should_not == 0
    log.index("log_fail").should == nil
    log.scan(/log_ok/).length.should == messageCounter
  end


end