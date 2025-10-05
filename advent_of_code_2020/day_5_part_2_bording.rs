use std::io::stdin;

const ROW_NUM: usize = 128;
const COL_NUM: usize = 8;

#[derive(Debug)]
struct Seat {
    row: usize,
    col: usize,
}

fn get_seat(bording_pass: &str) -> Seat {
    let mut upper_row = ROW_NUM - 1;
    let mut lower_row = 0;

    let mut right_col = COL_NUM - 1;
    let mut left_col = 0;

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

fn get_seat_id(seat: &Seat) -> usize {
    return seat.row * 8 + seat.col;
}

fn main() {
    let bording_passes: Vec<String> = stdin()
        .lines()
        .map(|line| -> String { line.unwrap().replace("\n", "") })
        .collect();

    let mut seats_occupied: [bool; ROW_NUM * COL_NUM] = [false; ROW_NUM * COL_NUM];

    for bording_pass in bording_passes {
        let seat: Seat = get_seat(&bording_pass);

        let cur_id = get_seat_id(&seat);

        seats_occupied[cur_id as usize] = true;
    }
}
