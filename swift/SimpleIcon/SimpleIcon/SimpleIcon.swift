import Foundation

enum SimpleIconError: ErrorType {
    case invalidSourceFile(file: String)
    case sourceFileDoesNotExist(file: String)
}

class SimpleIcon {
    
    var name: String = ""
    var fileVersion: Int = 0
    var width: Int = 0
    var height: Int = 0
    var data = [[Int]]()
    
    init(file: String) throws {
        if file == "" {
            throw SimpleIconError.invalidSourceFile(file: file)
        }
        
        let fileManager = NSFileManager()
        if !fileManager.fileExistsAtPath(file) {
            throw SimpleIconError.sourceFileDoesNotExist(file: file)
        }
        
        do {
            let fileContent = try NSString(contentsOfFile: file, encoding: NSUTF8StringEncoding)
            try self.parse(fileContent)
        } catch {
            throw error
        }
        
    }
    
    func parse(fileContent: NSString) throws {
        var tmp = fileContent.componentsSeparatedByString(";;")
        name = tmp[0]
        fileVersion = Int(tmp[1])!
        
        let tmp2 = tmp[2].componentsSeparatedByString("x")
        width = Int(tmp2[0])!
        height = Int(tmp2[1])!
        
        if fileVersion == 1 {
            parseContent(tmp[3])
        }
    }
    
    func parseContent(content: String) {
        var offset = 0
        data = [[Int]](count: height, repeatedValue: [Int](count: width, repeatedValue: 0))
        
        for row in 0..<height {
            for col in 0..<width {
                let c = content[content.startIndex.advancedBy(offset)]
                if c == "1" {
                    data[row][col] = 1
                } else {
                    data[row][col] = 0
                }
                offset += 1
            }
        }
    }
    
    func display() {
        print("\(name) (\(width)x\(height))\nVersion: \(fileVersion)")
        
        for row in 0..<height {
            for col in 0..<width {
                var c = " "
                if data[row][col] == 1 {
                    c = "x"
                }
                
                print(c, terminator: "")
            }
            print()
        }
    }
    
}
