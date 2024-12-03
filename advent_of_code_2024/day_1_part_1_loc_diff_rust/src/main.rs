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

    let mut left_nums: Vec<i64> = vec![0; line_count];
    let mut right_nums: Vec<i64> = vec![0; line_count];

    for line in input.iter().enumerate(){
        for sub_str in line.1.split_whitespace().into_iter().enumerate(){
            if sub_str.0 & 1 == 1{//im so bad at rust
                right_nums[line.0] = sub_str.1.parse().unwrap();
            }else{
                left_nums[line.0] = sub_str.1.parse().unwrap();
            }
        }
    }
}
