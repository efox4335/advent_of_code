/*
 * just count and compare
*/

use std::io::stdin;

pub fn solver() {
    let mut valid_pass = 0;

    for line in stdin().lines() {
        let temp = line.unwrap();
        let mut sub_str = temp.split([' ', ':', '\n', '-']);

        let lower_bound: i32 = sub_str.next().unwrap().parse().unwrap();
        let upper_bound: i32 = sub_str.next().unwrap().parse().unwrap();

        let letter: char = sub_str.next().unwrap().chars().nth(0).unwrap();

        sub_str.next();
        let count = sub_str
            .next()
            .unwrap()
            .chars()
            .filter(|c| *c as char == letter)
            .count() as i32;

        if count >= lower_bound && count <= upper_bound {
            valid_pass += 1;
        }
    }

    println!("{}", valid_pass);
}
