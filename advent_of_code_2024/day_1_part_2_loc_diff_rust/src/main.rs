/*
 * parse input into vector of strings put both lists of nums in a hash map recording the number of occurrences when a val is found add the product of the val and the number of occurrences in the other array and add that to the product
 * this works because mul(a, b) = mul(a - 1, b) + b and the final product for each number takes the form num(left_num_count * right_num_count)
*/
use std::io;
use std::collections::HashMap;

fn main() {
    let mut input: Vec<String> = vec![String::new(); 1];
    let mut line_count:usize = 0;

    while io::stdin().read_line(&mut input[line_count]).unwrap() > 0{
        line_count += 1;
        input.resize(line_count + 1, String::new());
    }
    input.resize(line_count, String::new());//delets last empty line

    let mut left_nums: HashMap<i64, i64>= HashMap::new();
    let mut right_nums: HashMap<i64, i64>= HashMap::new();
    let mut prod:i64 = 0;

    for line in input.iter().enumerate(){
        let mut num_arr: [i64; 2] = [0, 0];
        for sub_str in line.1.split_whitespace().into_iter().enumerate(){//slightly less bad way of doing this
            num_arr[sub_str.0] = sub_str.1.parse().unwrap();
        }

        left_nums.entry(num_arr[0]).or_insert(0);
        let count = left_nums.get_key_value(&num_arr[0]).unwrap();
        left_nums.insert(num_arr[0], count.1 + 1);

        let zero = 0;
        let count = right_nums.get_key_value(&num_arr[0]).unwrap_or((&zero, &zero));
        prod += count.1 * num_arr[0];

        right_nums.entry(num_arr[1]).or_insert(0);
        let count = right_nums.get_key_value(&num_arr[1]).unwrap();
        right_nums.insert(num_arr[1], count.1 + 1);

        let count = left_nums.get_key_value(&num_arr[1]).unwrap_or((&zero, &zero));
        prod += count.1 * num_arr[1];
    }

    println!("{}", prod);
}
