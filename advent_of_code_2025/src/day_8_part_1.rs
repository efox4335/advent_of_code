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
    fn kruskal_iter(&self, iter: usize) -> Graph {
        let mut edge_vec = Vec::new();

        for (ver_1, ver_1_edges) in self.edges.iter().enumerate() {
            for (ver_2, edge_len) in ver_1_edges.iter().enumerate() {
                if let Some(edge_len) = *edge_len {
                    edge_vec.push(Reverse(Edge {
                        len: edge_len,
                        ver_1,
                        ver_2,
                    }));
                }
            }
        }

        let mut edge_heap = BinaryHeap::from(edge_vec);

        let mut unions = UnionFind {
            members: Vec::new(),
            unions: Vec::new(),
        };

        for (point, _) in self.edges.iter().enumerate() {
            unions.new_member(point);
        }

        let mut min_spann_tree_graph = Graph { edges: Vec::new() };

        for _ in 0..iter {
            let edge;

            loop {
                edge = edge_heap.pop().unwrap().0;

                if !unions.are_connected(edge.ver_1, edge.ver_2) {
                    unions.merge(edge.ver_1, edge.ver_2);

                    break;
                }

                //i cann't read
                break;
            }

            min_spann_tree_graph.add_edge(edge.ver_1, edge.ver_2, Some(edge.len));
        }

        min_spann_tree_graph
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

    // (member, size)
    fn get_connected_comp(&self) -> Vec<(usize, usize)> {
        let mut seen = vec![false; self.edges.len()];

        let mut connected_comp = Vec::new();

        for (ver, _) in self.edges.iter().enumerate() {
            if seen[ver] {
                continue;
            }

            seen[ver] = true;

            let mut vis_stack = Vec::new();

            vis_stack.push(ver);

            let mut comp_size = 0;

            while vis_stack.len() > 0 {
                let vis_ver = vis_stack.pop().unwrap();

                comp_size += 1;

                for pos_ver in 0..self.edges.len() {
                    if seen[pos_ver] {
                        continue;
                    }

                    if self.is_connected(pos_ver, vis_ver) {
                        vis_stack.push(pos_ver);
                        seen[pos_ver] = true;
                    }
                }
            }

            connected_comp.push((ver, comp_size));
        }

        connected_comp
    }

    fn is_connected(&self, ver_1: usize, ver_2: usize) -> bool {
        let mut ver_1 = ver_1;
        let mut ver_2 = ver_2;

        if ver_1 == ver_2 {
            return false;
        }

        if ver_1 > ver_2 {
            swap(&mut ver_1, &mut ver_2);
        }

        if ver_1 >= self.edges.len() {
            return false;
        }

        if ver_2 >= self.edges[ver_1].len() {
            return false;
        }

        match self.edges[ver_1][ver_2] {
            Some(_) => true,
            None => false,
        }
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

    let connected_comp = graph.kruskal_iter(1000).get_connected_comp();

    let mut comp_sizes: Vec<usize> = connected_comp
        .iter()
        .map(|(_, size)| *size)
        .collect::<Vec<usize>>();

    comp_sizes.sort();
    comp_sizes.reverse();

    println!("{}", comp_sizes[0] * comp_sizes[1] * comp_sizes[2]);
}
