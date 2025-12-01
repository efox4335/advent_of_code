use std::{collections::HashMap, io};
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

    let mut allergen_free_count = 0;

    for (_, ingredient) in ingredients.iter_mut() {
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

        if ingredient.possible_allergens.len() == 0 {
            allergen_free_count += ingredient.foods_in.len();
        }
    }

    println!("{allergen_free_count}")
}
