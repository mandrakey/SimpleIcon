package de.mandrakey.simpleicon

class SimpleIcon {

    static void main(args) {
        def s = new SimpleIcon('data/schwert2.txt')
        s.display()
    }

    static final def DELIM = ';;'

    String name
    int version
    int width
    int height
    int[][] data

    SimpleIcon(String filename) {
        loadFromFile filename
    }

    void loadFromFile(String filename) {
        def f = new File(filename)
        if (!f.exists()) {
            throw new IllegalArgumentException("File ${filename} does not exist")
        }

        def fileContent = f.readLines().join('').trim()
        parseHeader fileContent
    }

    void parseHeader(String fileContent) {
        def tmp = fileContent.split DELIM
        if (tmp.size() != 4) {
            throw new IllegalArgumentException('File content has illegal format (expect 4 fields)')
        }

        name = tmp[0]
        version = tmp[1] as int

        def size = tmp[2].split 'x'
        width = size[0] as int
        height = size[1] as int

        def fileData = tmp[3]

        switch (version) {
            case 1:
                parseData fileData
                break
            case 2:
                parseDataV2 fileData
                break
            default:
                throw new RuntimeException("Cannot parse file with unknown version ${version}")
        }
    }

    void parseData(String filedata) {
        def offset = 0
        data = new int[height][width]

        (0..height - 1).each { row ->
            (0..width - 1).each { col ->
                data[row][col] = filedata[offset++] as int
            }
        }
    }

    void parseDataV2(String filedata) {
        def offset = 0
        def charpos = 0
        data = new int[height][width]

        while (offset <= width) {
            (0..height - 1).each { row ->
                (offset..offset + 7).each { col ->
                    if (col >= width) return

                    data[row][col] = filedata[charpos++] as int
                }
            }

            offset += 8
        }
    }

    void display() {
        println "${name} (${width}x${height})\nVersion: ${version}"

        data.each { row ->
            row.each {
                print it == 1 ? 'x' : ' '
            }
            print "\n"
        }
    }

}