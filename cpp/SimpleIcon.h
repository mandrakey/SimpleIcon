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

#ifndef SIMPLEICON_H
#define SIMPLEICON_H

#include <string>
using std::string;

/**
 * Class representing a SimpleIcon instance.
 */
class SimpleIcon
{
public:
    /**
     * Error codes for SimpleIcon method return values.
     */
    enum Error {
        NO_ERROR = 0,
        ILLEGAL_INPUT_FORMAT = 1
    };

    /**
     * Construct an empty SimpleIcon object.
     */
	SimpleIcon();

    /**
     * Construct a SimpleIcon object loading data from a specified file.
     * @param file File path to load data from
     */
    explicit SimpleIcon(const string &file);

    ~SimpleIcon();

    /**
     * Loads content from specified file and parses it.
     * @param file Path to load data from
     * @return true if file reading and parsing succeeded
     */
    bool loadFromFile(const string &file);

    /**
     * Display the parsed image as "ascii art".
     */
    void display() const;

    // GETTER / SETTER
    const string & name() const;
    int fileVersion() const;
    int width() const;
    int height() const;
    char ** data() const;
	
private:
    /** Field delimiter in image datafiles. */
    static const string DELIM;

    string mName;
	int mFileVersion;
	int mWidth;
	int mHeight;
    char **mData;

    /**
     * Parse file header data and prepare to parse content.
     * Splits file content on DELIM to obtain fields. Sets header information,
     * then passes image data string either to parseData or parseDataV2.
     * @param fileContent Complete file content as loaded from the file
     * @return Some value from SimpleIcon::Error. On Success, returns
     * NO_ERROR.
     */
    int parseHeader(const string &fileContent);

    /**
     * Parses the image data string into usable form.
     * Uses file version 1.
     * @param fileData Content of the data field from the file contents
     */
    void parseData(const string &fileData);

    /**
     * Parses the image data string into usable form.
     * Uses file version 2.
     * @param fileData Content of the data field from the file contents
     */
    void parseDataV2(const string &fileData);
};

#endif
