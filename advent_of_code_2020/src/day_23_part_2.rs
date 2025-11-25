use std::io;

const MAX_CUP: i32 = 1000000;
const ROUND_NUM: i32 = 10000000;

struct Cups {
    current_cup: i32,
    cup_arr: Vec<i32>,
}

impl Cups {
    fn new(input: i32) -> Cups {
        let mut cup_arr: Vec<i32> = (1..=MAX_CUP).collect();
        let mut input = input;

        for val in cup_arr[0..9].iter_mut().rev() {
            *val = input % 10;
            input /= 10;
        }

        let mut new_cups = Cups {
            cup_arr: Vec::new(),
            current_cup: cup_arr[0],
        };

        for _ in 0..MAX_CUP {
            new_cups.cup_arr.push(-1);
        }

        for values in cup_arr.windows(2) {
            new_cups.cup_arr[(values[0] - 1) as usize] = values[1] - 1;
        }

        new_cups.cup_arr[(MAX_CUP - 1) as usize] = cup_arr[0] - 1;

        new_cups
    }

    fn remove_selected(&mut self) -> (i32, i32, [i32; 3]) {
        let picked_cups_head = self.cup_arr[(self.current_cup - 1) as usize];

        let mut after_picked = picked_cups_head;

        let mut picked_cups = [0; 3];

        let mut removed_tail = after_picked;

        for index in 0..3 {
            picked_cups[index] = after_picked + 1;

            removed_tail = after_picked;

            after_picked = self.cup_arr[after_picked as usize];
        }

        self.cup_arr[(self.current_cup - 1) as usize] = after_picked;

        (picked_cups_head, removed_tail, picked_cups)
    }

    fn play_round(&mut self) {
        let (removed_cups, removed_tail, picked_cups) = self.remove_selected();

        let mut target_num = if self.current_cup == 1 {
            MAX_CUP
        } else {
            self.current_cup - 1
        };

        while picked_cups.contains(&target_num) {
            target_num = if target_num == 1 {
                MAX_CUP
            } else {
                target_num - 1
            };
        }

        let target_cup = target_num - 1;

        let after_removed = self.cup_arr[target_cup as usize];

        self.cup_arr[target_cup as usize] = removed_cups;

        self.cup_arr[removed_tail as usize] = after_removed;

        self.current_cup = self.cup_arr[(self.current_cup - 1) as usize] + 1;
    }

    fn get_prod(&self) -> i64 {
        let mut prod = 1;

        let mut current_cup = self.cup_arr[0];

        for _ in 0..2 {
            prod *= current_cup as i64 + 1;
            current_cup = self.cup_arr[current_cup as usize];
        }

        prod
    }
}

pub fn solver() {
    let input = io::stdin().lines().next().unwrap().unwrap();

    let input: i32 = input.parse().unwrap();

    let mut state = Cups::new(input);

    for _ in 0..ROUND_NUM {
        state.play_round();
    }

    println!("{}", state.get_prod());
}
