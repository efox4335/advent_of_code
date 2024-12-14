use std::io::stdin;

struct BusTimes {
    id: u64,
    arival_time: u64,
}

fn main() {
    let mut line = String::default();
    let _ = stdin().read_line(&mut line);
    line.clear();

    let _ = stdin().read_line(&mut line);

    let mut bus_vec: Vec<BusTimes> = vec![];
    for (num, id) in line
        .split([',', '\n'])
        .filter(|s| !s.is_empty())
        .enumerate()
    {
        if !id.is_empty() && id.as_bytes()[0] != b'x' {
            bus_vec.push(BusTimes {
                id: id.parse().unwrap(),
                arival_time: num as u64,
            });
        }
    }
}
