package j2me.util;

import java.util.Random;

public class RandomMe {
    private static final int BITS_PER_BYTE = 8;
    private static final int BYTES_PER_INT = 4;

    public static void nextBytes(Random rndThis, byte[] bytes) {
        int numRequested = bytes.length;

        int numGot = 0, rnd = 0;

        while (true) {
            for (int i = 0; i < BYTES_PER_INT; i++) {
                if (numGot == numRequested)
                    return;

                //TODO: rndThis.next
                rnd = (i==0 ? 0/*rndThis.next(BITS_PER_BYTE * BYTES_PER_INT)*/
                            : rnd >> BITS_PER_BYTE);
                bytes[numGot++] = (byte)rnd;
            }
        }
    }
    
}
