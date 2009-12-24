# blowfish.rb  Richard Kernahan <kernighan_rich@rubyforge.org>
#
#  Blowfish algorithm by Bruce Schneider
#  Ported by Richard Kernahan from the reference C code

module Crypt
class Blowfish
  
  require 'crypt/cbc'
  include Crypt::CBC
  
  require 'crypt/blowfish-tables'
  include Crypt::BlowfishTables
  
  ULONG = 0x100000000
  
  def block_size
    return(8)
  end
  
  
  def initialize(key)
    @key = key
    raise "Bad key length: the key must be 1-56 bytes." unless (key.length.between?(1,56))
    @pArray = []
    @sBoxes = []
    setup_blowfish()
  end
  
  
  def f(x)
    a, b, c, d = [x].pack('N').unpack('CCCC')
    y = (@sBoxes[0][a] + @sBoxes[1][b]) % ULONG
    y = (y ^ @sBoxes[2][c]) % ULONG
    y = (y + @sBoxes[3][d]) % ULONG
    return(y)
  end
  
  
  def setup_blowfish()
    @sBoxes = Array.new(4) { |i| INITIALSBOXES[i].clone }
    @pArray = INITIALPARRAY.clone
    keypos = 0
    0.upto(17) { |i|
      data = 0
      4.times {
        #data = ((data << 8) | @key[keypos]) % ULONG
		data = ((data << 8) | @key[keypos].ord) % ULONG
        keypos = (keypos.next) % @key.length
      }
      @pArray[i] = (@pArray[i] ^ data) % ULONG
    }
    l = 0
    r = 0
    0.step(17, 2) { |i|
      l, r = encrypt_pair(l, r)
      @pArray[i]   = l
      @pArray[i+1] = r
    }
    0.upto(3) { |i|
        l, r = encrypt1(i,l,r)
    }
  end

  def encrypt1(i,l,r)
      0.step(255, 2) { |j|
        l, r = encrypt_pair(l, r)
        @sBoxes[i][j]   = l
        @sBoxes[i][j+1] = r
      }
      return([l, r])
  end
  
  def encrypt_pair(xl, xr)
    0.upto(15) { |i|
        xl = (xl ^ @pArray[i]) % ULONG
        xr = (xr ^ f(xl)) % ULONG
        xl, xr = [xl, xr].reverse
    }
    xl, xr = [xl, xr].reverse
    xr = (xr ^ @pArray[16]) % ULONG
    xl = (xl ^ @pArray[17]) % ULONG
    return([xl, xr])
  end
  
  
  def decrypt_pair(xl, xr)
    17.downto(2) { |i|
        xl = (xl ^ @pArray[i]) % ULONG
        xr = (xr ^ f(xl)) % ULONG
        xl, xr = [xl, xr].reverse
    }
    xl, xr = [xl, xr].reverse
    xr = (xr ^ @pArray[1]) % ULONG
    xl = (xl ^ @pArray[0]) % ULONG
    return([xl, xr])
  end
  
  
  def encrypt_block(block)
    xl, xr = block.unpack('NN')
    xl, xr = encrypt_pair(xl, xr)
    encrypted = [xl, xr].pack('NN')
    return(encrypted)
  end
  
  
  def decrypt_block(block)
    xl, xr = block.unpack('NN')
    xl, xr = decrypt_pair(xl, xr)
    decrypted = [xl, xr].pack('NN')
    return(decrypted)
  end
  
end
end