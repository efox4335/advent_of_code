use std::io::stdin;

fn main() {
    let mut line = String::default();
    let _ = stdin().read_line(&mut line);
    line.pop(); //strips newline
    let ear_leave: u32 = line.parse().unwrap();

    line.clear();


    let _ = stdin().read_line(&mut line);

    let bus_id: Vec<u32> = line
        .split([',', 'x', '\n'])
        .filter(|&id| !id.to_string().is_empty())
        .map(|id| id.parse().unwrap())
        .collect();
}
