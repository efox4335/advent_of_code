const ROW_NUM: u32 = 128;
const COL_NUM: u32 = 8;

#[derive(Debug)]
struct Seat {
    row: u32,
    col: u32,
}

fn get_seat(bording_pass: &str) -> Seat {
    let mut upper_row: u32 = ROW_NUM - 1;
    let mut lower_row: u32 = 0;

    let mut right_col: u32 = COL_NUM - 1;
    let mut left_col: u32 = 0;

    for letter in bording_pass.chars() {
        match letter {
            'F' => upper_row -= (upper_row - lower_row) / 2 + 1,
            'B' => lower_row += (upper_row - lower_row) / 2 + 1,
            'R' => left_col += (right_col - left_col) / 2 + 1,
            'L' => right_col -= (right_col - left_col) / 2 + 1,

            _ => println!("bad input"),
        }
    }

    return Seat {
        row: lower_row,
        col: left_col,
    };
}

fn main() {}
