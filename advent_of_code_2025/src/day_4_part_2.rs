use std::io;

struct Cords {
    row: usize,
    col: usize,
}

fn can_move(cord: Cords, floor: &Vec<Vec<u8>>) -> bool {
    let mut adj_rolls = 0;

    for col_off in -1..=1 {
        let col_check = cord.col as i32 + col_off;

        if col_check < 0 {
            continue;
        }

        let col_check = col_check as usize;

        if col_check >= floor.len() {
            continue;
        }
        for row_off in -1..=1 {
            let row_check = cord.row as i32 + row_off;

            if col_off == 0 && row_off == 0 {
                continue;
            }

            if row_check < 0 {
                continue;
            }

            let row_check = row_check as usize;

            if row_check >= floor[0].len() {
                continue;
            }

            if floor[col_check][row_check] == b'@' {
                adj_rolls += 1;
            }
        }
    }

    adj_rolls < 4
}

pub fn solver() {
    let mut floor: Vec<Vec<u8>> = io::stdin()
        .lines()
        .map(|line| line.unwrap().as_bytes().into_iter().map(|c| *c).collect())
        .collect();

    let mut move_count = 0;

    loop {
        let last_moved = move_count;
        for col in 0..floor.len() {
            for row in 0..floor[0].len() {
                if floor[col][row] == b'@' && can_move(Cords { row, col }, &floor) {
                    move_count += 1;
                    floor[col][row] = b'.';
                }
            }
        }
        if last_moved == move_count {
            break;
        }
    }

    println!("{move_count}");
}
