/*
 * parse input into vector of strings get sum of the absolute value of the differences between the first and second number
*/
use std::io;

fn main() {
    let mut input: Vec<String> = vec![String::new(); 1];
    let mut line_count:usize = 0;

    while io::stdin().read_line(&mut input[line_count]).unwrap() > 0{
        line_count += 1;
        input.resize(line_count + 1, String::new());
    }
    input.resize(line_count, String::new());//delets last empty line


}
