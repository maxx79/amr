/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package ru.maximgorin.media.amr;

import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import ru.maximgorin.media.DecodingException;
import ru.maximgorin.media.FormatException;
import java.io.IOException;
import java.util.Arrays;
import javax.sound.sampled.AudioFormat;
import javax.sound.sampled.AudioFormat.Encoding;
import javax.sound.sampled.AudioInputStream;
/**
 *
 * @author maxim
 */
final public class WbDecoder {
    /**
     * Describes the samples rate per second of decoded data
     */
    public final static int AMRWB_FORMAT_FREQUENCY  = 16000;
    /**
     * Describes the bits count per sample
     */
    public final static int AMRWB_FORMAT_BITS_PER_SAMPLE = 16;
    /**
     * Describes the channels count
     */
    public final static int AMRWB_FORMAT_CHANNEL_COUNT = 1;
    static {
	try {
	    if (!NativeLibrary.isInitialized()) {
		NativeLibrary.load("/libs");
	    }
	}catch(IOException e) {
	    e.printStackTrace();
	}
    }
    final static int DECODED_BUFFER_SIZE = 320;
    final static int SIZES[] = { 17, 23, 32, 36, 40, 46, 50, 58, 60, 5, -1, -1, -1, -1, -1, 0 };
    public final static AudioFormat AMRWB_FORMAT = new AudioFormat(AudioFormat.Encoding.PCM_SIGNED, AMRWB_FORMAT_FREQUENCY, 
						AMRWB_FORMAT_BITS_PER_SAMPLE, AMRWB_FORMAT_CHANNEL_COUNT,
						AMRWB_FORMAT_BITS_PER_SAMPLE / 8, AMRWB_FORMAT_FREQUENCY, false);
    private native static long initDecoder();
    private native static void releaseDecoder(long h);
    private native static byte [] decodeAll(byte data[])  throws FormatException, DecodingException;
    private native static byte [] decodeFrame(long h, byte inputBuffer[]) throws DecodingException;
    /**
     * Checks if the bytes array is AMR-WB
     * @param header  bytes array
     * @return true, if the bytes array is AMR-WB format, otherwise false
     */
    public static boolean isAmrWb(byte header[]) {
	if (header == null) {
	    return false;
	}
	if (header.length < 6) {
	    return false;
	}
	final byte chunk[] = Arrays.copyOf(header, 6);
	return "#!AMR-WB\n".equals(new String(chunk));
    }
    /**
     * Checks if the bytes stream is AMR-WB
     * @param is  bytes stream
     * @return true, if the bytes stream is AMR-WB format, otherwise false
     */
    public static boolean isAmrWb(InputStream is) throws IOException{
	if (is == null) {
	    return false;
	}	
	final byte chunk[] = new byte[9];
	int readBytes = is.read(chunk);
	if (readBytes != 9) {
	    return false;
	}
	return isAmrWb(chunk);
    }
    /**
     * Decodes the data in AMR-WB format (with the header) into raw array of bytes (8000Hz, 16 bps, mono, little endian)  
     * @param data  the bytes array of encoded data (AMR-WB) 
     * @return decoded bytes array
     * @throws FormatException
     * @throws DecodingException 
     */
    public static byte [] decode(byte data[]) throws FormatException, DecodingException{
	byte decodedData[] = decodeAll(data);
	if (decodedData == null) {
	    throw new DecodingException();
	}
	return decodedData;
    }
    /**
     * Decodes the data in AMR-WB data (with the its header) into raw array of bytes (8000Hz, 16 bps, mono, little endian)  
     * @param is  the bytes stream of encoded data (AMR-WB) 
     * @return decoded bytes array
     * @throws FormatException
     * @throws DecodingException 
     */
    public static byte [] decode(InputStream is) throws FormatException, DecodingException, IOException{
        final ByteArrayOutputStream baos = new ByteArrayOutputStream();
	try {
	    decode(is, baos);
	    return baos.toByteArray();
	}
	finally {
	    if (baos != null) {
		try {
		    baos.close();
		}catch(Exception e) {
		}
	    }
	}        
    }
    /**
     * Decodes the data in AMR-WB data (with the its header) into a raw stream of bytes (8000Hz, 16 bps, mono, little endian)  
     * @param is  the bytes stream of encoded data (AMR-WB)      
     * @param os  the bytes stream of decoded data      
     * @throws FormatException
     * @throws DecodingException 
     * @throws IOException
     */
    public static void decode(InputStream is, OutputStream os) throws FormatException, DecodingException, IOException{
        if (is == null || os == null) {
            throw new NullPointerException();
        }        
        final byte header[] = new byte[9];
        is.read(header, 0, header.length);
        if (!isAmrWb(header)) {
            throw new FormatException();
        }        
        final long h = initDecoder();
	if (h == 0) {
	    throw new DecodingException("Can not receive decoder handler");
	}
        byte decodedBuf[];
        try {
             while(true) {
                int value = is.read();
                if (value == -1) {
                    break;// eof
                }
                int size = SIZES[(value >> 3) & 0x0f];
                if (size <= 0) {
                    throw new DecodingException();
                }
                final byte encodedBuf[] = new byte[size + 1];
		encodedBuf[0] = (byte)value;
                int readBytes = is.read(encodedBuf, 1, size);
                if (readBytes != size) {
                    throw new DecodingException();
                }
		decodedBuf = decodeFrame(h, encodedBuf);
                if (decodedBuf == null) {
		    throw new DecodingException();
		}
                os.write(decodedBuf);
             }
             os.flush();
        }
        finally {
            releaseDecoder(h);
        }
    }
    /**
     * Decodes the bytes array into the AudioInputStream object
     * @param decodedData  the bytes array of raw data (8000Hz, 16 bps, mono, little endian) 
     * @return AudioInputStream object
     * @throws FormatException
     * @throws DecodingException
     * @throws IOException 
     */
    public static AudioInputStream getAudioInputStream(byte decodedData[]) throws FormatException, DecodingException, IOException{
	if (decodedData == null) {
	    throw new NullPointerException();
	}
	return new AudioInputStream(new ByteArrayInputStream(decodedData), AMRWB_FORMAT, decodedData.length);
    }
}
