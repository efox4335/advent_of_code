/*
 * FLOOD FILL
*/

use std::io::stdin;

#[derive(Copy, Clone)]
struct PlantTile {
    plant: char,
    is_visited: bool,
}

impl Default for PlantTile {
    fn default() -> Self {
        PlantTile {
            plant: 0 as char,
            is_visited: false,
        }
    }
}

struct ShapeData {
    area: i32,
    perimeter: i32,
}

#[derive(Copy, Clone)]
struct Cord {
    row: usize,
    col: usize,
}

const BUFSIZE: usize = 200;

fn get_shape_data(
    point: Cord,
    input: &mut [[PlantTile; BUFSIZE]; BUFSIZE],
    bound: usize,
) -> ShapeData {
    let mut visit_stack: Vec<Cord> = vec![];
    let cur_type = input[point.row][point.col].plant;
    visit_stack.push(point);
    input[point.row][point.col].is_visited = true;
    let mut shape_data: ShapeData = ShapeData {
        area: 0,
        perimeter: 0,
    };

    while !(visit_stack.is_empty()) {
        let cur_point = visit_stack.pop().unwrap();
        let mut neighbour_count = 0;


        //top
        if cur_point.row > 0 && input[cur_point.row - 1][cur_point.col].plant == cur_type {
            neighbour_count += 1;
            if input[cur_point.row - 1][cur_point.col].is_visited == false {
                input[cur_point.row - 1][cur_point.col].is_visited = true;
                visit_stack.push(Cord {
                    row: (cur_point.row - 1),
                    col: cur_point.col,
                });
            }
        }

        //bottom
        if cur_point.row < bound && input[cur_point.row + 1][cur_point.col].plant == cur_type {
            neighbour_count += 1;
            if input[cur_point.row + 1][cur_point.col].is_visited == false {
                input[cur_point.row + 1][cur_point.col].is_visited = true;
                visit_stack.push(Cord {
                    row: (cur_point.row + 1),
                    col: cur_point.col,
                });
            }
        }

        //left
        if cur_point.col > 0 && input[cur_point.row][cur_point.col - 1].plant == cur_type {
            neighbour_count += 1;
            if input[cur_point.row][cur_point.col - 1].is_visited == false {
                input[cur_point.row][cur_point.col - 1].is_visited = true;
                visit_stack.push(Cord {
                    row: cur_point.row,
                    col: (cur_point.col - 1),
                });
            }
        }

        //right
        if cur_point.col < bound && input[cur_point.row][cur_point.col + 1].plant == cur_type {
            neighbour_count += 1;
            if input[cur_point.row][cur_point.col + 1].is_visited == false {
                input[cur_point.row][cur_point.col + 1].is_visited = true;
                visit_stack.push(Cord {
                    row: cur_point.row,
                    col: (cur_point.col + 1),
                });
            }
        }

        shape_data.area += 1;
        shape_data.perimeter += 4 - neighbour_count;
    }
    shape_data
}

fn main() {
    let mut input: [[PlantTile; BUFSIZE]; BUFSIZE] = [[PlantTile::default(); BUFSIZE]; BUFSIZE];
    let mut bound: usize = 0;

    for (line_count, line) in stdin().lines().enumerate() {
        for (char_count, plant) in line.unwrap().chars().enumerate() {
            if plant != '\n' {
                input[line_count][char_count].plant = plant;
            }
        }

        bound = line_count;
    }



    let mut line: usize = 0;
    let mut total = 0;

    while line <= bound{
        let mut plant = 0;
        while plant <= bound{
            if input[line][plant].is_visited == false{
                let shape_data = get_shape_data(Cord{row: line, col: plant}, &mut input, bound);

                println!("{} {}", shape_data.area, shape_data.perimeter);

                total += shape_data.area * shape_data.perimeter;
            }

            plant += 1;
        }

        line += 1;
    }

    println!("{}", total);
}
