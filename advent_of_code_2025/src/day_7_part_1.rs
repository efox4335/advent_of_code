use std::io;

pub fn solver() {
    let mut has_beam = vec![false; 200];

    let mut total_splits = 0;

    for line in io::stdin().lines() {
        let line = line.unwrap();

        for (index, space) in line.as_bytes().iter().enumerate() {
            match space {
                b'S' => has_beam[index] = true,
                b'^' => {
                    if has_beam[index] == true {
                        has_beam[index] = false;

                        total_splits += 1;

                        if index + 1 < line.len() {
                            has_beam[index + 1] = true;
                        }

                        if index > 0 {
                            has_beam[index - 1] = true;
                        }
                    };
                }
                _ => (),
            }
        }
    }

    println!("{total_splits}");
}
