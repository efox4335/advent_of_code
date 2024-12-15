/*
 * hash table where the key is the diff between 2020 and num
 * record is num
*/

use std::{collections::HashMap, io::stdin};

fn main() {
    let input: Vec<i32> = stdin()
        .lines()
        .map(|line| line.unwrap().parse::<i32>().unwrap())
        .collect();

    let mut map = HashMap::<i32, i32>::new();
    for num in input.iter() {
        map.insert(2020 - num, *num);
    }

    for num in input {
        match map.get(&num) {
            Some(val) => {
                println!("{}", val * num);
                std::process::exit(0);
            }
            _ => (),
        }
    }
}
