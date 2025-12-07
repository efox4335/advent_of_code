use std::io;

pub fn solver() {
    let mut total_paths: Vec<u64> = vec![0; 200];

    for line in io::stdin().lines() {
        let line = line.unwrap();

        for (index, space) in line.as_bytes().iter().enumerate() {
            match space {
                b'S' => total_paths[index] = 1,
                b'^' => {
                    total_paths[index - 1] += total_paths[index];
                    total_paths[index + 1] += total_paths[index];

                    total_paths[index] = 0;
                }
                _ => (),
            }
        }
    }

    println!("{}", total_paths.iter().sum::<u64>());
}
