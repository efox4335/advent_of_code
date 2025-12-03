use std::collections::BinaryHeap;
use std::io;

const ON_LEN: usize = 12;

struct DigitPlace {
    digit: u8,
    place: usize,
}

impl Ord for DigitPlace {
    fn cmp(&self, other: &Self) -> std::cmp::Ordering {
        self.digit
            .cmp(&other.digit)
            //place cmp needed so it will always choose the first big number
            .then(other.place.cmp(&self.place))
    }
}

impl PartialOrd for DigitPlace {
    fn partial_cmp(&self, other: &Self) -> Option<std::cmp::Ordering> {
        Some(self.cmp(other))
    }
}

impl PartialEq for DigitPlace {
    fn eq(&self, other: &Self) -> bool {
        self.digit == other.digit && self.place == other.place
    }
}

impl Eq for DigitPlace {}

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

    let mut largest_jolts: Vec<u64> = Vec::new();

    for bank in input {
        let mut on_batts: Vec<u8> = Vec::new();

        let mut batt_heap: BinaryHeap<DigitPlace> = BinaryHeap::new();

        let mut place_num = ON_LEN;

        let mut last_added_index: Option<usize> = None;

        for (batt_num, batt) in bank.iter().enumerate() {
            batt_heap.push(DigitPlace {
                digit: *batt,
                place: batt_num,
            });

            if batt_num + place_num == bank.len() {
                loop {
                    let pot_top = batt_heap.pop().unwrap();

                    if let Some(last_index) = last_added_index {
                        if pot_top.place > last_index {
                            on_batts.push(pot_top.digit);
                            last_added_index = Some(pot_top.place);
                            place_num -= 1;
                            break;
                        }
                    } else {
                        on_batts.push(pot_top.digit);
                        last_added_index = Some(pot_top.place);
                        place_num -= 1;
                        break;
                    }
                }
            }
        }

        largest_jolts.push(on_batts.iter().fold(0, |acc, batt| acc * 10 + *batt as u64));
    }

    for (ind, num) in largest_jolts.iter().enumerate() {
        println!("{num} {}", ind + 1);
    }

    println!("{}", largest_jolts.iter().sum::<u64>());
}
