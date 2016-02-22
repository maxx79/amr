package ru.maximgorin.media;

/**
 *
 * @author maxim
 */
public class DecodingException extends Exception {

	public DecodingException() {
	}

	public DecodingException(String message) {
		super(message);
	}

	public DecodingException(String message, Throwable cause) {
		super(message, cause);
	}

	public DecodingException(Throwable cause) {
		super(cause);
	}

}
