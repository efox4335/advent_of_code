/*
 * hash table where the key is the diff between 2020 and num
 * record is num
*/

use std::io::stdin;

fn main() {
    let input: Vec<i32> = stdin()
        .lines()
        .map(|line| line.unwrap().parse::<i32>().unwrap())
        .collect();
}
