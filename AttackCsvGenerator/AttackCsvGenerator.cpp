#include <iostream>
#include <thread>
#include <unordered_map>
#include <vector>
#include <string>
#include <memory>
#include <mutex>
#include <shared_mutex>
#include <fstream>

#include "types.h"
#include "pf_ranges.h"

using std::vector;
using std::unordered_map;
using std::string;
using std::unique_ptr;
using std::tuple;
using std::shared_ptr;

using std::cout;
using std::string;
using std::make_shared;

using namespace pathfinder_combat_simulator;

void GenerateAttacksForRanges(pf_ranges& input_ranges, std::shared_ptr<pathfinder_combat_simulator::attack_abstraction> atk, std::shared_ptr<pathfinder_combat_simulator::data_access> dal)
{
    // attack loop
    int ai = 0;
    int num_of_rows =
        input_ranges.attribute_modifiers.size() *
        input_ranges.unmodified_attack_rolls.size() *
        input_ranges.attack_bonuses.size() *
        input_ranges.armor_classes.size() *
        input_ranges.minimum_crits.size() *
        input_ranges.unmodified_attack_rolls.size();

    cout << "Generating " << num_of_rows << " attacks" << std::endl;

    for (int attribute_modifier : input_ranges.attribute_modifiers)
    {
        for (int confirmation_roll : input_ranges.unmodified_attack_rolls)
        {
            for (int attack_bonus : input_ranges.attack_bonuses)
            {
                for (int armor_class : input_ranges.armor_classes)
                {
                    for (int minimum_crit : input_ranges.minimum_crits)
                    {
                        for (int unmodified_attack_roll : input_ranges.unmodified_attack_rolls)
                        {
                            float returnable = 0.0f;

                            auto attack_req = make_shared<attack_request>(unmodified_attack_roll, attack_bonus, minimum_crit, armor_class, confirmation_roll, attribute_modifier);
                            auto this_attack_outcome = atk->get_attack_outcome(attack_req);

                            dal->persist_attack_results(attack_req, this_attack_outcome);
                            ai++;
                        }
                    }
                }
            }
        }
    }

    cout << ai << " Attacks calculated" << std::endl;
}



void main()
{

    auto run_single_threaded = true;
    auto seed = time(nullptr);
    srand(static_cast<unsigned int>(seed));
    auto rng = make_shared<dice_manager>();
    auto dal = make_shared<data_access>(make_shared<std::shared_mutex>());
    auto atk = make_shared<attack_abstraction>(rng);
    auto input_ranges = pf_ranges();

    GenerateAttacksForRanges(input_ranges, atk, dal);
}


