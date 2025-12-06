use std::io;

enum Operation {
    Add,
    Mul,
}

struct Problem {
    numbers: Vec<u64>,
    op: Option<Operation>,
}

impl Problem {
    fn new() -> Self {
        Problem {
            numbers: Vec::new(),
            op: None,
        }
    }

    fn add_number(&mut self, num: u64) {
        self.numbers.push(num);
    }

    fn add_op(&mut self, op: Operation) {
        self.op = Some(op);
    }
}

pub fn solver() {
    let input: Vec<Vec<u8>> = io::stdin()
        .lines()
        .map(|line| line.unwrap().as_bytes().to_vec())
        .collect();

    let mut problems: Vec<Problem> = Vec::new();
    let mut problems_index = 0;

    for col_index in 0..input[0].len() {
        if input[input.len() - 1][col_index] != b' ' {
            problems.push(Problem::new());
            problems_index = problems.len() - 1;

            match input[input.len() - 1][col_index] {
                b'+' => problems[problems_index].add_op(Operation::Add),
                b'*' => problems[problems_index].add_op(Operation::Mul),
                c => panic!("char {c} should not be here"),
            }
        }

        let mut new_num = 0;

        for row_index in 0..input.len() - 1 {
            let cur_char = input[row_index][col_index];

            if new_num > 0 && cur_char == b' ' {
                break;
            }

            new_num *= 10;

            new_num += if cur_char != b' ' {
                cur_char as u64 - b'0' as u64
            } else {
                0
            };
        }

        if new_num == 0 {
            continue;
        }

        problems[problems_index].add_number(new_num);
    }

    let mut total_sum = 0;

    for problem in problems {
        match problem.op {
            Some(Operation::Add) => {
                for num in problem.numbers {
                    total_sum += num;
                }
            }
            Some(Operation::Mul) => {
                let mut solution = 1;

                for num in problem.numbers {
                    solution *= num;
                }

                total_sum += solution;
            }
            None => panic!("should never happen"),
        }
    }

    println!("{total_sum}");
}
