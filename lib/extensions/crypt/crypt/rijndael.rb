# rijndael.rb  Richard Kernahan <kernighan_rich@rubyforge.org>

# Adapted from the reference C implementation:
#   rijndael-alg-ref.c   v2.2   March 2002
#   Reference ANSI C code
#   authors: Paulo Barreto and Vincent Rijmen
#   This code is placed in the public domain.

module Crypt
class Rijndael

  require 'crypt/cbc'
  include Crypt::CBC
  
  require 'crypt/rijndael-tables'
  include Crypt::RijndaelTables
  
  
  def initialize(userKey, keyBits = 256, blockBits = 128)
    case keyBits
      when 128 
        @keyWords = 4
      when 192 
        @keyWords = 6
      when 256
        @keyWords = 8
      else raise "The key must be 128, 192, or 256 bits long."
    end
    
    case (keyBits >= blockBits) ? keyBits : blockBits
      when 128 
        @rounds = 10
      when 192 
        @rounds = 12
      when 256
        @rounds = 14
      else raise "The key and block sizes must be 128, 192, or 256 bits long."
    end
   
    case blockBits
      when 128 
        @blockSize = 16
        @blockWords = 4
        @shiftIndex = 0
      when 192 
        @blockSize = 24
        @blockWords = 6
        @shiftIndex = 1
      when 256 
        @blockSize = 32
        @blockWords = 8
        @shiftIndex = 2
      else raise "The block size must be 128, 192, or 256 bits long."
    end
    
    uk = userKey.unpack('C'*userKey.length)
    maxUsefulSizeOfUserKey = (keyBits/8)
    uk = uk[0..maxUsefulSizeOfUserKey-1]    # truncate
    padding = 0
    if (userKey.length < keyBits/8)
      shortfallInUserKey = (keyBits/8 - userKey.length)
      shortfallInUserKey.times { uk << padding }
    end
    @key = [[], [], [], []]
    0.upto(uk.length-1) { |pos|
      @key[pos % 4][pos / 4] = uk[pos]
    }
    @roundKeys = generate_key_schedule(@key, keyBits, blockBits)
  end

  
  def block_size
    return(@blockSize) # needed for CBC
  end
  
  
  def mul(a, b)
    if ((a ==0) | (b == 0))
      result = 0 
    else
      result = AlogTable[(LogTable[a] + LogTable[b]) % 255]
    end
    return(result)
  end
  
  
  def add_round_key(blockArray, roundKey)
  0.upto(3) { |i|
    0.upto(@blockWords) { |j|

      if blockArray[i][j].is_a?(String) then
         blockArray[i][j] = blockArray[i][j].unpack('C*').first
      end
      blockArray[i][j] ^= roundKey[i][j]
    }
  }
  return(blockArray)
  end
  
  
  def shift_rows(blockArray, direction)
    tmp = []
    1.upto(3) { |i|  # row zero remains unchanged
      0.upto(@blockWords-1) { |j|
        tmp[j] = blockArray[i][(j + Shifts[@shiftIndex][i][direction]) % @blockWords]
      }
      0.upto(@blockWords-1) { |j|
        blockArray[i][j] = tmp[j]
      }
    }
    return(blockArray)
  end
  
  
  def substitution(blockArray, sBox)
    # replace every byte of the input with the byte at that position in the S-box
    0.upto(3) { |i|
      0.upto(@blockWords-1) { |j|
        blockArray[i][j] = sBox[blockArray[i][j]]
      }
    }
    return(blockArray)
  end
  
  
  def mix_columns(blockArray)
    mixed = [[], [], [], []]
    0.upto(@blockWords-1) { |j|
      0.upto(3) { |i|
        mixed[i][j] = mul(2,blockArray[i][j]) ^
          mul(3,blockArray[(i + 1) % 4][j]) ^
          blockArray[(i + 2) % 4][j] ^
          blockArray[(i + 3) % 4][j]
      }
    }
    return(mixed)
  end
  
  
  def inverse_mix_columns(blockArray)
    unmixed = [[], [], [], []]
    0.upto(@blockWords-1) { |j|
      0.upto(3) { |i|
        unmixed[i][j] = mul(0xe, blockArray[i][j]) ^
          mul(0xb, blockArray[(i + 1) % 4][j]) ^                
          mul(0xd, blockArray[(i + 2) % 4][j]) ^
          mul(0x9, blockArray[(i + 3) % 4][j])
      }
    }
     return(unmixed)
  end
  
  
  def generate_key_schedule(k, keyBits, blockBits)
    tk = k[0..3][0..@keyWords-1]  # using slice to get a copy instead of a reference
    keySched = []
    (@rounds + 1).times { keySched << [[], [], [], []] }
    t = 0
    j = 0
    while ((j < @keyWords) && (t < (@rounds+1)*@blockWords))
      0.upto(3) { |i|
        keySched[t / @blockWords][i][t % @blockWords] = tk[i][j]
      }
      j += 1
      t += 1
    end
    # while not enough round key material collected, calculate new values
    rconIndex = 0
    while (t < (@rounds+1)*@blockWords) 
      0.upto(3) { |i|
        tk[i][0] ^= S[tk[(i + 1) % 4][@keyWords - 1]]
      }
      tk[0][0] ^= Rcon[rconIndex]
      rconIndex = rconIndex.next
      if (@keyWords != 8)
        1.upto(@keyWords - 1) { |j|
          0.upto(3) { |i|
            tk[i][j] ^= tk[i][j-1];
          }
        }
      else
        1.upto(@keyWords/2 - 1) { |j|
          0.upto(3) { |i|
            tk[i][j] ^= tk[i][j-1]
          }
        }
        0.upto(3) { |i|
          tk[i][@keyWords/2] ^= S[tk[i][@keyWords/2 - 1]]
        }
        (@keyWords/2 + 1).upto(@keyWords - 1) { |j|
          0.upto(3) { |i| 
            tk[i][j] ^= tk[i][j-1] 
          }
        }
      end
      j = 0
      while ((j < @keyWords) && (t < (@rounds+1) * @blockWords))
        0.upto(3) { |i|
          keySched[t / @blockWords][i][t % @blockWords] = tk[i][j]
        }
        j += 1
        t += 1
      end
    end
    return(keySched)
  end
  
  
  def encrypt_byte_array(blockArray)
    blockArray = add_round_key(blockArray, @roundKeys[0])
    1.upto(@rounds - 1) { |round|
      blockArray = substitution(blockArray, S)
      blockArray = shift_rows(blockArray, 0)
      blockArray = mix_columns(blockArray)
      blockArray = add_round_key(blockArray, @roundKeys[round])
    }
    # special round without mix_columns
    blockArray = substitution(blockArray,S)
    blockArray = shift_rows(blockArray,0)
    blockArray = add_round_key(blockArray, @roundKeys[@rounds])
    return(blockArray)
  end
  
  
  def encrypt_block(block)
    raise "block must be #{@blockSize} bytes long" if (block.length() != @blockSize)
    blockArray = [[], [], [], []]
    0.upto(@blockSize - 1) { |pos|
      blockArray[pos % 4][pos / 4] = block[pos]
    }
    encryptedBlock = encrypt_byte_array(blockArray)
    encrypted = ""
    0.upto(@blockSize - 1) { |pos|
      encrypted << encryptedBlock[pos % 4][pos / 4]
    }
    return(encrypted)
  end
  
  
  def decrypt_byte_array(blockArray)
    # first special round without inverse_mix_columns
    # add_round_key is an involution - applying it a second time returns the original result
    blockArray = add_round_key(blockArray, @roundKeys[@rounds]) 
    blockArray = substitution(blockArray,Si)   # using inverse S-box
    blockArray = shift_rows(blockArray,1)
    (@rounds-1).downto(1) { |round|
      blockArray = add_round_key(blockArray, @roundKeys[round])
      blockArray = inverse_mix_columns(blockArray)
      blockArray = substitution(blockArray, Si) 
      blockArray = shift_rows(blockArray, 1)
    }
    blockArray = add_round_key(blockArray, @roundKeys[0])
    return(blockArray)
  end
  
  
  def decrypt_block(block)
    raise "block must be #{@blockSize} bytes long" if (block.length() != @blockSize)
    blockArray = [[], [], [], []]
    0.upto(@blockSize - 1) { |pos|
      blockArray[pos % 4][pos / 4] = block[pos]
    }
    decryptedBlock = decrypt_byte_array(blockArray)
    decrypted = ""
    0.upto(@blockSize - 1) { |pos|
      decrypted << decryptedBlock[pos % 4][pos / 4]
    }
    return(decrypted)
  end
  
  
end
end