/* Copyright (C) 2013 Maurice Bleuel <mandrakey@lavabit.com>
 *
 * This file is part of SimpleIcon, C++ STL version.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

/*
 * Programmieraufgabe: SimpleIcon
 *
 * Ziel ist es, eine Klasse zu schreiben welche in der Lage ist ein als
 * ASCII-Textdatei mit speziellem Format abgespeichertes Bild zu laden und
 * anzuzeigen. Jedes dieser Bilder hat folgende Zusatzinformationen:
 * - einen Namen
 * - die Version der verwendeten Spezifikation (siehe unten)
 * - eine Größe (in Form Höhe x Breite, also z.B. "8x8")
 *
 * Die eigentlichen Bildinformationen liegen als Ketten der Zeichen 1 und 0 vor.
 * Dort wo eine 1 steht, ist ein Bildpunkt zu malen.
 *
 * Alle diese Informationen sollen durch das Trennzeichen ";;" (doppeltes
 * Semikolon) voneinander getrennt sein und in folgender Reihenfolge in der
 * Datei stehen:
 *
 * name;;version;;größe;;daten
 *
 * Es gibt 2 verschiedene Spezifikationen die umgesetzt werden können:
 *
 * Version 1 (einfacher): Die Bildpunkte wurden Zeilenweise nacheinander
 * geschrieben. Ist also ein Bild 8 Pixel hoch und 4 Pixel breit, so kommen
 * zuerst die 4 Pixel der ersten Zeile, dann die der zweiten und so weiter:
 * 1100 0110 1111 1101 0100 1111 1011 1111
 * 1234 1234 1234 1234 1234 1234 1234 1234
 *
 * Version 2 (schwieriger): Generell werden die Punkte ähnlich gespeichert,
 * allerdings immer maximal 8 pro Zeile direkt nacheinander. Ist ein Bild z.B.
 * 12 Pixel breit und 2 Pixel hoch, so werden zunächst die ersten 8 Pixel der
 * ersten, dann die ersten 8 Pixel der zweiten Zeile geschrieben. Darauf folgen
 * dann die restlichen 4 der ersten und dann der zweiten Zeile:
 * 11000110 11111101 0100 1111
 * 12345678 12345678 1234 1234
 */

#include "SimpleIcon.h"

#include <iostream>
using std::cout;
using std::endl;

int main()
{
    SimpleIcon sv1("data/schwert2.txt");
    sv1.display();
}
