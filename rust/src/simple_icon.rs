use std::fs::File;
use std::io::Read;
use std::str::FromStr;

#[doc = "Delimiter for simple icon filetype header."]
static SIMPLEICON_DELIMITER : &'static str = ";;";

#[doc = "Represents a **SimpleIcon**."]
pub struct SimpleIcon {
    name : String,
    file_version : u16,
    width : u32,
    height : u32,
    data : Vec<Vec<char>>
}

impl SimpleIcon {

    #[doc = "Display contents of a parsed **SimpleIcon**."]
    pub fn display(&self) {
        println!("{} ({}x{})\nVersion: {}\n", 
            self.name, self.width, self.height, self.file_version);
        
        for col in 0..self.height {
            for row in 0..self.width {
                print!("{}", 
                    match self.data[col as usize][row as usize] {
                        '1' => "x",
                        _ => " "
                    });
            }
            println!(" ");
        }
    }

}

#[doc = "Builder for SimpleIcon objects."]
pub struct SimpleIconBuilder {
    filename : String,
    obj : SimpleIcon
}

impl SimpleIconBuilder {
    
    //==========================================================================
    // STATIC METHODS
    
    #[doc = "Create a new SimpleIconBuilder."]
    pub fn new() -> SimpleIconBuilder {
        let s = SimpleIconBuilder { filename: String::new(), 
            obj: SimpleIcon { name: "EMPTY".to_string(), file_version: 0, 
            width: 0, height: 0, data: vec![vec![' ' ]] } };
        return s;
    }
    
    //==========================================================================
    // INSTANCE METHODS
    
    #[doc = "Set filename to load data from."]
    pub fn filename(& mut self, v_filename : String) {
        self.filename = v_filename;
    }
    
    #[doc = "Construct SimpleIcon and return it."]
    pub fn finish(& mut self) -> Option<&SimpleIcon> {
        if !self.load_from_file() {
            println!("Failed to load icon from file '{}'.", self.filename);
            return None;
        }
        return Some(&self.obj);
    }
    
    //--------------------------------------------------------------------------
    // private helper functions
    
    #[doc = "Load data for SimpleIcon from previously defined file."]
    fn load_from_file(&mut self) -> bool {
        let mut f = match File::open(&self.filename) {
            Ok(f) => f,
            Err(_) => {
                println!("Failed to open file");
                return false;
            }
        };
        
        let mut file_content = String::new();
        match f.read_to_string(&mut file_content) {
            Err(e) => {
                println!("Could not read from file: {:?}", e);
                return false;
            }
            _ => {}
        };
        
        self.parse_header(&file_content)
    }
    
    #[doc = "Parse SimpleIcon file header."]
    fn parse_header(&mut self, file_content : &String) -> bool {
        let mut tmp : Vec<&str> = file_content.split(SIMPLEICON_DELIMITER).collect();
        
        self.obj.name = tmp[0].to_string();
        self.obj.file_version = match u16::from_str(tmp[1]) {
            Ok(n) => n,
            Err(e) => {
                println!("Conversion error: {:?}", e);
                return false;
            }
        };
        let file_data = tmp[3];
        
        tmp = tmp[2].split('x').collect();
        self.obj.width = match u32::from_str(tmp[0]) {
            Ok(n) => n,
            Err(e) => {
                println!("Conversion error: {:?}", e);
                return false;
            }
        };
        self.obj.height = match u32::from_str(tmp[1]) {
            Ok(n) => n,
            Err(e) => {
                println!("Conversion error: {:?}", e);
                return false;
            }
        };
        
        self.obj.data = match self.obj.file_version {
            1 => self.parse_data(file_data.chars().collect()),
            _ => self.parse_data_v2(file_data.chars().collect())
        };
        return true;
    }
    
    #[doc = "Parse SimpleIcon contents according to version 1."]
    fn parse_data(&self, file_data : Vec<char>) -> Vec<Vec<char>> {
        let mut offset = 0;
        let mut data : Vec<Vec<char>> = Vec::new();
        
        for col in 0..self.obj.height {
            data.push(Vec::new());
            for _ in 0..self.obj.width {
                data[col as usize].push(file_data[offset]);
                offset += 1;
            }
        }
        
        return data;
    }
    
    #[doc = "Parse SimpleIcon contents according to version 2."]
    fn parse_data_v2(&self, file_data : Vec<char>) -> Vec<Vec<char>> {
        let mut offset = 0;
        let mut char_pos = 0;
        let mut data : Vec<Vec<char>> = Vec::new();
        
        while offset < self.obj.width {
            for col in 0..self.obj.height {
                data.push(Vec::new());
                for row in offset..offset + 8 {
                    if row < self.obj.width {
                        data[col as usize].push(file_data[char_pos]);
                        char_pos += 1;
                    }
                }
            }
            
            offset += 8;
        }
        
        return data;
    }
}
