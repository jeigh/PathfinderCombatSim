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

void GenerateDamagesForRanges(pf_ranges& input_ranges, attack_abstraction& atk, std::shared_ptr<pathfinder_combat_simulator::data_access> dal)
{
    // damage loop
    int di = 0;

    for (int attribute_modifier : input_ranges.attribute_modifiers)
    {
        for (auto this_attack_outcome : input_ranges.attack_outcomes)
        {
            for (int crit_multiplier : input_ranges.crit_multipliers)
            {
                for (int damage_dice_count : input_ranges.damage_dice_count)
                {
                    for (int die_size : input_ranges.die_sizes)
                    {
                        auto dmg_strategy = statistical_mean_damage_strategy(damage_dice_count, die_size);
                        auto dmg_req = make_shared<damage_request>(this_attack_outcome, crit_multiplier, dmg_strategy, attribute_modifier);
                        auto expected_result = atk.get_damage_outcome(dmg_req);

                        dal->persist_damage_results(damage_dice_count, die_size, dmg_req, expected_result);
                        di++;
                    }
                }
            }
        }
    }
    cout << di << " Damages calculated" << std::endl;
}

int main()
{
    auto run_single_threaded = true;
    auto seed = time(nullptr);
    srand(static_cast<unsigned int>(seed));
    auto rng = dice_manager();
    auto dal = make_shared<data_access>(make_shared<std::shared_mutex>());
    auto atk = attack_abstraction(rng);
    auto input_ranges = pf_ranges();

	GenerateDamagesForRanges(input_ranges, atk, dal);
}
