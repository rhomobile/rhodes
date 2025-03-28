
#
# It is auto-generated content.
# Do not do required for this file in your application.
#

class File
  Separator = "/"

  SEPARATOR = "/"

  ALT_SEPARATOR = "\\"

  PATH_SEPARATOR = ";"

  module Constants
  end

  class Stat
    def <=>(req)
    end
    def dev
    end
    def dev_major
    end
    def dev_minor
    end
    def ino
    end
    def mode
    end
    def nlink
    end
    def uid
    end
    def gid
    end
    def rdev
    end
    def rdev_major
    end
    def rdev_minor
    end
    def size
    end
    def blksize
    end
    def blocks
    end
    def atime
    end
    def mtime
    end
    def ctime
    end
    def inspect
    end
    def ftype
    end
    def directory?
    end
    def readable?
    end
    def readable_real?
    end
    def world_readable?
    end
    def writable?
    end
    def writable_real?
    end
    def world_writable?
    end
    def executable?
    end
    def executable_real?
    end
    def file?
    end
    def zero?
    end
    def size?
    end
    def owned?
    end
    def grpowned?
    end
    def pipe?
    end
    def symlink?
    end
    def socket?
    end
    def blockdev?
    end
    def chardev?
    end
    def setuid?
    end
    def setgid?
    end
    def sticky?
    end
  end

  module WaitReadable
  end

  module WaitWritable
  end

  SEEK_SET = 0

  SEEK_CUR = 1

  SEEK_END = 2

  LOCK_SH = 1

  LOCK_EX = 2

  LOCK_UN = 8

  LOCK_NB = 4

  RDONLY = 0

  WRONLY = 1

  RDWR = 2

  APPEND = 8

  CREAT = 256

  EXCL = 1024

  NONBLOCK = 4

  TRUNC = 512

  BINARY = 32768

  FNM_NOESCAPE = 1

  FNM_PATHNAME = 2

  FNM_DOTMATCH = 4

  FNM_CASEFOLD = 8

  FNM_SYSCASE = 8

  def self.binread(fileName)
  end
  def self.new(fileName)
  end
  def self.open(fileName)
  end
  def self.write(content)
  end
  def self.close
  end
  def self.directory?(req)
  end
  def self.exists?(req)
  end
  def self.exists?(req)
  end
  def self.readable?(req)
  end
  def self.readable_real?(req)
  end
  def self.world_readable?(req)
  end
  def self.writable?(req)
  end
  def self.writable_real?(req)
  end
  def self.world_writable?(req)
  end
  def self.executable?(req)
  end
  def self.executable_real?(req)
  end
  def self.file?(req)
  end
  def self.zero?(req)
  end
  def self.size?(req)
  end
  def self.size(req)
  end
  def self.owned?(req)
  end
  def self.grpowned?(req)
  end
  def self.pipe?(req)
  end
  def self.symlink?(req)
  end
  def self.socket?(req)
  end
  def self.blockdev?(req)
  end
  def self.chardev?(req)
  end
  def self.setuid?(req)
  end
  def self.setgid?(req)
  end
  def self.sticky?(req)
  end
  def self.identical?(req,req1)
  end
  def self.stat(req)
  end
  def self.lstat(req)
  end
  def self.ftype(req)
  end
  def self.atime(req)
  end
  def self.mtime(req)
  end
  def self.ctime(req)
  end
  def self.utime(rest)
  end
  def self.chmod(rest)
  end
  def self.chown(rest)
  end
  def self.lchmod
  end
  def self.lchown
  end
  def self.link(req,req1)
  end
  def self.symlink
  end
  def self.readlink
  end
  def self.unlink(rest)
  end
  def self.delete(rest)
  end
  def self.rename(req,req1)
  end
  def self.umask(rest)
  end
  def self.truncate(req,req1)
  end
  def self.expand_path(rest)
  end
  def self.absolute_path(rest)
  end
  def self.realpath(rest)
  end
  def self.realdirpath(rest)
  end
  def self.basename(rest)
  end
  def self.dirname(req)
  end
  def self.extname(req)
  end
  def self.path(req)
  end
  def self.split(req)
  end
  def self.join(string1, string2)
  end
  def self.fnmatch(rest)
  end
  def self.fnmatch?(rest)
  end
  def lstat
  end
  def atime
  end
  def mtime
  end
  def ctime
  end
  def size
  end
  def chmod(req)
  end
  def chown(req,req1)
  end
  def truncate(req)
  end
  def flock(req)
  end
  def path
  end
  def to_path
  end
end
