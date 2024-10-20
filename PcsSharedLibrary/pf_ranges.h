#include "types.h"
#include <iostream>

using std::cout;
using std::string;
using std::make_shared;

using namespace pathfinder_combat_simulator;

class pf_ranges
{
public:
    pf_ranges()
    {
        for (int i = 0; i <= 80; i++)
            armor_classes.push_back(i);

        for (int i = -70; i <= 70; i++)
            attack_minus_ac.push_back(i);
    }

    // the attack bonus of the attacker
    vector<int> attack_bonuses = vector<int>{ -10, -9, -8, -7, -6, -5, -4, -3, -2, -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

    // the armor class of the target
    vector<int> armor_classes = vector<int>();

    // the die role needed to crit
    vector<int> minimum_crits = vector<int>{ 20 };

    // the multiplier applied when crit occurs
    vector<int> crit_multipliers = vector<int>{ 2 };

    // the string representing the damage dice
    vector<string> damage_dice_options = vector<string> { "1d3",    "1d4",  "1d6",  "1d8",  "1d10", "1d12", "1d20" };
    /*vector<float> mean_damages = vector<float>          { 2.0f,     2.5f,   3.5f,   4.5f,   5.5f,   6.5f,   10.5f };*/
	vector<int> die_sizes = vector<int>                 { 3,        4,      6,      8,      10,     12,     20 };

    // how many damage dice to roll
    vector<int> damage_dice_count = vector<int>{ 1, 2 };

    // the die role for the attack
    vector<int> unmodified_attack_rolls = vector<int>{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20 };

    // the attack outcome
    vector<attack_outcome> attack_outcomes = vector<attack_outcome>{ attack_outcome::hit_and_crit, attack_outcome::hit_no_crit, attack_outcome::miss };


    vector<int> attribute_modifiers = vector<int>{ -5, -4, -3, -2, -1, 0, 1, 2, 3, 4, 5 };

    vector<int> attack_minus_ac = vector<int>{ -10, -9, -8, -7, -6, -5, -4, -3, -2, -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20 };
};