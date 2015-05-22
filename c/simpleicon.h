/* Copyright (C) 2013 Maurice Bleuel <mandrakey@lavabit.com>
 *
 * This file is part of SimpleIcon, plain C version.
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

/**
 * Contains declarations for SimpleIcon data functions.
 * @file simpleicon.h
 * @author Maurice Bleuel
 */

#ifndef SIMPLEICON_H
#define SIMPLEICON_H

#define SI_DELIM ";;"

#define SI_NO_ERROR 0
#define SI_ILLEGAL_INPUT_FORMAT 1

#define SI_POS_NAME 0
#define SI_POS_VERSION 1
#define SI_POS_SIZE 2
#define SI_POS_DATA 3

/**
 * Data struct representing data of a SimpleIcon.
 */
typedef struct
{
    char *name;
    int file_version;
    int width;
    int height;
    char **data;
} SimpleIcon;

/**
 * Create a SimpleIcon struct from given file.
 * @param file Path of the file to load data from
 * @return Pointer to struct in memory
 */
SimpleIcon *SimpleIcon_create(const char *file);

/**
 * Destroy an existing SimpleIcon struct.
 * @param _si Pointer to struct to delete from memory
 */
void SimpleIcon_destroy(SimpleIcon *_si);

/**
 * Load data from given file into a SimpleIcon struct.
 * @param _si Pointer to struct to be filled
 * @param file File-struct pointer to opened file to read from
 * @return Success or not? (see constants.h)
 */
int SimpleIcon_loadFromFile(SimpleIcon *_si, FILE *file);

/**
 * Display a SimpleIcon struct's data on screen.
 * @param _si Pointer to SimpleIcon to display
 */
void SimpleIcon_display(SimpleIcon *_si);

/**
 * Parse header data from file contents.
 * @param _si Pointer to SimpleIcon struct to fill
 * @param fileContent String read from file
 * @return Numeric error/success value. One of
 * - SI_NO_ERROR
 * - SI_ILLEGAL_INPUT_FORMAT
 */
int SimpleIcon_parseHeader(SimpleIcon *_si, const char *fileContent);

/**
 * Parse SimpleIcon version 1 data.
 * @param _si Pointer to struct to fill
 * @param fileData String of icon data read from file
 */
void SimpleIcon_parseData(SimpleIcon *_si, const char *fileData);

/**
 * Parse SimpleIcon version 2 data.
 * @param _si Pointer to struct to fill
 * @param fileData String of icon data read from file
 */
void SimpleIcon_parseDataV2(SimpleIcon *_si, const char *fileData);

#endif
