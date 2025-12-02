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
        val *= 10;
        val += mut_num % 10;
        mut_num /= 10;
    }

    val
}

fn is_rep(num: u64) -> bool {
    let digit_count: u64 = num.checked_ilog10().unwrap() as u64 + 1;

    for factor in 1..digit_count {
        //if only i could read
        if digit_count % factor != 0 || factor * 2 != digit_count {
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
        for num in range.lower..=range.upper {
            if is_rep(num) {
                rep_list.push(num);
            }
        }
    }

    println!("{}", rep_list.iter().sum::<u64>());
}
