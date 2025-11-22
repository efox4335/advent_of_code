use std::io::stdin;

pub fn solver() {
    let mut valid_pass = 0;

    for line in stdin().lines() {
        let temp = line.unwrap();
        let mut sub_str = temp.split([' ', ':', '\n', '-']);

        let lower_bound: usize = sub_str.next().unwrap().parse::<usize>().unwrap() - 1;
        let upper_bound: usize = sub_str.next().unwrap().parse::<usize>().unwrap() - 1;

        let letter: char = sub_str.next().unwrap().chars().nth(0).unwrap();

        sub_str.next();
        let pass: Vec<char> = sub_str.next().unwrap().chars().collect();

        if (pass[lower_bound] == letter || pass[upper_bound] == letter)
            && pass[lower_bound] != pass[upper_bound]
        {
            valid_pass += 1;
        }
    }

    println!("{}", valid_pass);
}
