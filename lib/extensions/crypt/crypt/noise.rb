# crypt/rattle.rb  Richard Kernahan <kernighan_rich@rubyforge.org>

# add_noise - take a message and intersperse noise to make a new noisy message of given byte-length
# remove_noise - take a noisy message and extract the message

module Crypt
module Noise

  def add_noise(newLength)
    message = self
    usableNoisyMessageLength = newLength / 9 * 8
    bitmapSize = newLength / 9
    remainingBytes = newLength - usableNoisyMessageLength - bitmapSize
    if (message.length > usableNoisyMessageLength)
      minimumNewLength = (message.length / 8.0).ceil * 9
      puts "For a clear text of #{message.length} bytes, the minimum obscured length"
      puts "is #{minimumNewLength} bytes which allows for no noise in the message."
      puts "You should choose an obscured length of at least double the clear text"
      puts "length, such as #{message.length / 8 * 32} bytes"
      raise "Insufficient length for noisy message" 
    end
    bitmap = []
    usableNoisyMessageLength.times { bitmap << false }
    srand(Time.now.to_i)
    positionsSelected = 0
    while (positionsSelected < message.length)
      positionTaken = rand(usableNoisyMessageLength)
      if bitmap[positionTaken]
        next
      else
        bitmap[positionTaken] = true
        positionsSelected = positionsSelected.next
      end
    end
    
    noisyMessage = ""
    0.upto(bitmapSize-1) { |byte|
      c = 0
      0.upto(7) { |bit|
        c = c + (1<<bit) if bitmap[byte * 8 + bit]
      }
      noisyMessage << c.chr
    }
    posInMessage = 0
    0.upto(usableNoisyMessageLength-1) { |pos|
      if bitmap[pos]
        meaningfulByte = message[posInMessage]
        noisyMessage << meaningfulByte
        posInMessage = posInMessage.next
      else
        noiseByte = rand(256).chr
        noisyMessage << noiseByte
      end
    }
    remainingBytes.times {
        noiseByte = rand(256).chr
        noisyMessage << noiseByte
    }
    return(noisyMessage)
  end
  
  
  def remove_noise
    noisyMessage = self
    bitmapSize = noisyMessage.length / 9
    actualMessageLength =  bitmapSize * 8
    
    actualMessageStart = bitmapSize
    actualMessageFinish = bitmapSize + actualMessageLength - 1
    actualMessage = noisyMessage[actualMessageStart..actualMessageFinish]
    
    bitmap = []
    0.upto(bitmapSize - 1) { |byte|
      c = noisyMessage[byte]
      0.upto(7) { |bit|
        bitmap[byte * 8 + bit] = (c[bit] == 1)
      }
    }
    clearMessage = ""
    0.upto(actualMessageLength) { |pos|
      meaningful = bitmap[pos]
      if meaningful
        clearMessage << actualMessage[pos]
      end
    }
    return(clearMessage)
  end
  
end
end

class String
  include Crypt::Noise
end