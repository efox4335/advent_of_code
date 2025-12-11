use std::io::stdin;

#[derive(Debug, Clone)]
struct Matrix {
    col_num: usize,
    row_num: usize,
    mat: Vec<Vec<i64>>,
}

impl Matrix {
    fn new() -> Self {
        Matrix {
            mat: Vec::new(),
            col_num: 0,
            row_num: 0,
        }
    }

    fn new_ident(rank: usize) -> Self {
        let mut new_mat = Self::new();

        new_mat.mat = vec![vec![0; rank]; rank];

        new_mat.row_num = rank;
        new_mat.col_num = rank;

        for diag_index in 0..rank {
            new_mat.mat[diag_index][diag_index] = 1;
        }

        new_mat
    }

    fn append_zero_row(&mut self) {
        self.mat.push(vec![0 as i64; self.col_num]);

        self.row_num += 1;
    }

    fn append_row(&mut self, row: Vec<i64>) {
        if row.len() != self.col_num {
            panic!("should never happen");
        }

        self.mat.push(row);
        self.row_num += 1;
    }

    fn append_col(&mut self, col: Vec<i64>) {
        let mut col = col.clone();

        if col.len() > self.row_num {
            let row_dif = col.len() - self.row_num;

            for _ in 0..row_dif {
                self.append_zero_row();
            }
        }

        if col.len() < self.row_num {
            col.extend(vec![0; self.row_num - col.len()]);
        }

        for row_index in 0..self.row_num {
            self.mat[row_index].push(col[row_index]);
        }

        self.col_num += 1;
    }

    fn transpose(self) -> Self {
        let mut tran_matrix = Self::new();

        for row in self.mat {
            tran_matrix.append_col(row);
        }

        tran_matrix
    }

    fn row_swap(&mut self, row_1_index: usize, row_2_index: usize) {
        if row_1_index == row_2_index {
            return;
        }

        self.mat.swap(row_1_index, row_2_index);
    }

    fn row_sub(&mut self, row_1_index: usize, row_2_index: usize, scale: i64) {
        let sub_row: Vec<i64> = self.mat[row_1_index]
            .iter()
            .map(|val| *val * scale)
            .collect();

        for col_index in 0..self.col_num {
            self.mat[row_2_index][col_index] -= sub_row[col_index];
        }
    }

    fn unimod_row_redux(&mut self) -> Self {
        let mut ele_matrix = Self::new_ident(self.row_num);

        let mut cur_row_index = 0;

        for pivot_col in 0..self.col_num {
            loop {
                let mut is_reduced = true;

                let mut lowest_pivot_index = cur_row_index;
                let mut lowest_pivot = self.mat[cur_row_index][pivot_col];

                for check_row in (cur_row_index + 1)..self.row_num {
                    let cur_pivot = self.mat[check_row][pivot_col];

                    if cur_pivot != 0 {
                        if cur_pivot.abs() < lowest_pivot.abs() || lowest_pivot == 0 {
                            lowest_pivot = cur_pivot;
                            lowest_pivot_index = check_row;
                        }

                        is_reduced = false;
                    }
                }

                if is_reduced {
                    break;
                }

                if lowest_pivot_index != cur_row_index {
                    self.row_swap(lowest_pivot_index, cur_row_index);
                    ele_matrix.row_swap(lowest_pivot_index, cur_row_index);
                }

                for sub_row in (cur_row_index + 1)..self.row_num {
                    let scale = self.mat[sub_row][pivot_col] / lowest_pivot;

                    self.row_sub(cur_row_index, sub_row, scale);
                    ele_matrix.row_sub(cur_row_index, sub_row, scale);
                }
            }

            if self.mat[cur_row_index][pivot_col] != 0 {
                cur_row_index += 1;
            }

            if cur_row_index == self.row_num {
                break;
            }
        }

        ele_matrix
    }

    fn back_sub(&self, end_states: &Matrix) -> Matrix {
        let mut fixed_coefs = Vec::new();

        let mut cur_pivot_col = 0;

        for row_index in 0..self.row_num {
            let cur_pivot = self.mat[row_index][cur_pivot_col];

            if cur_pivot == 0 {
                continue;
            }

            let cur_end_state = end_states.mat[row_index][0];

            let mut fixed_sum = 0;

            for col_index in 0..cur_pivot_col {
                fixed_sum += fixed_coefs[col_index] * self.mat[row_index][col_index];
            }

            fixed_sum = cur_end_state - fixed_sum;

            if fixed_sum % cur_pivot != 0 {
                panic!("this should never happen");
            }

            fixed_coefs.push(fixed_sum / cur_pivot);

            cur_pivot_col += 1;

            if cur_pivot_col == self.col_num {
                break;
            }
        }

        let mut coef_mat = Matrix::new();

        coef_mat.append_col(fixed_coefs);

        coef_mat
    }

    fn matrix_vec_mul(&self, m_vec: &Matrix) -> Matrix {
        let mut sol_vec = vec![0; self.row_num];

        if m_vec.col_num != 1 {
            panic!("this should never happen");
        }

        if m_vec.row_num != self.col_num {
            panic!("should never happen");
        }

        for row_index in 0..self.row_num {
            for col_index in 0..self.col_num {
                sol_vec[row_index] += self.mat[row_index][col_index] * m_vec.mat[col_index][0];
            }
        }

        let mut sol_mat = Matrix::new();
        sol_mat.append_col(sol_vec);
        sol_mat
    }

    fn sol_vec_sum(&self) -> Option<i64> {
        let mut sum = 0;

        for row_index in 0..self.row_num {
            let val = self.mat[row_index][0];

            if val < 0 {
                return None;
            }

            sum += val;
        }

        Some(sum)
    }

    fn are_equal(&self, other: &Self) -> bool {
        if self.row_num != other.row_num || self.col_num != other.col_num {
            return false;
        }

        for row_index in 0..self.row_num {
            for col_index in 0..self.col_num {
                if self.mat[row_index][col_index] != other.mat[row_index][col_index] {
                    return false;
                }
            }
        }

        true
    }
}

#[derive(Debug, Clone)]
struct System {
    end_state: Matrix,
    button_matrix: Matrix,
    solution: Matrix,
}

pub fn solver() {
    let mut systems = Vec::new();

    for line in stdin().lines() {
        let line = line.unwrap();

        let mut line = line.split([' ']).filter(|s| s.len() > 0);

        line.next();

        let mut new_system = System {
            end_state: Matrix::new(),
            button_matrix: Matrix::new(),
            solution: Matrix::new(),
        };

        for part in line {
            let byte_part = part.as_bytes();
            if byte_part[0] == b'(' {
                let mut new_button = Vec::new();

                for wire in part.split([',', '(', ')']).filter(|s| s.len() > 0) {
                    let wire = (wire.as_bytes()[0] - b'0') as usize;

                    new_button.extend(vec![0; wire - new_button.len() + 1]);

                    new_button[wire] = 1;
                }

                let length = new_button.len();

                new_system.button_matrix.append_col(new_button);
                new_system.solution.append_col(vec![0; length]);
            } else {
                let mut joltage_vec = Vec::new();

                for joltage in part.split(['{', '}', ',']).filter(|s| s.len() > 0) {
                    let joltage = joltage.parse::<i64>().unwrap();

                    joltage_vec.push(joltage);
                }

                new_system.end_state.append_col(joltage_vec);
            }
        }

        systems.push(new_system);
    }

    let mut lowest_solutions: Vec<i64> = Vec::new();

    for (sys_num, system) in systems.iter_mut().enumerate() {
        let og_mat = system.button_matrix.clone();

        //println!("normal {:?}", system.button_matrix);

        let tran = system.button_matrix.clone().transpose();
        system.button_matrix = tran;
        //println!("tran {:?}", system.button_matrix);

        let mut ident = system.button_matrix.unimod_row_redux();
        //println!("row redux {:?} \nelem {:?}", system.button_matrix, ident);

        let tran = system.button_matrix.clone().transpose();
        system.button_matrix = tran;
        //println!("row redux tran {:?}", system.button_matrix);
        //println!("end states {:?}", system.end_state);

        let fixed_coef = system.button_matrix.back_sub(&system.end_state);
        //println!("fixed coefficients {:?}", fixed_coef);

        if system.button_matrix.col_num == fixed_coef.row_num {
            let solution = system.button_matrix.matrix_vec_mul(&fixed_coef);
            //        println!("good mat sol {:?}", solution);

            match solution.sol_vec_sum() {
                Some(_) => (),
                None => panic!("store time"),
            }
        }

        let dif = system.button_matrix.col_num - fixed_coef.row_num;

        let range: i64 = 500;

        let mut check_counters = vec![0; dif];

        let mut smallest_solution = None;

        let mut sol_vec = Matrix::new();

        let mut sol_coef_vec = Matrix::new();

        ident = ident.transpose();

        loop {
            let mut pos_sol = fixed_coef.clone();

            for index in 0..dif {
                let row = vec![check_counters[index] - range / 2];

                pos_sol.append_row(row);
            }

            let cur_sol_vec = ident.matrix_vec_mul(&pos_sol);
            let cur_sol = cur_sol_vec.sol_vec_sum();

            match cur_sol {
                Some(cur_val) => match smallest_solution {
                    Some(best_val) => {
                        if best_val > cur_val {
                            //let check_output = og_mat.matrix_vec_mul(&cur_sol_vec);

                            //                            if check_output.are_equal(&system.end_state) {
                            sol_coef_vec = pos_sol;
                            sol_vec = cur_sol_vec;
                            smallest_solution = Some(cur_val);
                            //                          }
                        }
                    }
                    None => {
                        //                    let check_output = og_mat.matrix_vec_mul(&cur_sol_vec);

                        //                        if check_output.are_equal(&system.end_state) {
                        sol_coef_vec = pos_sol;
                        sol_vec = cur_sol_vec;
                        smallest_solution = Some(cur_val);
                        //                      }
                    }
                },
                None => (),
            }

            let mut is_counter_max = true;

            for counter in check_counters.iter() {
                if *counter < range - 1 {
                    is_counter_max = false;
                    break;
                }
            }

            if check_counters.len() > 0 {
                let mut carry = 0;
                let mut cur_check_index = check_counters.len() - 1;

                check_counters[cur_check_index] += 1;

                loop {
                    check_counters[cur_check_index] += carry;

                    carry = check_counters[cur_check_index] / range;

                    check_counters[cur_check_index] %= range;

                    if cur_check_index == 0 {
                        break;
                    }

                    if carry == 0 {
                        break;
                    }

                    cur_check_index -= 1;
                }
            }

            if is_counter_max {
                break;
            }
        }

        let check_output = og_mat.matrix_vec_mul(&sol_vec);

        if !check_output.are_equal(&system.end_state) {
            println!("{}", sys_num + 1);
            println!("output {:?}", check_output);
            println!("buttons {:?}", og_mat);
            println!("fixed coef {:?}", fixed_coef);
            println!("sol coef {:?}", sol_coef_vec);

            println!("{:?}", system);
            println!("tran ele {:?}", ident);

            println!("sol vec {:?}", sol_vec);

            panic!("this one here");
        }

        println!("{:?}", sol_vec);

        match smallest_solution {
            Some(val) => lowest_solutions.push(val),
            None => panic!("should never happen"),
        }
    }

    println!("{}", lowest_solutions.iter().sum::<i64>());
}
