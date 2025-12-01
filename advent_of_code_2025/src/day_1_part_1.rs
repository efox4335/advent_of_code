use std::io;

fn parse_num(val: &str) -> (char, i32) {
    let mut str_iter = val.chars();

    let dir = str_iter.next().expect("no direction");

    let mut rot_amount: i32 = 0;

    for num in str_iter {
        if !num.is_numeric() {
            break;
        }

        rot_amount *= 10;
        rot_amount += num.to_digit(10).unwrap() as i32;
    }

    (dir, rot_amount)
}

pub fn solver() {
    let mut dial_num = 50;
    let mut password = 0;

    for line in io::stdin().lines() {
        let (dir, mut rot_amount) = parse_num(line.unwrap().as_str());

        rot_amount %= 100;

        if dir == 'R' {
            dial_num += rot_amount;
        } else {
            dial_num += 100;
            dial_num -= rot_amount;
        }

        dial_num %= 100;

        if dial_num == 0 {
            password += 1;
        }
    }

    println!("password {password}");
}
