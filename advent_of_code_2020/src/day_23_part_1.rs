use std::io;

struct Cups {
    current_index: usize,
    cup_arr: [u8; 9],
}

impl Cups {
    fn new(input: i32) -> Cups {
        let mut cup_arr = [10; 9];
        let mut input = input;

        for val in cup_arr.iter_mut().rev() {
            *val = (input % 10) as u8;
            input /= 10;
        }

        Cups {
            cup_arr,
            current_index: 0,
        }
    }

    fn play_round(&mut self) {
        let current_num = self.cup_arr[self.current_index];

        let mut picked_cups = [10; 3];

        for picked_off in 1..=3 {
            picked_cups[picked_off - 1] = self.cup_arr[(self.current_index + picked_off) % 9];
        }

        let mut target_num = if current_num == 1 { 9 } else { current_num - 1 };

        while picked_cups.contains(&target_num) {
            target_num = if target_num == 1 { 9 } else { target_num - 1 };
        }

        let target_index = self.cup_arr.iter().position(|n| *n == target_num).unwrap();

        for offset in 0..9 {
            let move_index = (self.current_index + 4 + offset) % 9;

            self.cup_arr[(self.current_index + 1 + offset) % 9] = self.cup_arr[move_index];
            if target_index == move_index {
                for moved_offset in 1..=3 {
                    self.cup_arr[(self.current_index + offset + 1 + moved_offset) % 9] =
                        picked_cups[moved_offset - 1];
                }

                break;
            }
        }

        self.current_index = (self.current_index + 1) % 9;
    }

    fn get_output(&self) -> i32 {
        let one_index = self.cup_arr.iter().position(|n| *n == 1).unwrap();

        let mut output = 0;

        for index in 1..9 {
            output += self.cup_arr[(one_index + index) % 9] as i32;
            output *= 10;
        }

        output / 10
    }
}

pub fn solver() {
    let input = io::stdin().lines().next().unwrap().unwrap();

    let input: i32 = input.parse().unwrap();

    let mut state = Cups::new(input);

    for _ in 0..100 {
        state.play_round();
    }

    println!("{}", state.get_output());
}
