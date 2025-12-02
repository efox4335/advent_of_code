/*
 * algorithm for the number of numbers with repeting digits with n digits
 * sort all factors of n in ascending order and remove duplicates and n
 *
 * factors are important because if k is not a factor of n than any pattern that is k digits long will get cut off and won't count
 *
 * n does not count because the pattern must repeat at least once
 *
 * duplicate factors are removed because each pattern should only be counted once
 *
 * if k is the list of factors of n and jn are all numbers that are in the list that are factors kn
 * the total is equal to the sum of 10 ^ kn minus the sum of 10 ^ jn for all jn for all k
 *
 * the reason for the subtraction is that if j is a factor of k than 10 ^ k will include patterns that are j digits long and are already counted
 *
 * this could easily be adapted for numbers that are n digits and less than y
 * also for numbers with n digits that are greater than y
*/
use std::io;

#[derive(Debug)]
struct Range {
    lower: u64,
    upper: u64,
}

fn get_ending_digits(num: u64, digit_num: u64) -> u64 {
    let mut val = 0;
    let mut mut_num = num;

    for _ in 0..digit_num {
        val += mut_num % 10;
        mut_num /= 10;
        val *= 10;
    }

    val
}

fn is_rep(num: u64) -> bool {
    let digit_count: u64 = num.checked_ilog10().unwrap() as u64 + 1;

    for factor in 1..digit_count {
        if digit_count % factor != 0 {
            continue;
        }

        let mut mut_num = num;
        let mut last_start = get_ending_digits(mut_num, factor);
        mut_num /= 10_u64.pow(factor.try_into().unwrap());

        for _ in 1..(digit_count / factor) {
            let current_start = get_ending_digits(mut_num, factor);

            if last_start != current_start {
                break;
            }

            mut_num /= 10_u64.pow(factor.try_into().unwrap());

            if mut_num == 0 {
                return true;
            }

            last_start = current_start;
        }
    }

    false
}

fn get_starting_digits(num: u64, digit_num: u64) -> u64 {
    let digit_count = num.checked_ilog10().unwrap() as u64 + 1;

    num / 10_u64.pow(digit_count as u32 - digit_num as u32)
}

fn seg_to_full(seg: u64, seg_len: u64, rep_count: u64) -> u64 {
    let mut full_num = 0;
    let shift_amount = 10_u64.pow(seg_len as u32);

    for _ in 0..rep_count {
        full_num *= shift_amount;
        full_num += seg;
    }

    full_num
}

fn list_rep(range: &Range, rep_list: &mut Vec<u64>) {
    let lower_digit_count: u64 = range.lower.checked_ilog10().unwrap() as u64 + 1;
    let upper_digit_count: u64 = range.upper.checked_ilog10().unwrap() as u64 + 1;

    for digit_count in lower_digit_count..=upper_digit_count {
        for factor in 1..digit_count {
            if digit_count % factor != 0 {
                continue;
            }

            let lower_bound = if digit_count == lower_digit_count {
                get_starting_digits(range.lower, factor)
            } else {
                10_u64.pow(factor as u32 - 1)
            };

            let upper_bound = if digit_count == upper_digit_count {
                get_starting_digits(range.upper, factor)
            } else {
                10_u64.pow(factor as u32) - 1
            };

            for num in lower_bound..=upper_bound {
                if is_rep(num) {
                    continue;
                }

                let full_num = seg_to_full(num, factor, digit_count / factor);

                if full_num >= range.lower && full_num <= range.upper {
                    rep_list.push(full_num);
                }
            }
        }
    }
}

pub fn solver() {
    let ranges: Vec<Range> = io::stdin()
        .lines()
        .next()
        .unwrap()
        .unwrap()
        .split([',', '\n'])
        .map(|range| {
            let mut nums = range.split('-');

            Range {
                lower: nums.next().unwrap().parse().unwrap(),
                upper: nums.next().unwrap().parse().unwrap(),
            }
        })
        .collect();

    let mut rep_list: Vec<u64> = Vec::new();

    for range in ranges {
        list_rep(&range, &mut rep_list);
    }

    println!("{}", rep_list.iter().sum::<u64>());
}
