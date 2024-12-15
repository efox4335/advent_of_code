/*
 * hash table where the key is the diff between 2020 and num
 * create a secondary hash table with all the pairs of differences between that one and the input list
 * record is num
 *
 * should be O(n^2)
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

    let mut second_order_map = HashMap::<i32, i32>::new();
    for val in map.iter(){
        for num in input.iter(){
            second_order_map.insert(val.0 - num, *num);
        }
    }

    for num in input {
        match second_order_map.get(&num) {
            Some(val) => {
                let temp = val + num;
                println!("{}", val * num * map.get(&temp).unwrap());
                std::process::exit(0);
            }
            _ => (),
        }
    }
}
