#include <iostream>
#include <thread>

#include "types.h"

using std::cout;
using std::string;
using std::make_shared;

using namespace pathfinder_combat_simulator;


class OrcNeckSplitterAttack : public attack
{
public:
	OrcNeckSplitterAttack()
	{
        shared_ptr<die_roll> theDamageDice = make_shared<die_roll>();

        theDamageDice->die_count = 1;
        theDamageDice->die_size = 8;
        theDamageDice->modifer_after_addition = 4;

		auto theDamageEffect = make_shared<damage_effect>();
        vector vector_of_damage_dice = { theDamageDice };
        theDamageEffect->damage_dice = vector_of_damage_dice;
        theDamageEffect->_damage_type = physical;
        damage_effects_.push_back(*theDamageEffect);
	}
};


class combat_algorithm_tests 
{
public:
	explicit combat_algorithm_tests(const shared_ptr<combat_process>& combat_algorithm, int battle_id) : combatAlgorithm(combat_algorithm), battle_id_(battle_id) { }

private:
    shared_ptr<combat_process> combatAlgorithm;
    int battle_id_;

public:
	void run_test()  
	{
        // brutes
        auto ugthar = make_shared<mobile_object>("Ugthar collector of toes", 30, 18, 0, make_shared<OrcNeckSplitterAttack>());
        auto bloodfist = make_shared<mobile_object>("Bloodfist the cannibal", 30, 18, 0, make_shared<OrcNeckSplitterAttack>());

		// warriors
        auto thaddeus = make_shared<mobile_object>("Thaddeus the Angry", 23, 18, 0, make_shared<OrcNeckSplitterAttack>());
        auto diosthenes = make_shared<mobile_object>("Diosthenes the Pensive", 23, 18, 0, make_shared<OrcNeckSplitterAttack>());

        // cowards
        auto timmidides = make_shared<mobile_object>("Timmidides the coward", 11, 18, 0, make_shared<OrcNeckSplitterAttack>());
        auto squeaks = make_shared<mobile_object>("Squeaks", 10, 18, 0, make_shared<OrcNeckSplitterAttack>());      

        vector<shared_ptr<combat_team>> combatGroups;
        
        combatGroups.push_back(make_shared<combat_team>("Clan Foesmash", vector{ thaddeus, ugthar  }));
        combatGroups.push_back(make_shared<combat_team>("Clan Skullmugs", vector{ bloodfist, timmidides }));

        // enable when multiple groups work
        //combatGroups.push_back(make_shared<combat_team>("Clan Femurclub", vector{ diosthenes, squeaks }));

        auto the_battle = make_shared<battle>(battle_id_, combatGroups);

		combatAlgorithm->process_battle_until_only_one_team_is_concious(the_battle);
	}
};


class ranges
{
public:
    ranges()
    {
        for (int i = 0; i <= 80; i++)
            armor_classes.push_back(i);
    }

    // the attack bonus of the attacker
    vector<int> attack_bonuses = vector<int>{ -10, -9, -8, -7, -6, -5, -4, -3, -2, -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
    
    // the armor class of the target
    vector<int> armor_classes = vector<int>();
    
    // the die role needed to crit
    vector<int> minimum_crits = vector<int>{ 18, 19, 20 };
    
    // the multiplier applied when crit occurs
    vector<int> crit_multipliers = vector<int>{ 1, 2, 3 };
    
    // the string representing the damage dice
    vector<string> damage_dice_options = vector<string>{ "1d3", "1d4", "1d6", "1d8", "1d10", "1d12", "1d20" };
    
    // the predicted mean of the die
	vector<float> mean_damages = vector<float>         { 2.0f,  2.5f,  3.5f,  4.5f,  5.5f,   6.5f,   10.5f };
    
    // how many damage dice to roll
	vector<int> damage_dice_count = vector<int>{ 1, 2 };   
    
    // the die role for the attack
    vector<int> unmodified_attack_rolls = vector<int>{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20 };

    // the attack outcome
	vector<attack_outcome> attack_outcomes = vector<attack_outcome>{ attack_outcome::hit_and_crit, attack_outcome::hit_no_crit, attack_outcome::miss };

    //todo: add strength mod for damage purposes
	vector<int> strength_modifiers = vector<int>{ -5, -4, -3, -2, -1, 0, 1, 2, 3, 4, 5 };
};


void GenerateDamagesForRanges(ranges& input_ranges, std::shared_ptr<pathfinder_combat_simulator::attack_abstraction> atk, std::shared_ptr<pathfinder_combat_simulator::data_access> dal);
void GenerateAttacksForRanges(ranges& input_ranges, std::shared_ptr<pathfinder_combat_simulator::attack_abstraction> atk, std::shared_ptr<pathfinder_combat_simulator::data_access> dal);

int main()
{
    auto run_single_threaded = true;
    auto seed = time(nullptr);

    srand(static_cast<unsigned int>(seed));

    cout << "Hit a key to begin...\n";

    if (!getchar()) return 0;

    //original_combat_stuff(run_single_threaded);

    auto rng = make_shared<dice_manager>();
    auto dal = make_shared<data_access>(make_shared<std::shared_mutex>());
    auto atk = make_shared<attack_abstraction>(rng, dal);
    vector<shared_ptr<attack_scenario>> scenarios;

    // there may be a better way of doing this without using nesting.
    auto input_ranges = ranges();
    
    GenerateAttacksForRanges(input_ranges, atk, dal);
    GenerateDamagesForRanges(input_ranges, atk, dal);
}

void GenerateAttacksForRanges(ranges& input_ranges, std::shared_ptr<pathfinder_combat_simulator::attack_abstraction> atk, std::shared_ptr<pathfinder_combat_simulator::data_access> dal)
{
    // attack loop
    int ai = 0;
    for (int attack_bonus : input_ranges.attack_bonuses)
    {
        for (int armor_class : input_ranges.armor_classes)
        {
            for (int minimum_crit : input_ranges.minimum_crits)
            {
                for (int unmodified_attack_roll : input_ranges.unmodified_attack_rolls)
                {
                    for (float statistical_damage_mean : input_ranges.mean_damages)
                    {
                        float returnable = 0.0f;

                        auto attack_req = make_shared<attack_request>(unmodified_attack_roll, attack_bonus, minimum_crit, armor_class);
                        auto this_attack_outcome = atk->get_attack_outcome(attack_req);

                        dal->persist_attack_results(attack_req, this_attack_outcome);
                        ai++;
                    }
                }
            }
        }
    }

    cout << ai << " Attacks calculated" << std::endl;
}

void GenerateDamagesForRanges(ranges& input_ranges, std::shared_ptr<pathfinder_combat_simulator::attack_abstraction> atk, std::shared_ptr<pathfinder_combat_simulator::data_access> dal)
{
    // damage loop
    int di = 0;
    for (auto this_attack_outcome : input_ranges.attack_outcomes)
    {
        for (int armor_class : input_ranges.armor_classes)
        {
            for (int crit_multiplier : input_ranges.crit_multipliers)
            {
                for (int damage_dice_count : input_ranges.damage_dice_count)
                {
                    for (float statistical_damage_mean : input_ranges.mean_damages)
                    {
                        auto dmg_strategy = make_shared<statistical_mean_damage_strategy>(damage_dice_count, statistical_damage_mean);
                        auto dmg_req = make_shared<damage_request>(this_attack_outcome, crit_multiplier, dmg_strategy);
                        auto expected_result = atk->get_damage_outcome(dmg_req);

                        dal->persist_damage_results(damage_dice_count, statistical_damage_mean, dmg_req, expected_result);
                        di++;
                    }
                }
            }
        }
    }

    cout << di << " Damages calculated" << std::endl;
}

auto original_combat_stuff(bool run_single_threaded) 
{
    auto diceManager = make_shared<dice_manager>();
    auto ui_mutex = std::make_shared<std::shared_mutex>();
    auto userInterface = make_shared<user_interface>(ui_mutex, output_level::low);
    auto mobAi = make_shared <mob_ai>();
    auto db_mutex = std::make_shared<std::shared_mutex>();
    auto dal = make_shared<data_access>(db_mutex);
    auto combatAlgorithm = make_shared<combat_process>(diceManager, userInterface, mobAi, dal);

    if (!run_single_threaded) {
        vector<std::shared_ptr<std::thread>> the_threads;
        for (int i = 0; i < 100; ++i)
        {
            auto combatAlgorithmTests = make_shared<combat_algorithm_tests>(combatAlgorithm, i);
            auto back_pushable = std::make_shared<std::thread>([combatAlgorithmTests] { combatAlgorithmTests->run_test(); });
            the_threads.push_back(back_pushable);
        }

        for (std::shared_ptr<std::thread> the_thread : the_threads)
        {
            the_thread->join();
        }
    }
    else {
        for (int i = 0; i < 100; ++i)
        {
            auto combatAlgorithmTests = make_shared<combat_algorithm_tests>(combatAlgorithm, i);
            combatAlgorithmTests->run_test();
        }
    }

}



