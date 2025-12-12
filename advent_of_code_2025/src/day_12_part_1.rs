use std::io::stdin;

#[derive(Debug)]
struct Shape {
    shape: Vec<Vec<u8>>,
    used_squares: u32,
}

impl Shape {
    fn new() -> Self {
        Shape {
            shape: Vec::new(),
            used_squares: 0,
        }
    }
}

#[derive(Debug)]
struct Region {
    req_shapes: Vec<u32>,
    col_num: usize,
    row_num: usize,
}

enum Part {
    Shapes,
    Regions,
}

pub fn solver() {
    let mut cur_part = Part::Shapes;

    let mut shapes: Vec<Shape> = Vec::new();
    let mut regions: Vec<Region> = Vec::new();

    let mut cur_shape = Shape::new();

    for line in stdin().lines() {
        let line = line.unwrap();

        if line.contains('x') {
            cur_part = Part::Regions;
        }

        match cur_part {
            Part::Shapes => {
                if line.len() == 0 {
                    shapes.push(cur_shape);

                    cur_shape = Shape::new();

                    continue;
                }

                let line = line.as_bytes();

                if line[0] != b'.' && line[0] != b'#' {
                    continue;
                }

                let mut shape_line = Vec::new();
                let mut taken_count = 0;

                for tile in line {
                    if *tile != b'.' && *tile != b'#' {
                        break;
                    }

                    if *tile == b'#' {
                        taken_count += 1;
                    }

                    shape_line.push(*tile);
                }

                cur_shape.shape.push(shape_line);
                cur_shape.used_squares += taken_count;
            }
            Part::Regions => {
                let mut line_split = line.split(['x', ':']);

                let col_num = line_split.next().unwrap().parse::<usize>().unwrap();
                let row_num = line_split.next().unwrap().parse::<usize>().unwrap();

                let pres_count = line_split
                    .next()
                    .unwrap()
                    .split(' ')
                    .filter(|s| s.len() > 0);

                let mut pres_vec = Vec::new();

                for pres in pres_count {
                    pres_vec.push(pres.parse::<u32>().unwrap());
                }

                regions.push(Region {
                    req_shapes: pres_vec,
                    col_num,
                    row_num,
                });
            }
        }
    }

    let mut good_regions = 0;

    for reg in regions {
        let size = reg.col_num * reg.row_num;

        let mut full_tiles = 0;

        for amount in reg.req_shapes.iter() {
            full_tiles += 9 * *amount as usize;
        }

        let mut tiles = 0;

        for (ind, amount) in reg.req_shapes.iter().enumerate() {
            tiles += shapes[ind].used_squares as usize * *amount as usize;
        }

        if full_tiles > size && tiles < size {
            panic!("real alg needed");
        }

        if tiles > size {
            continue;
        }

        good_regions += 1;
    }

    println!("{good_regions}");
}
