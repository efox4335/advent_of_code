/*
 * use mod op to loop without needing to dupclate input
 * add a loop with all slopes
*/

use std::io::stdin;

struct Pos {
    row: usize,
    col: usize,
}

struct Slope {
    row_dif: usize,
    col_dif: usize,
}

pub fn solver() {
    let input: Vec<String> = stdin()
        .lines()
        .map(|s| s.unwrap().replace("\n", ""))
        .collect();

    let slopes: [Slope; 5] = [
        Slope {
            row_dif: 1,
            col_dif: 1,
        },
        Slope {
            row_dif: 1,
            col_dif: 3,
        },
        Slope {
            row_dif: 1,
            col_dif: 5,
        },
        Slope {
            row_dif: 1,
            col_dif: 7,
        },
        Slope {
            row_dif: 2,
            col_dif: 1,
        },
    ];

    let mut tree_prod = 1;

    for slope in slopes {
        let row_dif: usize = slope.row_dif;
        let col_dif: usize = slope.col_dif;

        let mut cur_pos: Pos = Pos { row: 0, col: 0 };
        let mut tree_count = 0;

        let col_len = input[0].chars().count();
        let row_len = input.len();

        while cur_pos.row < row_len {
            if input[cur_pos.row].chars().nth(cur_pos.col).unwrap() != '.' {
                tree_count += 1;
            }

            cur_pos.row += row_dif;
            cur_pos.col += col_dif;

            cur_pos.col %= col_len;
        }

        tree_prod *= tree_count;
    }

    println!("{}", tree_prod);
}
