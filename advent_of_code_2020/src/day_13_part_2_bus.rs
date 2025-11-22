//no euclid algo needed as the numbers are small

use std::io::stdin;

#[derive(Clone, Copy)]
struct BusTimes {
    id: u64,
    arival_time: u64,
}

pub fn solver() {
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
            let id_val = id.parse::<u64>().unwrap();
            bus_vec.push(BusTimes {
                id: id_val,
                arival_time: (id_val - (num as u64 % id_val)) % id_val,
            });
        }
    }

    let mut prod = 1;
    for i in bus_vec.iter() {
        prod *= i.id;
    }

    let mut time: u128 = 0;
    for id in bus_vec.iter() {
        let base_val = prod / id.id;
        let mut add_val = base_val;

        while add_val % id.id != id.arival_time {
            add_val %= prod;
            add_val += base_val;
        }

        time += add_val as u128;
        time %= prod as u128;
    }
    println!("{}", time % prod as u128);
}
