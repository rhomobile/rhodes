package j2me.nio.channels;

import java.io.IOException;
import j2me.nio.ByteBuffer;

public abstract class Pipe {
        public static abstract class SinkChannel
        {
                public abstract void close() throws IOException;
                public abstract int write(ByteBuffer src) throws IOException;   
        }
        
        public static abstract class SourceChannel
        {
                public abstract void close() throws IOException;
                public abstract int read(ByteBuffer dst) throws IOException;
        }       
}
