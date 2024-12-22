/*
 * convert each passport into a single string then run .find on each passport
*/

use std::io::stdin;

struct Fields {
    byr: bool,
    iyr: bool,
    eyr: bool,
    hgt: bool,
    hcl: bool,
    ecl: bool,
    pid: bool,
    cid: bool,
}

impl Default for Fields {
    fn default() -> Self {
        Self {
            byr: Default::default(),
            iyr: Default::default(),
            eyr: Default::default(),
            hgt: Default::default(),
            hcl: Default::default(),
            ecl: Default::default(),
            pid: Default::default(),
            cid: Default::default(),
        }
    }
}

impl Fields {
    fn is_valid(&self) -> bool {
        if self.byr == true
            && self.iyr == true
            && self.eyr == true
            && self.hgt == true
            && self.hcl == true
            && self.ecl == true
            && self.pid == true
        {
            return true;
        }

        return false;
    }
}

fn main() {
    let mut passports: Vec<String> = vec!["".to_string()];

    for line in stdin().lines().map(|s| s.unwrap().replace("\n", "")) {
        if line.is_empty() {
            passports.push("".to_string());
        } else {
            passports.last_mut().unwrap().push_str(&line);
            passports.last_mut().unwrap().push_str(" ");
        }
    }

    let mut valid_pass_count = 0;

    for passport in passports {
        let mut cur_fields: Fields = Fields::default();
        let mut fields = passport.split([' ', ':']).into_iter().peekable();
        let mut cur_str = fields.next().unwrap();
        while !cur_str.is_empty(){
            match cur_str{
                "byr" => {
                    let field = fields.peek().unwrap();
                    if field.chars().count() == 4{
                        let num = field.parse::<i32>().unwrap();
                        if num >= 1920 && num <= 2002{
                            cur_fields.byr = true;
                        }
                    }
                },
                "iyr" => cur_fields.iyr = true,
                "eyr" => cur_fields.eyr = true,
                "hgt" => cur_fields.hgt = true,
                "hcl" => cur_fields.hcl = true,
                "ecl" => cur_fields.ecl = true,
                "pid" => cur_fields.pid = true,
                "cid" => cur_fields.cid = true,
                _ => (),
            }

            cur_str = fields.next().unwrap();
        }

        if cur_fields.is_valid(){
            valid_pass_count += 1;
        }
    }

    println!("{}", valid_pass_count);
}
