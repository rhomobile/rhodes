Testing Mac do
  $data.each do |basename, output|
    testing "#{ basename } output" do
      expected = basename

      macaddr = assert{ Mac.parse(output) }

      assert{ macaddr.is_a?(String) }
      assert{ macaddr =~ Mac::RE }
      assert{ macaddr.list.is_a?(Array) }
      assert{ macaddr == expected }
    end
  end


  testing ".addr" do
    assert{ Mac.addr }
  end
end


### c8\:bc\:c8\:9b\:28\:b1

BEGIN {
  $testdir = File.dirname(File.expand_path(__FILE__))
  $rootdir = File.dirname($testdir)
  $libdir = File.join($rootdir, 'lib')
  require File.join($libdir, 'macaddr')
  require File.join($testdir, 'testing')

  $datadir = File.join($testdir, 'data')
  $data = {}

  {
    'c8:bc:c8:9b:28:b1' => 'osx',
    'd0:4e:f1:68:29:79' => 'noifconfig'
  }.each do |expected, basename|
    entry = File.join($datadir, basename)
    $data[expected] = IO.read(entry)
  end
}
