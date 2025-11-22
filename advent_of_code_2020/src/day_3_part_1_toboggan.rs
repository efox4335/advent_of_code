/*
 * use mod op to loop without needing to dupclate input
*/

use std::io::stdin;

struct Pos {
    row: usize,
    col: usize,
}

pub fn solver() {
    let input: Vec<String> = stdin()
        .lines()
        .map(|s| s.unwrap().replace("\n", ""))
        .collect();

    const ROW_DIF: usize = 1;
    const COL_DIF: usize = 3;

    let mut cur_pos: Pos = Pos { row: 0, col: 0 };
    let mut tree_count = 0;

    let col_len = input[0].chars().count();
    let row_len = input.len();

    while cur_pos.row < row_len {
        if input[cur_pos.row].chars().nth(cur_pos.col).unwrap() != '.' {
            tree_count += 1;
        }

        cur_pos.row += ROW_DIF;
        cur_pos.col += COL_DIF;

        cur_pos.col %= col_len;
    }

    println!("{}", tree_count);
}
