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

import java.io.{BufferedReader, FileInputStream, FileNotFoundException, 
    InputStreamReader}

/**
 * Construct a simple icon object by loading the icon data from given file.
 * @param file
 * @throws Exception
 */
class SimpleIcon(file : String) {
    private var delim = ";;"
    
    var mName = "EMPTY"
    var mFileVersion = 1
    var mWidth = 8
    var mHeight = 8
    var mData : Array[Array[Char]] = _
    
    if (!loadFromFile(file)) {
        throw new Exception("Failed to create SimpleIcon")
    }
    
    /**
	 * Loads the icon data from the given file.
	 * @param file
	 * @return TRUE if the file was loaded successfully
	 */
    def loadFromFile(file : String) : Boolean = {
        var fis : FileInputStream = null
        try {
            fis = new FileInputStream(file)
        } catch {
            case ex : FileNotFoundException => {
                println("File " + file + " does not exist. Abort.")
                return false
            }
        }
        
        //Create DataInputStream and read file data
        var inr = new InputStreamReader(fis)
        var br = new BufferedReader(inr)
        var fileContent = new StringBuilder()
        
        var line = br.readLine()
        while (line != null) {
            fileContent.append(line)
            line = br.readLine()
        }
        br.close()
        inr.close()
        fis.close()
        
        //----
		//Parse file data for later use
        
        try {
            parseHeader(fileContent.toString())
        } catch {
            case ex : Exception => {
                ex.printStackTrace()
                return false
            }
        }
        
        return true
    }
    
    /**
	 * Parses file header information like name, size etc and writes them into the object attributes.
	 * @param fileContent
	 * @throws Exception
	 */
    def parseHeader(fileContent : String) = {
        var tmp = fileContent.split(delim)
        if (tmp.length != 4) {
            throw new IllegalArgumentException("SimpleIcon.parseHeader: "
                + "fileContent has illegal format (expect 4 fields)")
        }
        
        //----
		//Set header data
        
        mName = tmp(0)
        mFileVersion = tmp(1).toInt
        var fileData = tmp(3)
        
        tmp = tmp(2) split "x"
        mWidth = tmp(0).toInt
        mHeight = tmp(1).toInt
        
        //----
        // Parse data
        
        mData = if (mFileVersion == 1) parseData(fileData) else parseDataV2(fileData)
        
    }
    
    /**
	 * Parses the file data string of 0's and 1's [Version 1]
	 * @param fileData
	 * @return image pixel data as two-dimensional array.
	 */
    private def parseData(fileData : String) : Array[Array[Char]] = {
        var offset = 0
        var data = Array.ofDim[Char](mHeight, mWidth)
        
        var col = 0
        for (col <- 0 to mHeight - 1) {
            var row = 0
            for (row <- 0 to mWidth - 1) {
                data(col)(row) = fileData(offset)
                offset += 1
            }
        }
        
        return data
    }
    
    /**
	 * Parses the file data string of 0's and 1's [Version 1]
	 * @param fileData
	 * @return image pixel data as two-dimensional array.
	 */
    private def parseDataV2(fileData : String) : Array[Array[Char]] = {
        var offset = 0
        var charPos = 0
        var data = Array.ofDim[Char](mHeight, mWidth)
        
        while (offset < mWidth) {
            var col = 0
            for (col <- 0 to mHeight - 1) {
                var row = offset
                for (row <- offset to offset + 7) {
                    if (row < mWidth) {
                        data(col)(row) = fileData(charPos)
                        charPos += 1
                    }
                }
            }
            
            offset += 8
        }
        
        return data
    }
    
    /**
	 * Display currently set image data.
	 */
    def display() = {
        println(mName + " (" + mWidth + "x" + mHeight + ")\n" 
            + "Version: " + mFileVersion + "\n")
        
        var col = 0
        for (col <- 0 to mHeight - 1) {
            var row = 0
            for (row <- 0 to mWidth - 1) {
                print(if (mData(col)(row) == '1') 'x' else ' ')
            }
            println(' ')
        }
    }
}
