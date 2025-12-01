use std::env;

fn main() {
    let args: Vec<String> = env::args().collect();

    if args.len() != 2 {
        panic!("part and day required");
    }

    match args[1].as_str() {
        _ => panic!("unknown day/part"),
    }
}
