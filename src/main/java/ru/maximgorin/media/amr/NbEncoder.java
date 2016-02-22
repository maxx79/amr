package ru.maximgorin.media.amr;

import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;

import javax.sound.sampled.AudioFormat;
import javax.sound.sampled.AudioInputStream;

import ru.maximgorin.media.EncodingException;
import ru.maximgorin.media.FormatException;

/*
 { MR475,  4750 },
 { MR515,  5150 },
 { MR59,   5900 },
 { MR67,   6700 },
 { MR74,   7400 },
 { MR795,  7950 },
 { MR102, 10200 },
 { MR122, 12200 }
 */
final public class NbEncoder {
	static {
		try {
			if (!NativeLibrary.isInitialized()) {
				NativeLibrary.load("/libs");
			}
		} catch (IOException e) {
			e.printStackTrace();
		}
	}

	/**
	 * Contains the modes of encoding AMR-NB
	 */
	public enum Mode {
		MR475, MR515, MR59, MR67, MR74, MR795, MR102, MR122
	};

	private final static int RATES[] = { 4750, 5150, 5900, 6700, 7400, 7950,
			10200, 12200 };
	final static int DECODED_BUFFER_SIZE = 320;

	private static int isRateValid(int rate) {
		for (int i = 0; i < RATES.length; i++) {
			if (rate == RATES[i]) {
				return i;
			}
		}
		return -1;
	}

	private static int mode2Rate(Mode mode) {
		switch (mode) {
		case MR475:
			return RATES[0];
		case MR515:
			return RATES[1];
		case MR59:
			return RATES[2];
		case MR67:
			return RATES[3];
		case MR74:
			return RATES[4];
		case MR795:
			return RATES[5];
		case MR102:
			return RATES[6];
		case MR122:
			return RATES[7];
		}
		return -1;
	}

	private static int getModeIndex(Mode mode) {
		switch (mode) {
		case MR475:
			return 0;
		case MR515:
			return 1;
		case MR59:
			return 2;
		case MR67:
			return 3;
		case MR74:
			return 4;
		case MR795:
			return 5;
		case MR102:
			return 6;
		case MR122:
			return 7;
		}
		return -1;
	}

	private static Mode rate2Mode(int rate) {
		if (RATES[0] == rate) {
			return Mode.MR475;
		}
		if (RATES[1] == rate) {
			return Mode.MR515;
		}
		if (RATES[2] == rate) {
			return Mode.MR59;
		}
		if (RATES[3] == rate) {
			return Mode.MR67;
		}
		if (RATES[4] == rate) {
			return Mode.MR74;
		}
		if (RATES[5] == rate) {
			return Mode.MR795;
		}
		if (RATES[6] == rate) {
			return Mode.MR102;
		}
		if (RATES[7] == rate) {
			return Mode.MR122;
		}
		return null;
	}

	private native static long initEncoder(int dtx);

	private native static void releaseEncoder(long h);

	private native static byte[] encodeAll(int mode, int dtx, byte data[])
			throws FormatException, EncodingException;

	private native static byte[] encodeFrame(int mode, long h,
			byte inputBuffer[]) throws EncodingException;

	private static void checkFormat(AudioFormat format) throws FormatException {
		if (format == null) {
			throw new NullPointerException();
		}
		if (format.getChannels() != 0) {
			throw new FormatException("Only mono is supported");
		}
		if (format.getSampleRate() != 8000) {
			throw new FormatException("8000Hz sample rate is required");
		}
		if (format.getSampleSizeInBits() != 16) {
			throw new FormatException("16 bits per sample is required");
		}
		if (format.isBigEndian()) {
			throw new FormatException("Little endian sequence is required");
		}
		if (!format.getEncoding().equals(AudioFormat.Encoding.PCM_SIGNED)) {
			throw new FormatException("Signed PCM sample format is required");
		}
	}

	/**
	 * Encodes the raw bytes array into AMR-NB format
	 * 
	 * @param mode
	 *            a mode of encoding
	 * @param dtx
	 *            if true then use dtx, otherwise false
	 * @param data
	 *            the bytes array of raw data
	 * @return encoded bytes data in AMR-NB format (with the header)
	 * @throws FormatException
	 * @throws EncodingException
	 */
	public static byte[] encode(Mode mode, boolean dtx, byte data[])
			throws FormatException, EncodingException {
		if (data == null) {
			throw new NullPointerException();
		}
		if ((data.length % 2) != 0) {
			throw new FormatException("Invalid data size");
		}
		byte encodedData[] = encodeAll(getModeIndex(mode), dtx ? 1 : 0, data);
		if (encodedData == null) {
			throw new EncodingException();
		}
		return encodedData;
	}

	/**
	 * Encodes the raw bytes stream into AMR-NB format
	 * 
	 * @param mode
	 *            a mode of encoding
	 * @param dtx
	 *            if true then use dtx, otherwise false
	 * @param is
	 *            the bytes stream of raw data
	 * @return encoded bytes data in AMR-NB format (with the header)
	 * @throws FormatException
	 * @throws EncodingException
	 * @throws IOException
	 */
	public static byte[] encode(Mode mode, boolean dtx, InputStream is)
			throws FormatException, EncodingException, IOException {
		ByteArrayOutputStream baos = new ByteArrayOutputStream();
		try {
			encode(mode, dtx, is, baos);
			return baos.toByteArray();
		} finally {
			try {
				baos.close();
			} catch (Exception e) {
			}
		}
	}

	/**
	 * Encodes the raw bytes stream into AMR-NB format
	 * 
	 * @param mode
	 *            a mode of encoding
	 * @param dtx
	 *            if true then use dtx, otherwise false
	 * @param is
	 *            the bytes stream of raw data
	 * @param os
	 *            the bytes array of encoded data in AMR-NB format
	 * @throws FormatException
	 * @throws EncodingException
	 * @throws IOException
	 */
	public static void encode(Mode mode, boolean dtx, InputStream is,
			OutputStream os) throws FormatException, EncodingException,
			IOException {
		if (is == null || os == null) {
			throw new NullPointerException();
		}
		int imode = getModeIndex(mode);
		final long h = initEncoder(dtx ? 1 : 0);
		if (h == 0) {
			throw new EncodingException("Can not receive encoder handler");
		}
		final byte inputBuffer[] = new byte[DECODED_BUFFER_SIZE];
		int readBytes;
		byte outputBuffer[];
		try {
			os.write("#!AMR\n".getBytes());
			while (true) {
				readBytes = is.read(inputBuffer);
				if (readBytes == -1) {
					break;
				}
				if (readBytes < DECODED_BUFFER_SIZE) {
					break;
				}
				outputBuffer = encodeFrame(imode, h, inputBuffer);
				if (outputBuffer == null) {
					throw new EncodingException();
				}
				os.write(outputBuffer);
			}
			os.flush();
		} finally {
			releaseEncoder(h);
		}
	}

	/**
	 * Encodes the AudioInputStream into AMR-NB format
	 * 
	 * @param mode 
	 *            a mode of encoding
	 * @param dtx
	 *            if true then use dtx, otherwise false
	 * @param ais
	 *            AudioInputStream in the next format: SIGNED_PCM, 8000Hz, 16
	 *            bps, mono, little endian
	 * @param os
	 *            the bytes stream of encoded data in AMR-NB format
	 * @throws FormatException
	 * @throws EncodingException
	 * @throws IOException
	 */
	public static void encode(Mode mode, boolean dtx, AudioInputStream ais,
			OutputStream os) throws FormatException, EncodingException,
			IOException {
		if (ais == null || os == null) {
			throw new NullPointerException();
		}
		checkFormat(ais.getFormat());
		encode(mode, dtx, ais, os);
	}

	/**
	 * Encodes the AudioInputStream into AMR-NB format
	 * 
	 * @param mode
	 *            a mode of encoding
	 * @param dtx
	 *            if true then use dtx, otherwise false
	 * @param ais
	 *            AudioInputStream in the next format: SIGNED_PCM, 8000Hz, 16
	 *            bps, mono, little endian
	 * @return the bytes array of encoded data in AMR-NB format
	 * @throws FormatException
	 * @throws EncodingException
	 * @throws IOException
	 */
	public static byte[] encode(Mode mode, boolean dtx, AudioInputStream ais)
			throws FormatException, EncodingException, IOException {
		if (ais == null) {
			throw new NullPointerException();
		}
		checkFormat(ais.getFormat());
		return encode(mode, dtx, ais);
	}

}