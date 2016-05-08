# SimpleIcon
Ziel ist es ein Programm zu entwickeln, welches in der Lage ist eine 
als ASCII-Text abgespeicherte Repräsentation eines Bildes zu laden 
und anzuzeigen. Informationen zum Bild sowie die eigentlichen 
Bildpunkte sind in besagter Datei gespeichert. Für die Anzeige sollen 
die einzelnen Bildpunkte geeignet angezeigt werden (z. B. mit dem 
Zeichen *), so dass sich das gewünschte Bild ergibt.

## Eingabeformat
Jede Bilddatei enthält die folgenden Informationen:

* Den Namen des Bildes (z. B. »Schwert«)
* Die Version der Bilddatei, welche den zu verwendenden Algorithmus 
	zur Berechnung des fertigen Bildes bezeichnet
* Die Dimensionen des Bildes in Höhe x Breite, z. B. »8x8«
* Die eigentlichen Bilddaten als Folge der Zeichen 0 und 1

Diese vier Informationsfelder sind in jeder Bilddatei in obiger 
Reihenfolge enthalten und jeweils durch doppelte Semikolon-Zeichen 
»;;« voneinander getrennt.

## Versionen
Es gibt zwei Versionen des Algorithmus für die Berechnung der 
Bildinformation:

**Version 1 (Einfacher)**  
Die Bildpunkte werden einfach zeilenweise, von links nach rechts, 
hintereinander in 0 und 1 kodiert abgelegt. Hat ein Bild also die 
Dimensionen 4x12, so werden zuerst die vier Punkte der ersten Zeile 
notiert, dann die der zweiten Zeile und so weiter:

    Code:   000001100000 000011110000 001111111100 000001100000
    Spalte: 123456789ABC 123456789ABC 123456789ABC 123456789ABC
    Zeile:  1            2            3            4

Daraus ergibt sich dann folgendes Bild:

    .  123456789ABC
    1       **
    2      ****
    3    ********
    4       **   

**Version 2**  
Diese Version speichert die Daten in Anlehnung an das alte MS-DOS 
FON-Format für Schriftzeichen. Auch hier werden die Bildpunkte 
Zeilenweise, von links nach rechts, hintereinander in 0 und 1 kodiert. 
Allerdings wird das Bild horizontal in Blöcke von 8 Pixeln zerlegt. D. h. 
die Speicherung erfolgt für das gleiche Bild in Größe 4x12 wie folgt:

1. Die ersten 8 Pixel der ersten Zeile
2. Die ersten 8 Pixel der zweiten Zeile
3. Die ersten 8 Pixel der dritten Zeile
4. Die ersten 8 Pixel der vierten Zeile
5. Die nächsten bis zu 8 Pixel der ersten Zeile
6. etc

.  

    Code:   00000110 00001111 00111111 00000110 0000 0000 1100 0000
    Spalte: 12345678 12345678 12345678 12345678 9ABC 9ABC 9ABC 9ABC
    Zeile:  1        2        3        4        1    2    3    4
    
Auch dies ergibt das folgende Bild:

    .  123456789ABC
    1       **     
    2      ****    
    3    ********  
    4       **     

## Beispiele
Die folgenden Beispiele muss das Programm korrekt auslesen und 
darstellen können:

    Version 1:
    
    Schwert;;1;;10x3;;001000000011111111110010000000
    
    Version 2:
    
    Schwert;;2;;10x3;;001000001111111100100000001100
