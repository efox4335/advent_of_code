use std::io::stdin;

fn main() {
    let input: Vec<String> = stdin()
        .lines()
        .map(|s| s.unwrap().replace("\n", ""))
        .collect();
}
