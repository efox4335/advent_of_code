use std::collections::VecDeque;
use std::io::stdin;

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

fn round(deck_1: &mut VecDeque<i32>, deck_2: &mut VecDeque<i32>) {
    let deck_1_top_card = deck_1.pop_front().unwrap();
    let deck_2_top_card = deck_2.pop_front().unwrap();

    if deck_1_top_card > deck_2_top_card {
        deck_1.push_back(deck_1_top_card);
        deck_1.push_back(deck_2_top_card);
    } else {
        deck_2.push_back(deck_2_top_card);
        deck_2.push_back(deck_1_top_card);
    }
}

fn calculate_score(winning_deck: &mut VecDeque<i32>) -> i32 {
    let mut score: i32 = 0;

    for (index, value) in winning_deck.iter().rev().enumerate() {
        score += value * (index as i32 + 1);
    }

    score
}

pub fn solver() {
    let (mut deck_1, mut deck_2) = parse_input();

    while !deck_1.is_empty() && !deck_2.is_empty() {
        round(&mut deck_1, &mut deck_2);
    }

    if deck_1.is_empty() {
        println!("{}", calculate_score(&mut deck_2));
    } else {
        println!("{}", calculate_score(&mut deck_1));
    }
}
