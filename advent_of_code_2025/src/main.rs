use std::env;

mod day_1_part_1;
mod day_1_part_2;
mod day_2_part_1;
mod day_2_part_2;
mod day_3_part_1;
mod day_3_part_2;
mod day_4_part_1;
mod day_4_part_2;
fn main() {
    let args: Vec<String> = env::args().collect();

    if args.len() != 2 {
        panic!("part and day required");
    }

    match args[1].as_str() {
        "d1p1" => day_1_part_1::solver(),
        "d1p2" => day_1_part_2::solver(),
        "d2p1" => day_2_part_1::solver(),
        "d2p2" => day_2_part_2::solver(),
        "d3p1" => day_3_part_1::solver(),
        "d3p2" => day_3_part_2::solver(),
        "d4p1" => day_4_part_1::solver(),
        "d4p2" => day_4_part_2::solver(),
        _ => panic!("unknown day/part"),
    }
}
