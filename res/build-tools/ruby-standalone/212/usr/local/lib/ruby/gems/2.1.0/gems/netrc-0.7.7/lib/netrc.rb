require 'rbconfig'

class Netrc
  VERSION = "0.7.7"

  # see http://stackoverflow.com/questions/4871309/what-is-the-correct-way-to-detect-if-ruby-is-running-on-windows
  WINDOWS = RbConfig::CONFIG["host_os"] =~ /mswin|mingw|cygwin/
  CYGWIN  = RbConfig::CONFIG["host_os"] =~ /cygwin/

  def self.default_path
    if WINDOWS && !CYGWIN
      File.join(ENV['USERPROFILE'].gsub("\\","/"), "_netrc")
    else
      File.join((ENV["HOME"] || "./"), ".netrc")
    end
  end

  def self.check_permissions(path)
    perm = File.stat(path).mode & 0777
    if perm != 0600 && !(WINDOWS)
      raise Error, "Permission bits for '#{path}' should be 0600, but are "+perm.to_s(8)
    end
  end

  # Reads path and parses it as a .netrc file. If path doesn't
  # exist, returns an empty object. Decrypt paths ending in .gpg.
  def self.read(path=default_path)
    check_permissions(path)
    data = if path =~ /\.gpg$/
      decrypted = `gpg --batch --quiet --decrypt #{path}`
      if $?.success?
        decrypted
      else
        raise Error.new("Decrypting #{path} failed.") unless $?.success?
      end
    else
      File.read(path)
    end
    new(path, parse(lex(data.lines.to_a)))
  rescue Errno::ENOENT
    new(path, parse(lex([])))
  end

  def self.lex(lines)
    tokens = []
    for line in lines
      content, comment = line.split(/(\s*#.*)/m)
      content.each_char do |char|
        case char
        when /\s/
          if tokens.last && tokens.last[-1..-1] =~ /\s/
            tokens.last << char
          else
            tokens << char
          end
        else
          if tokens.last && tokens.last[-1..-1] =~ /\S/
            tokens.last << char
          else
            tokens << char
          end
        end
      end
      if comment
        tokens << comment
      end
    end
    tokens
  end

  def self.skip?(s)
    s =~ /^\s/
  end

  # Returns two values, a header and a list of items.
  # Each item is a tuple, containing some or all of:
  # - machine keyword (including trailing whitespace+comments)
  # - machine name
  # - login keyword (including surrounding whitespace+comments)
  # - login
  # - password keyword (including surrounding whitespace+comments)
  # - password
  # - trailing chars
  # This lets us change individual fields, then write out the file
  # with all its original formatting.
  def self.parse(ts)
    cur, item = [], []

    def ts.take
      if length < 1
        raise Error, "unexpected EOF"
      end
      shift
    end

    def ts.readto
      l = []
      while length > 0 && ! yield(self[0])
        l << shift
      end
      return l.join
    end

    pre = ts.readto{|t| t == "machine"}
    while ts.length > 0
      cur << ts.take + ts.readto{|t| ! skip?(t)}
      cur << ts.take
      if ts.include?('login')
        cur << ts.readto{|t| t == "login"} + ts.take + ts.readto{|t| ! skip?(t)}
        cur << ts.take
      end
      if ts.include?('password')
        cur << ts.readto{|t| t == "password"} + ts.take + ts.readto{|t| ! skip?(t)}
        cur << ts.take
      end
      cur << ts.readto{|t| t == "machine"}
      item << cur
      cur = []
    end

    [pre, item]
  end

  def initialize(path, data)
    @new_item_prefix = ''
    @path = path
    @pre, @data = data
  end

  attr_accessor :new_item_prefix

  def [](k)
    if item = @data.detect {|datum| datum[1] == k}
      [item[3], item[5]]
    end
  end

  def []=(k, info)
    if item = @data.detect {|datum| datum[1] == k}
      item[3], item[5] = info
    else
      @data << new_item(k, info[0], info[1])
    end
  end

  def length
    @data.length
  end

  def delete(key)
    datum = nil
    for value in @data
      if value[1] == key
        datum = value
        break
      end
    end
    @data.delete(datum)
  end

  def each(&block)
    @data.each(&block)
  end

  def new_item(m, l, p)
    [new_item_prefix+"machine ", m, "\n  login ", l, "\n  password ", p, "\n"]
  end

  def save
    if @path =~ /\.gpg$/
      e = IO.popen("gpg -a --batch --default-recipient-self -e", "r+") do |gpg|
        gpg.puts(unparse)
        gpg.close_write
        gpg.read
      end
      raise Error.new("Encrypting #{path} failed.") unless $?.success?
      File.open(@path, 'w', 0600) {|file| file.print(e)}
    else
      File.open(@path, 'w', 0600) {|file| file.print(unparse)}
    end
  end

  def unparse
    @pre + @data.map do |datum|
      datum = datum.join
      unless datum[-1..-1] == "\n"
        datum << "\n"
      else
        datum
      end
    end.join
  end

end

class Netrc::Error < ::StandardError
end
