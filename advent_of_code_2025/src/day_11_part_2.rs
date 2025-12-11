use std::collections::HashMap;
use std::io::stdin;

struct Device {
    connections: Vec<usize>,
}

struct Rack {
    devices: Vec<Device>,
    name_to_index: HashMap<String, usize>,
}

impl Rack {
    fn find_or_insert(&mut self, dev: String) -> usize {
        match self.name_to_index.get(&dev) {
            Some(index) => *index,
            None => {
                let ret_index = self.devices.len();

                self.devices.push(Device {
                    connections: Vec::new(),
                });
                self.name_to_index.insert(dev, ret_index);

                ret_index
            }
        }
    }

    fn connect(&mut self, parent: String, child: String) {
        let parent_index = self.find_or_insert(parent);
        let child_index = self.find_or_insert(child);

        self.devices[parent_index].connections.push(child_index);
    }

    fn new() -> Self {
        Rack {
            devices: Vec::new(),
            name_to_index: HashMap::new(),
        }
    }

    fn path_num(&self, cur: usize, end: usize, memo: &mut HashMap<usize, i64>) -> i64 {
        if let Some(val) = memo.get(&cur) {
            return *val;
        }

        if cur == end {
            memo.insert(cur, 1);

            return 1;
        }

        let mut cur_paths = 0;

        for child in self.devices[cur].connections.iter() {
            cur_paths += self.path_num(*child, end, memo);
        }

        memo.insert(cur, cur_paths);

        cur_paths
    }

    fn get_path_num(&self, st: String, end: String) -> i64 {
        let st_index = self.name_to_index.get(&st).unwrap();
        let end_index = self.name_to_index.get(&end).unwrap();

        let mut memo = HashMap::new();

        self.path_num(*st_index, *end_index, &mut memo)
    }
}

pub fn solver() {
    let mut rack = Rack::new();

    for line in stdin().lines() {
        let line = line.unwrap();

        let mut device_names = line.split([':', ' ', '\n']).filter(|s| s.len() > 0);

        let parent_name = device_names.next().unwrap().to_owned();

        for child_name in device_names {
            let child_name = child_name.to_owned();

            rack.connect(parent_name.to_string(), child_name);
        }
    }

    let svr_to_fft = rack.get_path_num("svr".to_string(), "fft".to_string());
    let fft_to_dac = rack.get_path_num("fft".to_string(), "dac".to_string());
    let dac_to_out = rack.get_path_num("dac".to_string(), "out".to_string());

    let svr_to_dac = rack.get_path_num("svr".to_string(), "dac".to_string());
    let dac_to_fft = rack.get_path_num("dac".to_string(), "fft".to_string());
    let fft_to_out = rack.get_path_num("fft".to_string(), "out".to_string());

    let total = (svr_to_fft * fft_to_dac * dac_to_out) + (svr_to_dac * dac_to_fft * fft_to_out);

    println!("{}", total);
}
