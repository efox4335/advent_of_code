/*
 * convert each passport into a single string then run .find on each passport
*/

use std::io::stdin;

fn main() {
    let mut passports: Vec<String> = vec!["".to_string()];

    for line in stdin().lines().map(|s| s.unwrap().replace("\n", "")) {
        if line.is_empty() {
            passports.push("".to_string());
        } else {
            passports.last_mut().unwrap().push_str(&line);
            passports.last_mut().unwrap().push_str(" ");
        }
    }
}
