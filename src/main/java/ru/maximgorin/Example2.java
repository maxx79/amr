package ru.maximgorin;

import java.io.FileInputStream;
import java.io.IOException;

import javax.sound.sampled.AudioSystem;
import javax.sound.sampled.Clip;

public class Example2 {
	public static void main(String[] args) throws IOException {
		if (args.length < 1) {
			System.out.println("Usage: Test.jar <in.amr>");
			return;
		}
		System.out.println("args[0]=" + args[0]);
		FileInputStream fis = new FileInputStream(args[0]);
		try {
			Clip clip = AudioSystem.getClip();
			clip.open(AudioSystem.getAudioInputStream(fis));
			clip.start();
		}

		catch (Exception e) {
			e.printStackTrace();
		}
		fis.close();
	}
}
