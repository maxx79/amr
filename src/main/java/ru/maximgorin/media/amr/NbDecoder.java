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
final public class NbDecoder {
	/**
	 * Describes the samples rate per second of decoded data
	 */
	public final static int AMRNB_FORMAT_FREQUENCY = 8000;
	/**
	 * Describes the bits count per sample
	 */
	public final static int AMRNB_FORMAT_BITS_PER_SAMPLE = 16;
	/**
	 * Describes the channels count
	 */
	public final static int AMRNB_FORMAT_CHANNEL_COUNT = 1;
	static {
		try {
			if (!NativeLibrary.isInitialized()) {
				NativeLibrary.load("/libs");
			}
		} catch (IOException e) {
			e.printStackTrace();
		}
	}
	final static int DECODED_BUFFER_SIZE = 320;
	final static int SIZES[] = { 12, 13, 15, 17, 19, 20, 26, 31, 5, 6, 5, 5, 0,
			0, 0, 0 };
	final public static AudioFormat AMRNB_FORMAT = new AudioFormat(
			AudioFormat.Encoding.PCM_SIGNED, AMRNB_FORMAT_FREQUENCY,
			AMRNB_FORMAT_BITS_PER_SAMPLE, AMRNB_FORMAT_CHANNEL_COUNT,
			AMRNB_FORMAT_BITS_PER_SAMPLE / 8, AMRNB_FORMAT_FREQUENCY, false);

	private native static long initDecoder();

	private native static void releaseDecoder(long h);

	private native static byte[] decodeAll(byte data[]) throws FormatException,
			DecodingException;

	private native static byte[] decodeFrame(long h, byte inputBuffer[])
			throws DecodingException;

	/**
	 * Checks if the bytes array is AMR-NB
	 * 
	 * @param header
	 *            bytes array
	 * @return true, if the bytes array is AMR-NB format, otherwise false
	 */
	public static boolean isAmrNb(byte header[]) {
		if (header == null) {
			return false;
		}
		if (header.length < 6) {
			return false;
		}
		final byte chunk[] = Arrays.copyOf(header, 6);
		return "#!AMR\n".equals(new String(chunk));
	}

	/**
	 * Checks if the bytes stream is AMR-NB
	 * 
	 * @param is
	 *            bytes stream
	 * @return true, if the bytes stream is AMR-NB format, otherwise false
	 */
	public static boolean isAmrNb(InputStream is) throws IOException {
		if (is == null) {
			return false;
		}
		final byte chunk[] = new byte[6];
		return isAmrNb(chunk);
	}

	/**
	 * Decodes the data in AMR-NB format (with the header) into raw array of
	 * bytes (8000Hz, 16 bps, mono, little endian)
	 * 
	 * @param data
	 *            the bytes array of encoded data (AMR-NB)
	 * @return decoded bytes array
	 * @throws FormatException
	 * @throws DecodingException
	 */
	public static byte[] decode(byte data[]) throws FormatException,
			DecodingException {
		byte decodedData[] = decodeAll(data);
		if (decodedData == null) {
			throw new DecodingException();
		}
		return decodedData;
	}

	/**
	 * Decodes the data in AMR-NB data (with the its header) into raw array of
	 * bytes (8000Hz, 16 bps, mono, little endian)
	 * 
	 * @param is
	 *            the bytes stream of encoded data (AMR-NB)
	 * @return decoded bytes array
	 * @throws FormatException
	 * @throws DecodingException
	 * @throws IOException
	 */
	public static byte[] decode(InputStream is) throws FormatException,
			DecodingException, IOException {
		final ByteArrayOutputStream baos = new ByteArrayOutputStream();
		try {
			decode(is, baos);
			return baos.toByteArray();
		} finally {
			if (baos != null) {
				try {
					baos.close();
				} catch (Exception e) {
				}
			}
		}
	}

	/**
	 * Decodes the data in AMR-NB data (with the its header) into a raw stream
	 * of bytes (8000Hz, 16 bps, mono, little endian)
	 * 
	 * @param is
	 *            the bytes stream of encoded data (AMR-NB)
	 * @param os
	 *            the bytes stream of decoded data
	 * @throws FormatException
	 * @throws DecodingException
	 * @throws IOException
	 */
	public static void decode(InputStream is, OutputStream os)
			throws FormatException, DecodingException, IOException {
		byte decodedBuf[];
		if (is == null || os == null) {
			throw new NullPointerException();
		}
		final byte header[] = new byte[6];
		is.read(header, 0, header.length);
		if (!isAmrNb(header)) {
			throw new FormatException();
		}
		final long h = initDecoder();
		if (h == 0) {
			throw new DecodingException("Can not receive decoder handler");
		}

		try {
			while (true) {
				int value = is.read();
				if (value == -1) {
					break;// eof
				}
				int size = SIZES[(value >> 3) & 0x0f];
				if (size <= 0) {
					throw new DecodingException();
				}
				final byte encodedBuf[] = new byte[size + 1];
				encodedBuf[0] = (byte) value;
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
		} finally {
			releaseDecoder(h);
		}
	}

	/**
	 * Decodes the bytes array into the AudioInputStream object
	 * 
	 * @param decodedData
	 *            the bytes array of raw data (8000Hz, 16 bps, mono, little
	 *            endian)
	 * @return AudioInputStream object
	 * @throws FormatException
	 * @throws DecodingException
	 * @throws IOException
	 */
	public static AudioInputStream getAudioInputStream(byte decodedData[])
			throws FormatException, DecodingException, IOException {
		if (decodedData == null) {
			throw new NullPointerException();
		}
		return new AudioInputStream(new ByteArrayInputStream(decodedData),
				AMRNB_FORMAT, decodedData.length);
	}
}
