use std::io::stdin;

#[derive(Debug)]
struct Point {
    x: i64,
    y: i64,
}

impl Point {
    fn rect_area(&self, other: &Self) -> i64 {
        ((self.x - other.x).abs() + 1) * ((self.y - other.y).abs() + 1)
    }
}

pub fn solver() {
    let mut points = Vec::new();

    for line in stdin().lines() {
        let line = line.unwrap();

        let mut point_iter = line.split([',', '\n']);

        let new_x = point_iter.next().unwrap().parse().unwrap();
        let new_y = point_iter.next().unwrap().parse().unwrap();

        points.push(Point { x: new_x, y: new_y });
    }

    let mut largest_area = 0;

    for point_1 in 0..(points.len() - 1) {
        for point_2 in (point_1 + 1)..points.len() {
            let cur_area = points[point_1].rect_area(&points[point_2]);

            if cur_area > largest_area {
                largest_area = cur_area;
            }
        }
    }

    println!("{largest_area}");
}
