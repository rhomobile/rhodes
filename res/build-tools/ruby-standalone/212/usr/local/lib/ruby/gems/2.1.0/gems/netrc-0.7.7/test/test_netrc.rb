$VERBOSE = true
require 'test/unit'
require 'fileutils'

require File.expand_path("#{File.dirname(__FILE__)}/../lib/netrc")
require "rbconfig"

class TestNetrc < Test::Unit::TestCase

  def setup
    File.chmod(0600, "data/sample.netrc")
    File.chmod(0644, "data/permissive.netrc")
  end

  def test_parse_empty
    pre, items = Netrc.parse(Netrc.lex([]))
    assert_equal("", pre)
    assert_equal([], items)
  end

  def test_parse_file
    pre, items = Netrc.parse(Netrc.lex(IO.readlines("data/sample.netrc")))
    assert_equal("# this is my netrc\n", pre)
    exp = [["machine ",
            "m",
            "\n  login ",
            "l",
            " # this is my username\n  password ",
            "p",
            "\n"]]
    assert_equal(exp, items)
  end

  def test_login_file
    pre, items = Netrc.parse(Netrc.lex(IO.readlines("data/login.netrc")))
    assert_equal("# this is my login netrc\n", pre)
    exp = [["machine ",
            "m",
            "\n  login ",
            "l",
            " # this is my username\n"]]
    assert_equal(exp, items)
  end

  def test_password_file
    pre, items = Netrc.parse(Netrc.lex(IO.readlines("data/password.netrc")))
    assert_equal("# this is my password netrc\n", pre)
    exp = [["machine ",
            "m",
            "\n  password ",
            "p",
            " # this is my password\n"]]
    assert_equal(exp, items)
  end

  def test_missing_file
    n = Netrc.read("data/nonexistent.netrc")
    assert_equal(0, n.length)
  end

  def test_permission_error
    original_windows = Netrc::WINDOWS
    Netrc.const_set(:WINDOWS, false)
    Netrc.read("data/permissive.netrc")
  rescue Netrc::Error
    assert true, "Should raise an error if permissions are wrong on a non-windows system."
  ensure
    Netrc.const_set(:WINDOWS, original_windows)
  end

  def test_permission_error_windows
    original_windows = Netrc::WINDOWS
    Netrc.const_set(:WINDOWS, true)
    Netrc.read("data/permissive.netrc")
  rescue Netrc::Error
    assert false, "Should not raise an error if permissions are wrong on a non-windows system."
  ensure
    Netrc.const_set(:WINDOWS, original_windows)
  end

  def test_round_trip
    n = Netrc.read("data/sample.netrc")
    assert_equal(IO.read("data/sample.netrc"), n.unparse)
  end

  def test_set
    n = Netrc.read("data/sample.netrc")
    n["m"] = "a", "b"
    exp = "# this is my netrc\n"+
          "machine m\n"+
          "  login a # this is my username\n"+
          "  password b\n"
    assert_equal(exp, n.unparse)
  end

  def test_set_get
    n = Netrc.read("data/sample.netrc")
    n["m"] = "a", "b"
    assert_equal(["a", "b"], n["m"])
  end

  def test_add
    n = Netrc.read("data/sample.netrc")
    n.new_item_prefix = "# added\n"
    n["x"] = "a", "b"
    exp = "# this is my netrc\n"+
          "machine m\n"+
          "  login l # this is my username\n"+
          "  password p\n"+
          "# added\n"+
          "machine x\n"+
          "  login a\n"+
          "  password b\n"
    assert_equal(exp, n.unparse)
  end

  def test_add_newlineless
    n = Netrc.read("data/newlineless.netrc")
    n.new_item_prefix = "# added\n"
    n["x"] = "a", "b"
    exp = "# this is my netrc\n"+
          "machine m\n"+
          "  login l # this is my username\n"+
          "  password p\n"+
          "# added\n"+
          "machine x\n"+
          "  login a\n"+
          "  password b\n"
    assert_equal(exp, n.unparse)
  end

  def test_add_get
    n = Netrc.read("data/sample.netrc")
    n.new_item_prefix = "# added\n"
    n["x"] = "a", "b"
    assert_equal(["a", "b"], n["x"])
  end

  def test_get_missing
    n = Netrc.read("data/sample.netrc")
    assert_equal(nil, n["x"])
  end

  def test_save
    n = Netrc.read("data/sample.netrc")
    n.save
    assert_equal(File.read("data/sample.netrc"), n.unparse)
  end

  def test_save_create
    FileUtils.rm_f("/tmp/created.netrc")
    n = Netrc.read("/tmp/created.netrc")
    n.save
    unless Netrc::WINDOWS
      assert_equal(0600, File.stat("/tmp/created.netrc").mode & 0777)
    end
  end

  def test_encrypted_roundtrip
    if `gpg --list-keys 2> /dev/null` != ""
      FileUtils.rm_f("/tmp/test.netrc.gpg")
      n = Netrc.read("/tmp/test.netrc.gpg")
      n["m"] = "a", "b"
      n.save
      assert_equal(0600, File.stat("/tmp/test.netrc.gpg").mode & 0777)
      assert_equal(["a", "b"], Netrc.read("/tmp/test.netrc.gpg")["m"])
    end
  end

  def test_missing_environment
    nil_home = nil
    ENV["HOME"], nil_home = nil_home, ENV["HOME"]
    n = Netrc.read
    assert_equal(nil, n["x"])
  ensure
    ENV["HOME"], nil_home = nil_home, ENV["HOME"]
  end


end
