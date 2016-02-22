package ru.maximgorin.media;

/**
 *
 * @author maxim
 */
public class EncodingException extends Exception {

	public EncodingException() {
	}

	public EncodingException(String message) {
		super(message);
	}

	public EncodingException(String message, Throwable cause) {
		super(message, cause);
	}

	public EncodingException(Throwable cause) {
		super(cause);
	}

}
