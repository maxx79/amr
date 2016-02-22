package ru.maximgorin;

import java.io.ByteArrayOutputStream;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;

import ru.maximgorin.media.DecodingException;
import ru.maximgorin.media.FormatException;
import ru.maximgorin.media.amr.NbDecoder;

public class Example1 {
	public static void main(String[] args) throws IOException {
		if (args.length < 2) {
			System.out.println("Usage: Test.jar <in.amr> <out.raw>");
			return;
		}
		System.out.println("args[0]=" + args[0]);
		System.out.println("args[1]=" + args[1]);
		FileInputStream fis = new FileInputStream(args[0]);
		FileOutputStream fos = new FileOutputStream(args[1]);
		try {
			NbDecoder.decode(fis, fos);
		} catch (FormatException e) {
			e.printStackTrace();
			System.out.println(e.getMessage());
		} catch (DecodingException e) {
			e.printStackTrace();
			System.out.println(e.getMessage());
		}
		fis.close();
		fos.close();
		// ----------------------------------------
		fis = new FileInputStream(args[0]);
		fos = new FileOutputStream(args[1] + ".2");
		try {
			byte data[] = NbDecoder.decode(fis);
			fos.write(data);
		} catch (FormatException e) {
			e.printStackTrace();
			System.out.println(e.getMessage());
		} catch (DecodingException e) {
			e.printStackTrace();
			System.out.println(e.getMessage());
		}
		fis.close();
		fos.close();

		fis = new FileInputStream(args[0]);
		fos = new FileOutputStream(args[1] + ".3");
		ByteArrayOutputStream baos = new ByteArrayOutputStream();
		int readBytes;
		byte buffer[] = new byte[4096];
		while ((readBytes = fis.read(buffer)) != -1) {
			baos.write(buffer, 0, readBytes);
		}
		try {
			buffer = NbDecoder.decode(baos.toByteArray());
			fos.write(buffer);
		} catch (FormatException e) {
			e.printStackTrace();
			System.out.println(e.getMessage());
		} catch (DecodingException e) {
			e.printStackTrace();
			System.out.println(e.getMessage());
		}
		fis.close();
		fos.close();

	}
}
