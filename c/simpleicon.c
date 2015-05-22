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
 * Implementation of SimpleIcon functionality.
 * @file simpleicon.c
 * @author Maurice Bleuel
 */

#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <stdlib.h>

#include "constants.h"
#include "simpleicon.h"

SimpleIcon *SimpleIcon_create(const char *file)
{
    if (!file) {
        return 0;
    }

    FILE *f = fopen(file, "rt");
    if (!f) {
        return 0;
    }

    SimpleIcon *_si = malloc(sizeof(SimpleIcon));
    SimpleIcon_loadFromFile(_si, f);

    fclose(f);
    return _si;
}

void SimpleIcon_destroy(SimpleIcon *_si)
{
    free(_si->name);
    for (int i = 0; i < _si->height; ++i) {
        free(_si->data[i]);
        _si->data[i] = 0;
    }
    free(_si->data);
    free(_si);

    _si->name = 0;
    _si->data = 0;
    _si = 0;
}

int SimpleIcon_loadFromFile(SimpleIcon *_si, FILE *file)
{
    char *file_content = malloc(sizeof(char) * 2048);
    for (int i = 0; i < 2048; ++i) {
        file_content[i] = '\0';
    }
    fgets(file_content, 2048, file);
    SimpleIcon_parseHeader(_si, file_content);

    free(file_content);
    return TRUE;
}

void SimpleIcon_display(SimpleIcon *_si)
{
    printf("%s (%dx%d)\nVersion: %d\n",
           _si->name, _si->width, _si->height, _si->file_version);

    for (int col = 0; col < _si->height; ++col) {
        for (int row = 0; row < _si->width; ++row) {
            char out = (_si->data[col][row] == '1') ? 'x' : ' ';
            printf("%c", out);
        }
        printf("\n");
    }
}

int SimpleIcon_parseHeader(SimpleIcon *_si, const char *file_data)
{
    char *text = malloc(sizeof(char) * (strlen(file_data) + 1));
    strncpy(text, file_data, strlen(file_data) + 1);
    int pos = SI_POS_NAME;

    char *buf;
    char *versionbuf;
    char *file_content;
    buf = strtok(text, SI_DELIM);
    while (buf) {
        switch (pos) {
        case SI_POS_NAME:
            _si->name = malloc(sizeof(char) * strlen(buf) + 1);
            strncpy(_si->name, buf, strlen(buf) + 1);
            pos = SI_POS_VERSION;
            break;
        case SI_POS_VERSION:
            _si->file_version = atoi(buf);
            pos = SI_POS_SIZE;
            break;
        case SI_POS_SIZE:
            versionbuf = buf;
            pos = SI_POS_DATA;
            break;
        case SI_POS_DATA:
            file_content = malloc(sizeof(char) * strlen(buf));
            strcpy(file_content, buf);
            break;
        }

        buf = strtok(0, SI_DELIM);
    }

    // Parse version text
    _si->width = atoi(strtok(versionbuf, "x"));
    _si->height = atoi(versionbuf);

    // Parse data
    int retval = SI_NO_ERROR;
    switch (_si->file_version) {
    case 1:
        SimpleIcon_parseData(_si, file_content);
        break;
    case 2:
        SimpleIcon_parseDataV2(_si, file_content);
        break;
    default:
        retval = SI_ILLEGAL_INPUT_FORMAT;
    }

    // Cleanup
    free(buf);
    free(text);
    free(file_content);

    return retval;
}

void SimpleIcon_parseData(SimpleIcon *_si, const char *fileData)
{
    int offset = 0;
    char **data;
    data = malloc(sizeof(char *) * _si->height);
    for (int i = 0; i < _si->height; ++i) {
        data[i] = malloc(sizeof(char) * _si->width);
    }

    for (int col = 0; col < _si->height; ++col) {
        for (int row = 0; row < _si->width; ++row) {
            data[col][row] = fileData[offset++];
        }
    }

    _si->data = data;
}

void SimpleIcon_parseDataV2(SimpleIcon *_si, const char *file_data)
{
    int charPos = 0;
    int offset = 0;
    char **data;
    data = malloc(sizeof(char *) * _si->height);
    for (int i = 0; i < _si->height; ++i) {
        data[i] = malloc(sizeof(char) * _si->width);
    }

    for (offset = 0; offset < _si->width; offset += 8) {
        for (int col = 0; col < _si->height; ++col) {
            for (int row = offset; row < offset + 8; ++row) {
                if (row < _si->width) {
                    data[col][row] = file_data[charPos++];
                }
            }
        }
    }

    _si->data = data;
}
