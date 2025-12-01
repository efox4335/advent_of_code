use std::env;

mod day_1_part_1;
mod day_1_part_2;
fn main() {
    let args: Vec<String> = env::args().collect();

    if args.len() != 2 {
        panic!("part and day required");
    }

    match args[1].as_str() {
        "d1p1" => day_1_part_1::solver(),
        "d1p2" => day_1_part_2::solver(),
        _ => panic!("unknown day/part"),
    }
}
