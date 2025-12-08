use std::cmp::Reverse;
use std::collections::BinaryHeap;
use std::io;
use std::mem::swap;

struct Point {
    x: i64,
    y: i64,
    z: i64,
}

impl Point {
    /*
     * because we only need to compare to get the min spanning tree getting the square root is not needed
     */
    fn euc_dist_ord(&self, other: &Self) -> i64 {
        let x_dist = self.x - other.x;
        let y_dist = self.y - other.y;
        let z_dist = self.z - other.z;

        return x_dist * x_dist + y_dist * y_dist + z_dist * z_dist;
    }
}

#[derive(PartialEq, Eq, PartialOrd, Ord)]
struct Edge {
    len: i64,
    ver_1: usize,
    ver_2: usize,
}

struct Graph {
    edges: Vec<Vec<Option<i64>>>,
}

impl Graph {
    fn kruskal(&self) -> Option<Edge> {
        let mut edge_heap = BinaryHeap::new();

        for (ver_1, ver_1_edges) in self.edges.iter().enumerate() {
            for (ver_2, edge_len) in ver_1_edges.iter().enumerate() {
                if let Some(edge_len) = *edge_len {
                    edge_heap.push(Reverse(Edge {
                        len: edge_len,
                        ver_1,
                        ver_2,
                    }));
                }
            }
        }

        let mut unions = UnionFind {
            members: Vec::new(),
            unions: Vec::new(),
        };

        for (point, _) in self.edges.iter().enumerate() {
            unions.new_member(point);
        }

        let mut last_edge = None;

        'outer: loop {
            let mut edge;

            loop {
                edge = edge_heap.pop().unwrap().0;

                if !unions.are_connected(edge.ver_1, edge.ver_2) {
                    unions.merge(edge.ver_1, edge.ver_2);

                    break;
                }

                if edge_heap.len() == 0 {
                    break 'outer;
                }
            }

            last_edge = Some(edge);
        }

        last_edge
    }

    fn add_edge(&mut self, ver_1: usize, ver_2: usize, weight: Option<i64>) {
        let mut ver_1 = ver_1;
        let mut ver_2 = ver_2;

        if ver_1 > ver_2 {
            swap(&mut ver_1, &mut ver_2);
        }

        let ver_1_vec_len = self.edges.len();

        //because ver_1 <= ver_2 this will cover ver_1 and allows for iterating over all vertexs with self.edges
        if ver_1_vec_len <= ver_2 {
            self.edges
                .extend(vec![Vec::new(); ver_2 - ver_1_vec_len + 1]);
        }

        let ver_2_vec_len = self.edges[ver_1].len();

        if ver_2_vec_len <= ver_2 {
            self.edges[ver_1].extend(vec![None; ver_2 - ver_2_vec_len + 1]);
        }

        self.edges[ver_1][ver_2] = weight;
    }
}

struct UnionFind {
    members: Vec<Option<usize>>,
    unions: Vec<usize>,
}

impl UnionFind {
    fn new_member(&mut self, member: usize) {
        let new_union_name = self.unions.len();

        self.unions.push(new_union_name);

        let members_len = self.members.len();

        if members_len <= member {
            self.members.extend(vec![None; member - members_len + 1]);
        }

        self.members[member] = Some(new_union_name);
    }

    fn merge(&mut self, union_1_member: usize, union_2_member: usize) {
        let new_union_name = self.unions.len();

        self.unions.push(new_union_name);

        let union_1_root = self.find(union_1_member);
        let union_2_root = self.find(union_2_member);

        self.unions[union_1_root] = new_union_name;
        self.unions[union_2_root] = new_union_name;
    }

    fn find(&mut self, member: usize) -> usize {
        let mut union_root = self.unions[self.members[member].expect("{member} not included")];

        loop {
            let last_node = union_root;

            union_root = self.unions[union_root];

            if union_root == self.unions[union_root] {
                break;
            }

            self.unions[last_node] = self.unions[union_root];
        }

        union_root
    }

    fn are_connected(&mut self, member_1: usize, member_2: usize) -> bool {
        self.find(member_1) == self.find(member_2)
    }
}

pub fn solver() {
    let mut points: Vec<Point> = Vec::new();

    for line in io::stdin().lines() {
        let line = line.unwrap();

        let mut num = line.split([',', '\n']);

        points.push(Point {
            x: num.next().unwrap().parse().unwrap(),
            y: num.next().unwrap().parse().unwrap(),
            z: num.next().unwrap().parse().unwrap(),
        });
    }

    let mut graph = Graph { edges: Vec::new() };

    for point_1 in 0..points.len() {
        for point_2 in point_1..points.len() {
            if point_1 == point_2 {
                graph.add_edge(point_1, point_2, None);
            } else {
                graph.add_edge(
                    point_1,
                    point_2,
                    Some(points[point_1].euc_dist_ord(&points[point_2])),
                );
            }
        }
    }

    let last_edge = graph.kruskal().unwrap();

    println!("{}", points[last_edge.ver_1].x * points[last_edge.ver_2].x);
}
