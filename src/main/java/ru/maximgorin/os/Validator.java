package ru.maximgorin.os;

/**
 * Special class to identify the current OS
 * 
 * @author maxim
 *
 */
public class Validator {
	private final static String OS = System.getProperty("os.name")
			.toLowerCase();

	/**
	 * Checks, if the OS is a Windows family
	 * 
	 * @return true - Windows, false - another one
	 */
	public static boolean isWindows() {
		return (OS.indexOf("win") >= 0);
	}

	/**
	 * Checks, if the OS is MAC OS 9+
	 * 
	 * @return true - MAC OS, false - another one
	 */
	public static boolean isMac() {
		return (OS.indexOf("mac") >= 0);
	}

	/**
	 * Checks, if the OS is a Linux family
	 * 
	 * @return true - Linux, false - another one
	 */
	public static boolean isLinux() {
		return (OS.indexOf("linux") >= 0);
	}

	/**
	 * Checks, if the OS is a Unix family
	 * 
	 * @return true - Unix, false - another one
	 */
	public static boolean isUnix() {
		return (OS.indexOf("nix") >= 0 || OS.indexOf("nux") >= 0 || OS
				.indexOf("aix") > 0);
	}

	/**
	 * Checks, if the OS is a Solaris
	 * 
	 * @return true - Solaris, false - another one
	 */
	public static boolean isSolaris() {
		return (OS.indexOf("sunos") >= 0);
	}

	/**
	 * Retrivies number of bits of the architecture
	 * 
	 * @return 32 or 64 bits for Sun Java implementation
	 */
	public static int getSunJavaArch() {
		try {
			return Integer.parseInt(System.getProperty("sun.arch.data.model"));
		} catch (NumberFormatException e) {
		}
		return 32;
	}

	/**
	 * Retrivies number of bits of the architecture
	 * 
	 * @return 32 or 64 bits for Mac OS Java implementation
	 */
	public static int getMacJavaArch() {
		try {
			return Integer.parseInt(System.getProperty("sun.arch.data.model"));
		} catch (NumberFormatException e) {
		}
		return 32;
	}
}