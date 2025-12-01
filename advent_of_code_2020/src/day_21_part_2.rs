use std::{collections::HashMap, io};

#[derive(Debug)]
struct Ingredient {
    possible_allergens: Vec<String>,
    foods_in: Vec<usize>,
}

struct Allergen {
    foods_in: Vec<usize>,
}

#[derive(PartialEq)]
enum FoodPart {
    Ingredient,
    Allergen,
}

pub fn solver() {
    let mut ingredients: HashMap<String, Ingredient> = HashMap::new();
    let mut allergens: HashMap<String, Allergen> = HashMap::new();

    for (food_num, line) in io::stdin().lines().enumerate() {
        let mut part = FoodPart::Ingredient;

        for element in line.unwrap().split([',', ' ', '(', ')']) {
            if element == "contains" {
                part = FoodPart::Allergen;

                continue;
            }

            if element == "" {
                continue;
            }

            if part == FoodPart::Ingredient {
                if let Some(ingredient) = ingredients.get_mut(element) {
                    ingredient.foods_in.push(food_num);
                } else {
                    ingredients.insert(
                        element.to_string(),
                        Ingredient {
                            possible_allergens: Vec::new(),
                            foods_in: vec![food_num],
                        },
                    );
                }
            } else {
                if let Some(allergen) = allergens.get_mut(element) {
                    allergen.foods_in.push(food_num);
                } else {
                    allergens.insert(
                        element.to_string(),
                        Allergen {
                            foods_in: vec![food_num],
                        },
                    );
                }
            }
        }
    }

    let mut largest_possible_allergen = 0;

    let mut ingredients_to_remove: Vec<String> = Vec::new();

    for (ingredient_name, ingredient) in ingredients.iter_mut() {
        for (allergen, foods_in) in allergens.iter() {
            let mut in_all_foods = true;

            for food in foods_in.foods_in.iter() {
                if !ingredient.foods_in.contains(&food) {
                    in_all_foods = false;
                }
            }

            if in_all_foods {
                ingredient.possible_allergens.push(allergen.to_string());
            }
        }

        if ingredient.possible_allergens.len() > largest_possible_allergen {
            largest_possible_allergen = ingredient.possible_allergens.len();
        }

        if ingredient.possible_allergens.len() == 0 {
            ingredients_to_remove.push(ingredient_name.to_string());
        }
    }

    for to_remove in &ingredients_to_remove {
        ingredients.remove(to_remove);
    }

    let mut known_allergens: Vec<(String, String)> = Vec::new();

    while !ingredients.is_empty() {
        let mut ingredient_to_remove = String::new();
        let mut allergen_to_remove = String::new();

        for (name, ingredient) in ingredients.iter_mut() {
            if ingredient.possible_allergens.len() == 1 {
                ingredient_to_remove = name.to_string();
                allergen_to_remove = ingredient.possible_allergens[0].to_string();

                break;
            }
        }

        let ingredient = ingredients.remove_entry(&ingredient_to_remove).unwrap();
        known_allergens.push((ingredient.0, ingredient.1.possible_allergens[0].to_string()));

        for (_, conflict_ingredient) in ingredients.iter_mut() {
            conflict_ingredient.possible_allergens = conflict_ingredient
                .possible_allergens
                .iter()
                .filter(|aller| *aller != &allergen_to_remove)
                .map(|s| s.to_string())
                .collect();
        }
    }

    known_allergens.sort_by(|a, b| a.1.cmp(&b.1));

    for (num, (ingredient, _)) in known_allergens.iter().enumerate() {
        print!("{}", ingredient);

        if num == known_allergens.len() - 1 {
            print!("\n");
        } else {
            print!(",");
        }
    }
}
