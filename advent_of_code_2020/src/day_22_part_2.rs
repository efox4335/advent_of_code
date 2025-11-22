use std::collections::{HashMap, HashSet, VecDeque};
use std::io::stdin;

#[derive(Clone)]
enum DeckPart {
    Player1,
    Player2,
}

fn parse_input() -> (VecDeque<i32>, VecDeque<i32>) {
    let input = stdin().lines();

    let mut player_1_deck = VecDeque::new();
    let mut player_2_deck = VecDeque::new();

    let mut deck = DeckPart::Player1;

    for line in input {
        let line = line.expect("parse error");

        match line.chars().next().unwrap_or('\n') {
            'P' => continue,
            '\n' => {
                deck = DeckPart::Player2;
                continue;
            }
            _ => (),
        }

        match deck {
            DeckPart::Player1 => player_1_deck.push_back(line.parse().expect("parse int error")),
            DeckPart::Player2 => player_2_deck.push_back(line.parse().expect("parse int error")),
        }
    }

    (player_1_deck, player_2_deck)
}

fn round(
    deck_1: &mut VecDeque<i32>,
    deck_2: &mut VecDeque<i32>,
    game_cache: &mut HashMap<String, DeckPart>,
) {
    let deck_1_top_card = deck_1.pop_front().unwrap();
    let deck_2_top_card = deck_2.pop_front().unwrap();

    if deck_1_top_card as usize <= deck_1.len() && deck_2_top_card as usize <= deck_2.len() {
        let mut subgame_deck_1 = VecDeque::new();

        for (index, card) in deck_1.iter().enumerate() {
            if index == deck_1_top_card as usize {
                break;
            }

            subgame_deck_1.push_back(card.clone());
        }

        let mut subgame_deck_2 = VecDeque::new();

        for (index, card) in deck_2.iter().enumerate() {
            if index == deck_2_top_card as usize {
                break;
            }

            subgame_deck_2.push_back(card.clone());
        }

        match game(&mut subgame_deck_1, &mut subgame_deck_2, game_cache) {
            DeckPart::Player1 => {
                deck_1.push_back(deck_1_top_card);
                deck_1.push_back(deck_2_top_card);
            }
            DeckPart::Player2 => {
                deck_2.push_back(deck_2_top_card);
                deck_2.push_back(deck_1_top_card);
            }
        }

        return;
    }

    if deck_1_top_card > deck_2_top_card {
        deck_1.push_back(deck_1_top_card);
        deck_1.push_back(deck_2_top_card);
    } else {
        deck_2.push_back(deck_2_top_card);
        deck_2.push_back(deck_1_top_card);
    }
}

fn game(
    deck_1: &mut VecDeque<i32>,
    deck_2: &mut VecDeque<i32>,
    game_cache: &mut HashMap<String, DeckPart>,
) -> DeckPart {
    let mut states = HashSet::new();

    let starting_state = game_state_to_string(deck_1, deck_2);

    if let Some(res) = game_cache.get(&starting_state) {
        return res.clone();
    }

    while !deck_1.is_empty() && !deck_2.is_empty() {
        let cur_state = game_state_to_string(deck_1, deck_2);

        if states.contains(&cur_state) {
            game_cache.insert(starting_state, DeckPart::Player1);

            return DeckPart::Player1;
        }

        states.insert(cur_state);

        round(deck_1, deck_2, game_cache);
    }

    if deck_1.is_empty() {
        game_cache.insert(starting_state, DeckPart::Player2);

        return DeckPart::Player2;
    } else {
        game_cache.insert(starting_state, DeckPart::Player1);

        return DeckPart::Player1;
    }
}

#[inline(always)]
fn game_state_to_string(deck_1: &mut VecDeque<i32>, deck_2: &mut VecDeque<i32>) -> String {
    let mut output = String::new();

    for card in deck_1 {
        output += &card.to_string();
        output.push(',');
    }

    // needed because otherwise deck_1: 1, 2, deck_2: 3 and deck_1: 1, deck_2: 2, 3 are the same
    output.push('|');

    for card in deck_2 {
        output += &card.to_string();
        output.push(',');
    }

    output
}

fn calculate_score(winning_deck: &mut VecDeque<i32>) -> i32 {
    let mut score: i32 = 0;

    println!("{winning_deck:?}");

    for (index, value) in winning_deck.iter().rev().enumerate() {
        score += value * (index as i32 + 1);
    }

    score
}

pub fn solver() {
    let (mut deck_1, mut deck_2) = parse_input();

    let mut game_cache: HashMap<String, DeckPart> = HashMap::new();

    match game(&mut deck_1, &mut deck_2, &mut game_cache) {
        DeckPart::Player1 => println!("{}", calculate_score(&mut deck_1)),
        DeckPart::Player2 => println!("{}", calculate_score(&mut deck_2)),
    }
}
