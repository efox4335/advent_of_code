use std::io::stdin;

pub fn solver() {
    let mut line = String::default();
    let _ = stdin().read_line(&mut line);
    line.pop(); //strips newline
    let ear_leave: u32 = line.parse().unwrap();

    line.clear();

    let _ = stdin().read_line(&mut line);

    let bus_id: Vec<u32> = line
        .split([',', 'x', '\n'])
        .filter(|&id| !id.is_empty())
        .map(|id| id.parse().unwrap())
        .collect();

    let mut soonest_bus_id = u32::MAX;
    let mut wait = u32::MAX;
    for id in bus_id {
        let id_mod = ear_leave % id;
        let cur_wait = id - id_mod;

        if cur_wait < wait {
            soonest_bus_id = id;
            wait = cur_wait;
        }
    }

    println!("{}", soonest_bus_id * wait);
}
