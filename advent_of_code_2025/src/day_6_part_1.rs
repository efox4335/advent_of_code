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
    let mut problems: Vec<Problem> = Vec::new();

    for line in io::stdin().lines() {
        let line = line.unwrap();

        for (ind, segment) in line.split_whitespace().enumerate() {
            if ind >= problems.len() {
                problems.push(Problem::new());
            }

            if let Ok(num) = segment.parse::<u64>() {
                problems[ind].add_number(num);
            } else {
                let segment = segment.as_bytes();

                match segment[0] {
                    b'+' => problems[ind].add_op(Operation::Add),
                    b'*' => problems[ind].add_op(Operation::Mul),
                    c => panic!("char {c} should never appear"),
                }
            }
        }
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
