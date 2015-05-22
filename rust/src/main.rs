pub mod simple_icon;

use simple_icon::SimpleIconBuilder;

fn main() {
    let mut s = SimpleIconBuilder::new();
    s.filename("schwert2.txt".to_string());
    let si = s.finish();
    
    if si.is_some() {
        si.unwrap().display();
    }
}
