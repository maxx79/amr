package ru.maximgorin.media.amr;

import ru.maximgorin.os.Validator;
import java.io.File;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.io.FileOutputStream;

class NativeLibrary {
	private static String LIB_NAME = "libamr";
	private static String LINUX_EXT = "so";
	private static String MAC_EXT = "dylib";
	private static String WINDOWS_EXT = "dll";
	private static boolean initialized;

	static boolean isInitialized() {
		return initialized;
	}

	static void setInitialized(boolean value) {
		initialized = value;
	}

	static void load(String path) throws IOException {
		if (path == null) {
			throw new IOException("Can't load a library");
		}
		StringBuilder sb = new StringBuilder();
		sb.append(path);
		if (initialized) {
			return;
		}
		if (Validator.isWindows()) {
			sb.append("/").append(LIB_NAME).append(Validator.getSunJavaArch())
					.append(".").append(WINDOWS_EXT);
		} else if (Validator.isLinux()) {
			final String armPostfix = isLinuxArm() ? "-arm" : "";
			sb.append("/").append(LIB_NAME).append(Validator.getSunJavaArch())
					.append(armPostfix).append(".").append(LINUX_EXT);
		} else if (Validator.isMac()) {
			sb.append("/").append(LIB_NAME).append(Validator.getMacJavaArch())
					.append(".").append(MAC_EXT);
		} else {
			throw new IOException("Version is absent");
		}
		byte buffer[] = new byte[4096];
		InputStream is = null;
		OutputStream os = null;
		try {
			is = NativeLibrary.class.getResourceAsStream(sb.toString());
			File libFile = File.createTempFile("amr", null);
			os = new FileOutputStream(libFile);
			int readBytes;
			while ((readBytes = is.read(buffer)) != -1) {
				os.write(buffer, 0, readBytes);
			}
			is.close();
			is = null;
			os.close();
			os = null;
			System.load(libFile.getAbsolutePath());
		} finally {
			if (is != null) {
				try {
					is.close();
				} catch (Exception e) {
				}
			}
			if (os != null) {
				try {
					os.close();
				} catch (Exception e) {
				}
			}
		}

		initialized = true;
	}

	static boolean isLinuxArm() {
		StringBuilder sb = new StringBuilder();
		int readByte;
		InputStream is = null;
		try {
			Process process = Runtime.getRuntime().exec("arch");
			is = process.getInputStream();
			while ((readByte = is.read()) != -1) {
				sb.append((char) readByte);
			}
			return sb.toString().toLowerCase().indexOf("arm") != -1;
		} catch (IOException e) {
			return false;
		} finally {
			if (is != null) {
				try {
					is.close();
				} catch (Exception e) {
				}
			}
		}
	}
}