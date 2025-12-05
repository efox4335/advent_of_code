use std::{collections::BTreeSet, io};

#[derive(Clone, Copy, Debug)]
struct Range {
    lower: u64,
    upper: u64,
}

#[derive(Debug)]
enum RangeKey {
    Range(Range),
    Food(u64),
}

impl Ord for RangeKey {
    fn cmp(&self, other: &Self) -> std::cmp::Ordering {
        match self {
            RangeKey::Range(range) => match other {
                RangeKey::Range(other_range) => {
                    if range.upper < other_range.lower {
                        return std::cmp::Ordering::Less;
                    }

                    if range.lower > other_range.upper {
                        return std::cmp::Ordering::Greater;
                    }

                    return std::cmp::Ordering::Equal;
                }
                RangeKey::Food(other_food_id) => {
                    let other_food_id = *other_food_id;

                    if other_food_id > range.upper {
                        return std::cmp::Ordering::Less;
                    }

                    if other_food_id < range.lower {
                        return std::cmp::Ordering::Greater;
                    }

                    return std::cmp::Ordering::Equal;
                }
            },
            RangeKey::Food(food_id) => {
                let food_id = *food_id;
                match other {
                    RangeKey::Range(other_range) => {
                        if food_id > other_range.upper {
                            return std::cmp::Ordering::Greater;
                        }

                        if food_id < other_range.lower {
                            return std::cmp::Ordering::Less;
                        }

                        return std::cmp::Ordering::Equal;
                    }
                    RangeKey::Food(other_food_id) => {
                        let other_food_id = *other_food_id;
                        panic!("this should never happen {food_id} {other_food_id}");
                    }
                }
            }
        }
    }
}

impl PartialOrd for RangeKey {
    fn partial_cmp(&self, other: &Self) -> Option<std::cmp::Ordering> {
        Some(self.cmp(other))
    }
}

impl PartialEq for RangeKey {
    fn eq(&self, other: &Self) -> bool {
        match self {
            RangeKey::Range(range) => match other {
                RangeKey::Range(other_range) => {
                    if range.upper < other_range.lower || range.lower > other_range.upper {
                        return false;
                    }

                    return true;
                }
                RangeKey::Food(other_food_id) => {
                    let other_food_id = *other_food_id;

                    if other_food_id > range.upper || other_food_id < range.lower {
                        return false;
                    }

                    return true;
                }
            },
            RangeKey::Food(food_id) => {
                let food_id = *food_id;
                match other {
                    RangeKey::Range(other_range) => {
                        if food_id > other_range.upper || food_id < other_range.lower {
                            return false;
                        }

                        return true;
                    }
                    RangeKey::Food(other_food_id) => {
                        let other_food_id = *other_food_id;
                        panic!("this should never happen {food_id} {other_food_id}");
                    }
                }
            }
        }
    }
}

impl Range {
    fn merge(&self, other: &Self) -> Range {
        let lower = if self.lower > other.lower {
            other.lower
        } else {
            self.lower
        };
        let upper = if self.upper < other.upper {
            other.upper
        } else {
            self.upper
        };

        Range { lower, upper }
    }
}

impl Eq for RangeKey {}

pub fn solver() {
    let mut ranges: BTreeSet<RangeKey> = BTreeSet::new();

    for line in io::stdin().lines() {
        let line = line.unwrap();

        if line.len() == 0 {
            break;
        }

        let mut range_vals = line.split(['-', '\n']);

        let lower: u64 = range_vals.next().unwrap().parse().unwrap();
        let upper: u64 = range_vals.next().unwrap().parse().unwrap();

        let mut range = Range { lower, upper };

        loop {
            match ranges.take(&RangeKey::Range(range)) {
                Some(pot_range) => match pot_range {
                    RangeKey::Range(merge_range) => {
                        range = merge_range.merge(&range);
                    }
                    RangeKey::Food(food_id) => {
                        panic!("this should never happen {food_id}");
                    }
                },
                None => {
                    ranges.insert(RangeKey::Range(range));

                    break;
                }
            }
        }
    }

    let fresh_id_amount = ranges.iter().fold(0, |acc, range| match range {
        RangeKey::Range(range) => {
            return acc + range.upper - range.lower + 1;
        }
        _ => panic!("should never happen"),
    });

    println!("{fresh_id_amount}");
}
