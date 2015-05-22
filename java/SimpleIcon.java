/**
 * Programmieraufgabe: SimpleIcon
 * 
 * Ziel ist es, eine Klasse zu schreiben welche in der Lage ist ein als ASCII-Textdatei mit 
 * speziellem Format abgespeichertes Bild zu laden und anzuzeigen. Jedes dieser Bilder hat 
 * folgende Zusatzinformationen:
 * - einen Namen
 * - die Version der verwendeten Spezifikation (siehe unten)
 * - eine Größe (in Form Höhe x Breite, also z.B. "8x8")
 * 
 * Die eigentlichen Bildinformationen liegen als Ketten der Zeichen 1 und 0 vor. Dort wo eine 
 * 1 steht, ist ein Bildpunkt zu malen.
 * 
 * Alle diese Informationen sollen durch das Trennzeichen ";;" (doppeltes Semikolon) voneinander 
 * getrennt sein und in folgender Reihenfolge in der Datei stehen:
 * 
 * name;;version;;größe;;daten
 * 
 * Es gibt 2 verschiedene Spezifikationen die umgesetzt werden können:
 * 
 * Version 1 (einfacher): Die Bildpunkte wurden Zeilenweise nacheinander geschrieben. Ist also 
 * ein Bild 8 Pixel hoch und 4 Pixel breit, so kommen zuerst die 4 Pixel der ersten Zeile, dann 
 * die der zweiten und so weiter:
 * 1100 0110 1111 1101 0100 1111 1011 1111
 * 1234 1234 1234 1234 1234 1234 1234 1234
 * 
 * Version 2 (schwieriger): Generell werden die Punkte ähnlich gespeichert, allerdings immer maximal 
 * 8 pro Zeile direkt nacheinander. Ist ein Bild z.B. 12 Pixel breit und 2 Pixel hoch, so werden
 * zunächst die ersten 8 Pixel der ersten, dann die ersten 8 Pixel der zweiten Zeile geschrieben. Darauf 
 * folgen dann die restlichen 4 der ersten und dann der zweiten Zeile:
 * 11000110 11111101 0100 1111
 * 12345678 12345678 1234 1234
 */

import java.io.BufferedReader;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.InputStreamReader;

public class SimpleIcon {
	
	private String name;
	private int fileVersion = 1;
	private int width = 8;
	private int height = 8;
	private char[][] data;
	
	private static final String delim = ";;";
	
	public SimpleIcon() {
		this.name = "EMPTY";
		this.width = 8;
		this.height = 8;
		this.data = null;
	}
	
	/**
	 * Construct a simple icon object by loading the icon data from given file.
	 * @param file
	 * @throws Exception
	 */
	public SimpleIcon(String file) throws Exception {
		if (!this.loadFromFile(file))
			throw new Exception("Failed to create SimpleIcon.");
	}
	
	/**
	 * Loads the icon data from the given file.
	 * @param file
	 * @return TRUE if the file was loaded successfully
	 */
	public boolean loadFromFile(String file) {
		FileInputStream fis;
		
		try { fis = new FileInputStream(file); }
		catch (FileNotFoundException e) {
			System.err.println("File " + file + " does not exist. Abort.");
			return false;
		}
		
		//Create DataInputStream and read file data
		InputStreamReader inr = new InputStreamReader(fis);
		BufferedReader br = new BufferedReader(inr);
		StringBuilder fileContent = new StringBuilder();
		
		try {
			String line;
			while ((line = br.readLine()) != null)
				fileContent.append(line);
			br.close();
			inr.close();
			fis.close();
		} catch (Exception e) {
			System.err.println(e.getStackTrace());
			return false;
		}
		
		//----
		//Parse file data for later use
		try { this.parseHeader(fileContent.toString()); }
		catch (Exception e) { System.err.println("SimpleIcon.loadFromFile: "
				+ "Failed to parse icon data:\n" + e.getMessage());
		}
		return true;
	}
	
	/**
	 * Parses file header information like name, size etc and writes them into the object attributes.
	 * @param fileContent
	 * @throws Exception
	 */
	private void parseHeader(String fileContent) throws Exception {
		if (fileContent == null)
			throw new Exception("SimpleIcon.parseHeader: "
					+ "fileContent can not be null");
		
		String tmp[] = fileContent.split(SimpleIcon.delim);
		if (tmp.length != 4)
			throw new Exception("SimpleIcon.parseHeader: "
					+ "fileContent has illegal format (expect 4 fields)");
		
		//----
		//Set header data
		this.name = tmp[0];
		this.fileVersion = Integer.parseInt(tmp[1]);
		String fileData = tmp[3];
		
		tmp = tmp[2].split("x");
		this.width = Integer.parseInt(tmp[0]);
		this.height = Integer.parseInt(tmp[1]);
		
		//----
		//Parse data
		if (this.fileVersion == 1)
			this.data = this.parseData(fileData);
		else
			this.data = this.parseDataV2(fileData);
	}
	
	/**
	 * Parses the file data string of 0's and 1's [Version 1]
	 * @param fileData
	 * @return image pixel data as two-dimensional array.
	 */
	private char[][] parseData(String fileData) {
		int offset = 0;
		char[][] data = new char[this.height][this.width];
		
		for (int col = 0; col < this.height; ++col) {
			for (int row = 0; row < this.width; ++row) {
				data[col][row] = fileData.charAt(offset++);
			}
		}
		
		return data;
	}
	
	/**
	 * Parses the file data string of 0's and 1's [Version 1]
	 * @param fileData
	 * @return image pixel data as two-dimensional array.
	 */
	private char[][] parseDataV2(String fileData) {
		int charPos = 0;
		int offset = 0;
		char[][] data = new char[this.height][this.width];
		
		for (offset = 0; offset < this.width; offset += 8) {
			for (int col = 0; col < this.height; ++col) {
				for (int row = offset; row < offset+8; ++row) {
					if (row >= this.width) continue;
					data[col][row] = fileData.charAt(charPos++);
				}
			}
		}
		
		return data;
	}
	
	/**
	 * Display currently set image data.
	 */
	public void display() {
		System.out.println(this.name + " (" + this.width + "x" + this.height + ")\n"
				+ "Version: " + this.fileVersion + "\n");
		
		for (int col = 0; col < this.height; ++col) {
			for (int row = 0; row < this.width; ++row) {
				if (this.data[col][row] == '1')
					System.out.print('x');
				else
					System.out.print(' ');
			}
			System.out.println();
		}
	}
	
}
