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
 * Contains the program entry point.
 * @file main.c
 * @author Maurice Bleuel
 */

#include <stdio.h>
#include <stdlib.h>

#include "constants.h"
#include "simpleicon.h"

int main()
{
    SimpleIcon *si1 = SimpleIcon_create("data/schwert2.txt");

    if (!si1) {
        printf("Error loading files\n");
        return 1;
    }

    SimpleIcon_display(si1);
    SimpleIcon_destroy(si1);

}
