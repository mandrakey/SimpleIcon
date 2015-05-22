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

#include "SimpleIcon.h"

#include <fstream>
using std::ifstream;

#include <vector>
using std::vector;

#include <sstream>
using std::stringstream;

#include <iostream>
using std::cout;
using std::endl;

const string SimpleIcon::DELIM = ";;";

SimpleIcon::SimpleIcon() :
    mName(), mFileVersion(1), mWidth(8), mHeight(8), mData(0)
{
}

SimpleIcon::SimpleIcon(const string &file) :
    SimpleIcon()
{
    loadFromFile(file);
}

SimpleIcon::~SimpleIcon()
{
    if (mData) {
        for (int i = 0; i < mHeight; ++i) {
            if (mData[i]) {
                delete mData[i];
            }
        }
        delete mData;
    }

    mData = 0;
}

bool SimpleIcon::loadFromFile(const string &file)
{
    ifstream fs(file);
    string fileContent;
    string line;

    while (getline(fs, line)) {
        fileContent += line;
    }

    // Parse file content
    parseHeader(fileContent);
    return true;
}

void SimpleIcon::display() const
{
    cout << mName << "(" << mWidth << "x" << mHeight << ")" << endl <<
            "Version: " << mFileVersion << endl;

    for (int col = 0; col < mHeight; ++col) {
        for (int row = 0; row < mWidth; ++row) {
            cout << ((mData[col][row] == '1') ? 'x' : ' ');
        }
        cout << endl;
    }
}

int SimpleIcon::parseHeader(const string &fileContent)
{
    //istringstream stream(fileContent);
    string text(fileContent);
    vector<string> tokens;
    size_t pos = std::string::npos;

    while ((pos = text.find(DELIM)) != std::string::npos) {
        tokens.push_back(text.substr(0, pos));
        text.erase(0, pos + DELIM.length());
    }
    tokens.push_back(text);

    // Check file format
    if (tokens.size() != 4) {
        std::cerr << "Input file has illegal format (expect 4 fields)" << endl;
        return SimpleIcon::Error::ILLEGAL_INPUT_FORMAT;
    }

    // Set data
    stringstream version(tokens[1]);
    mName = tokens[0];
    string fileData = tokens[3];
    if ((version >> mFileVersion).fail()) {
        std::cerr << "File version number '" << tokens[1] << "' is not a number" << endl;
        return SimpleIcon::Error::ILLEGAL_INPUT_FORMAT;
    }

    string size(tokens[2]);
    pos = size.find("x");
    if (pos == std::string::npos) {
        std::cerr << "Illegal image size value '" << size << "'" << endl;
        return SimpleIcon::Error::ILLEGAL_INPUT_FORMAT;
    }

    stringstream* str = new stringstream(size.substr(0, pos));
    if ((*str >> mWidth).fail()) {
        std::cerr << "Fail to convert width value '" << str->str() << "'" << endl;
        return SimpleIcon::Error::ILLEGAL_INPUT_FORMAT;
    }
    str = new stringstream(size.substr(++pos));
    if ((*str >> mHeight).fail()) {
        std::cerr << "Fail to convert height value '" << str->str() << "'" << endl;
        return SimpleIcon::Error::ILLEGAL_INPUT_FORMAT;
    }

    // parse data
    switch (mFileVersion) {
    case 1:
        parseData(fileData);
        break;
    case 2:
        parseDataV2(fileData);
        break;
    default:
        return SimpleIcon::Error::ILLEGAL_INPUT_FORMAT;
    }

    return SimpleIcon::Error::NO_ERROR;
}

void SimpleIcon::parseData(const string &fileData)
{
    int offset = 0;
    char **data;
    data = new char * [mHeight];
    for (int i = 0; i < mHeight; ++i)
        data[i] = new char[mWidth];


    for (int col = 0; col < mHeight; ++col) {
        for (int row = 0; row < mWidth; ++row) {
            data[col][row] = fileData[offset++];
        }
    }

    mData = data;
}

void SimpleIcon::parseDataV2(const string &fileData)
{
    int charPos = 0;
    int offset = 0;
    char **data;
    data = new char * [mHeight];
    for (int i = 0; i < mHeight; ++i)
        data[i] = new char[mWidth];

    for (offset = 0; offset < mWidth; offset += 8) {
        for (int col = 0; col < mHeight; ++col) {
            for (int row = offset; row < offset + 8; ++row) {
                if (row >= mWidth)
                    continue;
                data[col][row] = fileData[charPos++];
            }
        }
    }

    mData = data;
}

// GETTER / SETTER

const string & SimpleIcon::name() const
{
    return mName;
}

int SimpleIcon::fileVersion() const
{
    return mFileVersion;
}

int SimpleIcon::width() const
{
    return mWidth;
}

int SimpleIcon::height() const
{
    return mHeight;
}

char ** SimpleIcon::data() const
{
    return mData;
}
