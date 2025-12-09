use std::io::stdin;

#[derive(Clone, Copy)]
enum RelDir {
    Left,
    Right,
}

enum CarDir {
    North,
    South,
    East,
    West,
}

#[derive(Debug, Clone, Copy)]
struct Point {
    x: i64,
    y: i64,
}

impl Point {
    fn rect_area(&self, other: &Self) -> i64 {
        ((self.x - other.x).abs() + 1) * ((self.y - other.y).abs() + 1)
    }

    //with self as the start
    fn get_line_dir(&self, other: &Self) -> CarDir {
        if self.y == other.y {
            if self.x < other.x {
                CarDir::East
            } else {
                CarDir::West
            }
        } else {
            if self.y < other.y {
                CarDir::North
            } else {
                CarDir::South
            }
        }
    }
}

struct Polygon {
    points: Vec<Point>,
    winding_order: Option<RelDir>,
    outer_points: Option<Vec<Point>>,
}

impl Polygon {
    fn con_rect(p1: Point, p2: Point) -> Self {
        Polygon {
            points: vec![
                p1,
                Point { x: p1.x, y: p2.y },
                p2,
                Point { x: p2.x, y: p1.y },
            ],
            winding_order: None,
            outer_points: None,
        }
    }

    fn rect_in_poly(&self, rect: &Self) -> bool {
        let outer_points = self.outer_points.as_ref().expect("calc outside first");

        for rect_index in 0..rect.points.len() {
            let rect_st = &rect.points[rect_index];
            let rect_end = &rect.points[(rect_index + 1) % rect.points.len()];

            let (lower_rect_x, upper_rect_x) = if rect_st.x < rect_end.x {
                (rect_st.x, rect_end.x)
            } else {
                (rect_end.x, rect_st.x)
            };

            let (lower_rect_y, upper_rect_y) = if rect_st.y < rect_end.y {
                (rect_st.y, rect_end.y)
            } else {
                (rect_end.y, rect_st.y)
            };

            for outer_index in 0..outer_points.len() {
                let cur_outer_st = outer_points[outer_index];
                let cur_outer_end = outer_points[(outer_index + 1) % outer_points.len()];

                let (lower_outer_x, upper_outer_x) = if cur_outer_st.x < cur_outer_end.x {
                    (cur_outer_st.x, cur_outer_end.x)
                } else {
                    (cur_outer_end.x, cur_outer_st.x)
                };

                let (lower_outer_y, upper_outer_y) = if cur_outer_st.y < cur_outer_end.y {
                    (cur_outer_st.y, cur_outer_end.y)
                } else {
                    (cur_outer_end.y, cur_outer_st.y)
                };

                if upper_rect_x < lower_outer_x || lower_rect_x > upper_outer_x {
                    continue;
                }

                if upper_rect_y < lower_outer_y || lower_rect_y > upper_outer_y {
                    continue;
                }

                return false;
            }
        }

        true
    }

    fn calc_min_outside(&mut self) {
        let winding_order = self.winding_order.expect("calculate winding order");

        let mut last_st = self.points.last().unwrap();

        let mut outside = Vec::new();

        for index in 0..self.points.len() {
            let cur_st = &self.points[index];
            let cur_end = &self.points[(index + 1) % self.points.len()];

            let last_dir = last_st.get_line_dir(cur_st);
            let cur_dir = cur_st.get_line_dir(cur_end);

            match last_dir {
                CarDir::North => match cur_dir {
                    CarDir::East => match winding_order {
                        RelDir::Right => outside.push(Point {
                            x: cur_st.x - 1,
                            y: cur_st.y + 1,
                        }),
                        RelDir::Left => outside.push(Point {
                            x: cur_st.x + 1,
                            y: cur_st.y - 1,
                        }),
                    },
                    CarDir::West => match winding_order {
                        RelDir::Right => outside.push(Point {
                            x: cur_st.x - 1,
                            y: cur_st.y - 1,
                        }),
                        RelDir::Left => outside.push(Point {
                            x: cur_st.x + 1,
                            y: cur_st.y + 1,
                        }),
                    },
                    _ => panic!("should never happen"),
                },
                CarDir::South => match cur_dir {
                    CarDir::West => match winding_order {
                        RelDir::Right => outside.push(Point {
                            x: cur_st.x + 1,
                            y: cur_st.y - 1,
                        }),
                        RelDir::Left => outside.push(Point {
                            x: cur_st.x - 1,
                            y: cur_st.y + 1,
                        }),
                    },
                    CarDir::East => match winding_order {
                        RelDir::Right => outside.push(Point {
                            x: cur_st.x + 1,
                            y: cur_st.y + 1,
                        }),
                        RelDir::Left => outside.push(Point {
                            x: cur_st.x - 1,
                            y: cur_st.y - 1,
                        }),
                    },
                    _ => panic!(),
                },
                CarDir::East => match cur_dir {
                    CarDir::South => match winding_order {
                        RelDir::Right => outside.push(Point {
                            x: cur_st.x + 1,
                            y: cur_st.y + 1,
                        }),
                        RelDir::Left => outside.push(Point {
                            x: cur_st.x - 1,
                            y: cur_st.y - 1,
                        }),
                    },
                    CarDir::North => match winding_order {
                        RelDir::Right => outside.push(Point {
                            x: cur_st.x - 1,
                            y: cur_st.y + 1,
                        }),
                        RelDir::Left => outside.push(Point {
                            x: cur_st.x + 1,
                            y: cur_st.y - 1,
                        }),
                    },
                    _ => panic!(),
                },
                CarDir::West => match cur_dir {
                    CarDir::North => match winding_order {
                        RelDir::Right => outside.push(Point {
                            x: cur_st.x - 1,
                            y: cur_st.y - 1,
                        }),
                        RelDir::Left => outside.push(Point {
                            x: cur_st.x + 1,
                            y: cur_st.y + 1,
                        }),
                    },
                    CarDir::South => match winding_order {
                        RelDir::Right => outside.push(Point {
                            x: cur_st.x + 1,
                            y: cur_st.y - 1,
                        }),
                        RelDir::Left => outside.push(Point {
                            x: cur_st.x - 1,
                            y: cur_st.y + 1,
                        }),
                    },
                    _ => panic!(),
                },
            }

            last_st = cur_st;
        }

        self.outer_points = Some(outside);
    }

    fn calc_winding_order(&mut self) {
        let mut last_dir: Option<CarDir> = None;

        let mut right_minus_left_turn_sum = 0;

        for index in 0..self.points.len() {
            let st_point = &self.points[index];
            let end_point = &self.points[(index + 1) % self.points.len()];

            let cur_dir = st_point.get_line_dir(end_point);

            if let Some(last_dir) = last_dir.as_ref() {
                match last_dir {
                    CarDir::North => match cur_dir {
                        CarDir::East => right_minus_left_turn_sum += 1,
                        CarDir::West => right_minus_left_turn_sum -= 1,
                        _ => panic!("should never happen"),
                    },
                    CarDir::South => match cur_dir {
                        CarDir::West => right_minus_left_turn_sum += 1,
                        CarDir::East => right_minus_left_turn_sum -= 1,
                        _ => panic!("should never happen"),
                    },
                    CarDir::East => match cur_dir {
                        CarDir::South => right_minus_left_turn_sum += 1,
                        CarDir::North => right_minus_left_turn_sum -= 1,
                        _ => panic!("should never happen"),
                    },
                    CarDir::West => match cur_dir {
                        CarDir::North => right_minus_left_turn_sum += 1,
                        CarDir::South => right_minus_left_turn_sum -= 1,
                        _ => panic!("should never happen"),
                    },
                }
            }

            last_dir = Some(cur_dir);
        }

        if right_minus_left_turn_sum > 0 {
            self.winding_order = Some(RelDir::Right);
        } else {
            self.winding_order = Some(RelDir::Left);
        }
    }
}

pub fn solver() {
    let mut poly = Polygon {
        points: Vec::new(),
        winding_order: None,
        outer_points: None,
    };

    let mut points = Vec::new();

    for line in stdin().lines() {
        let line = line.unwrap();

        let mut point_iter = line.split([',', '\n']);

        let new_x = point_iter.next().unwrap().parse().unwrap();
        let new_y = point_iter.next().unwrap().parse().unwrap();

        poly.points.push(Point { x: new_x, y: new_y });
        points.push(Point { x: new_x, y: new_y });
    }

    poly.calc_winding_order();
    poly.calc_min_outside();

    let mut largest_rect = 0;

    for p1_index in 0..(points.len() - 1) {
        for p2_index in p1_index..points.len() {
            let rect = Polygon::con_rect(points[p1_index], points[p2_index]);

            if poly.rect_in_poly(&rect) {
                let area = points[p1_index].rect_area(&points[p2_index]);

                if area > largest_rect {
                    largest_rect = area;
                }
            }
        }
    }

    println!("{largest_rect}");
}
