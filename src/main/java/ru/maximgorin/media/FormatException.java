package ru.maximgorin.media;

/**
 *
 * @author maxim
 */
public class FormatException extends Exception {

	public FormatException() {
	}

	public FormatException(String message) {
		super(message);
	}

	public FormatException(String message, Throwable cause) {
		super(message, cause);
	}

	public FormatException(Throwable cause) {
		super(cause);
	}

}
