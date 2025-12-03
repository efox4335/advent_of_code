use std::io;

pub fn solver() {
    let input: Vec<Vec<u8>> = io::stdin()
        .lines()
        .map(|line| {
            line.unwrap()
                .as_bytes()
                .iter()
                .map(|number| number - 48)
                .collect()
        })
        .collect();

    let mut largest_jolts: Vec<u32> = Vec::new();

    for bank in input {
        let mut first_batt = 0;
        let mut second_batt = 0;

        for (batt_num, batt) in bank.iter().enumerate() {
            if *batt as u32 > first_batt && batt_num != bank.len() - 1 {
                first_batt = *batt as u32;
                second_batt = 0;
                continue;
            }

            if *batt as u32 > second_batt {
                second_batt = *batt as u32;
            }
        }

        largest_jolts.push(first_batt * 10 + second_batt);
    }

    println!("{}", largest_jolts.iter().sum::<u32>());
}
