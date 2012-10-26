 
#
# It is auto-generated content.
# Do not do required for this file in your application.
#
 
class BasicSocket
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

  def self.do_not_reverse_lookup
  end  
  def self.do_not_reverse_lookup=(req)
  end  
  def self.for_fd(req)
  end  
  def close_read
  end  
  def close_write
  end  
  def shutdown(rest)
  end  
  def setsockopt(req,req1,req2)
  end  
  def getsockopt(req,req1)
  end  
  def getsockname
  end  
  def getpeername
  end  
  def send(rest)
  end  
  def recv(rest)
  end  
  def recv_nonblock(rest)
  end  
  def do_not_reverse_lookup
  end  
  def do_not_reverse_lookup=(req)
  end  
end

