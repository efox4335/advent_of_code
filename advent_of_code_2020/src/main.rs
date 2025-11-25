use std::env;

mod day_13_part_1_bus;
mod day_13_part_2_bus;
mod day_1_part_1_accounting;
mod day_1_part_2_accounting;
mod day_22_part_1;
mod day_22_part_2;
mod day_23_part_1;
mod day_23_part_2;
mod day_2_part_1_password;
mod day_2_part_2_password;
mod day_3_part_1_toboggan;
mod day_3_part_2_toboggan;
mod day_4_part_1_passport;
mod day_4_part_2_passport;
mod day_5_part_1_bording;
mod day_5_part_2_bording;
fn main() {
    let part = env::args().nth(1).expect("add part argument");

    match part {
        s if s == "d1p1" => day_1_part_1_accounting::solver(),
        s if s == "d1p2" => day_1_part_2_accounting::solver(),
        s if s == "d2p1" => day_2_part_1_password::solver(),
        s if s == "d2p2" => day_2_part_2_password::solver(),
        s if s == "d3p1" => day_3_part_1_toboggan::solver(),
        s if s == "d3p2" => day_3_part_2_toboggan::solver(),
        s if s == "d4p1" => day_4_part_1_passport::solver(),
        s if s == "d4p2" => day_4_part_2_passport::solver(),
        s if s == "d5p1" => day_5_part_1_bording::solver(),
        s if s == "d5p2" => day_5_part_2_bording::solver(),
        s if s == "d13p1" => day_13_part_1_bus::solver(),
        s if s == "d13p1" => day_13_part_2_bus::solver(),
        s if s == "d22p1" => day_22_part_1::solver(),
        s if s == "d22p2" => day_22_part_2::solver(),
        s if s == "d23p1" => day_23_part_1::solver(),
        s if s == "d23p2" => day_23_part_2::solver(),
        _ => panic!("unknown part"),
    }
}
