package main;

import (
  "fmt"
  "strings"
  "strconv"
  "errors"
  "bufio"
  "os"
  "flag"
)

// =============================================================================
// Class SimpleIcon

const DELIM = ";;"

type SimpleIcon struct {
  name string
  fileVersion int32
  width int32
  height int32
  data []byte
}

func NewSimpleIcon(path string) (*SimpleIcon, error) {
  file, err := os.Open(path)
  if err != nil {
    return nil, err
  }

  scanner := bufio.NewScanner(file)
  if !scanner.Scan() {
    return nil, errors.New("Failed to read from file")
  }

  fileContent := scanner.Text()
  res := SimpleIcon {}
  res.ParseHeader(fileContent)

  file.Close()
  return &res, nil
}

func (s *SimpleIcon) ParseHeader(fileContent string) error {
  if len(fileContent) == 0 {
    return errors.New("Cannot parse empty file")
  }

  tmp := strings.Split(fileContent, DELIM)
  if len(tmp) != 4 {
    return errors.New("File content has illegal format (expect 4 fields)")
  }

  size := tmp[2]
  s.name = tmp[0]

  //var err error = nil
  fileVersion, err := strconv.ParseInt(tmp[1], 10, 32)
  if err != nil {
    return err
  }
  s.fileVersion = int32(fileVersion)

  s.data = make([]byte, len(tmp[3]))
  for i := 0; i < len(tmp[3]); i++ {
    if tmp[3][i] == byte("1"[0]) {
      s.data[i] = 1
    } else {
      s.data[i] = 0
    }
  }

  tmp = strings.Split(size, "x")
  w, err := strconv.Atoi(tmp[0])
  if err != nil {
    return err
  }
  s.width = int32(w)

  h, err := strconv.Atoi(tmp[1])
  if err != nil {
    return err
  }
  s.height = int32(h)

  return nil
}

func (s *SimpleIcon) ParseData() [][]byte {
  offset := 0
  var data [][]byte
  data = make([][]byte, s.height)

  var row int32
  var col int32
  for row = 0; row < s.height; row++ {
    data[row] = make([]byte, s.width)
    for col = 0; col < s.width; col++ {
      data[row][col] = s.data[offset]
      offset += 1
    }
  }

  return data
}

func (s *SimpleIcon) ParseDataV2() [][]byte {
  charPos := 0
  offset := int32(0)
  data := make([][]byte, s.height)

  for offset <= s.width {
    for row := int32(0); row < s.height; row++ {
      if data[row] == nil {
        data[row] = make([]byte, s.width)
      }
      for col := offset; col < (offset + 8); col++ {
        if col >= s.width {
          continue
        }

        data[row][col] = s.data[charPos]
        charPos += 1
      }
    }

    offset += 8
  }

  return data
}

func (s *SimpleIcon) Display() {
  var data [][]byte
  if s.fileVersion == 1 {
    data = s.ParseData()
  } else {
    data = s.ParseDataV2()
  }

  fmt.Printf("%s (%dx%d)\nVersion: %d\n", s.name, s.width, s.height, s.fileVersion)

  var row int32
  var col int32
  for row = 0; row < s.height; row++ {
    for col = 0; col < s.width; col++ {
      out := ' '
      if data[row][col] == 1 {
        out = 'x'
      }
      fmt.Printf("%c", out)
    }
    fmt.Printf("\n")
  }
}

// =============================================================================
// Processing

func main() {
  flag.Parse()
  args := flag.Args()

  if len(args) != 1 {
    fmt.Println("Need input icon file as parameter")
    return
  }

  s, err := NewSimpleIcon(args[0])
  if err != nil {
    fmt.Printf("Error: %s\n", err)
    return
  }

  s.Display()
}
