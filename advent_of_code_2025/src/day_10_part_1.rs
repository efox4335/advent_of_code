use std::{collections::HashMap, io::stdin};

#[derive(Debug, Clone)]
struct System {
    end_state: i64,
    buttons: Vec<i64>,
    solution: Vec<bool>,
}

//because xor is associative and commutative each button can only be pushed max once
fn get_lowest(
    system: System,
    on_count: i64,
    memo: &mut HashMap<Vec<bool>, Option<i64>>,
) -> Option<i64> {
    if let Some(presses) = memo.get(&system.solution) {
        return *presses;
    }

    let mut xor_sum = 0;

    for (button, state) in system.solution.iter().enumerate() {
        if !*state {
            continue;
        }

        xor_sum ^= system.buttons[button];
    }

    if xor_sum == system.end_state {
        memo.insert(system.solution, Some(on_count));

        return Some(on_count);
    }

    let mut lowest_presses = None;

    for (button, state) in system.solution.iter().enumerate() {
        if *state {
            continue;
        }

        let mut next_system = system.clone();

        next_system.solution[button] = true;

        let presses = get_lowest(next_system, on_count + 1, memo);

        let presses = match presses {
            Some(val) => val,
            None => continue,
        };

        lowest_presses = match lowest_presses {
            Some(val) => {
                if val > presses {
                    Some(presses)
                } else {
                    Some(val)
                }
            }
            None => Some(presses),
        };
    }

    memo.insert(system.solution, lowest_presses);

    lowest_presses
}

pub fn solver() {
    let mut systems = Vec::new();

    for line in stdin().lines() {
        let line = line.unwrap();

        let mut line = line
            .split(['[', ']', '(', ')', ' '])
            .filter(|s| s.len() > 0);

        let mut new_system = System {
            end_state: 0,
            buttons: Vec::new(),
            solution: Vec::new(),
        };

        for (placement, light) in line.next().unwrap().as_bytes().iter().enumerate() {
            if *light != b'#' {
                continue;
            }

            new_system.end_state |= 1 << placement;
        }

        for button in line {
            let mut new_button = 0;

            for wire in button.split(',') {
                let wire = wire.as_bytes()[0];

                if wire == b'{' {
                    break;
                }

                let shift_num = wire - b'0';

                new_button |= 1 << shift_num;
            }

            new_system.buttons.push(new_button);
            new_system.solution.push(false);
        }

        systems.push(new_system);
    }

    let mut lowest_solutions = Vec::new();

    for system in systems {
        lowest_solutions.push(get_lowest(system, 0, &mut HashMap::new()).unwrap());
    }

    println!("{}", lowest_solutions.iter().sum::<i64>());
}
