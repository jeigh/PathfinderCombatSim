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
        theDamageDice->modifer_after_addition = 2;

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
        auto ugthar = make_shared<mobile_object>("Ugthar collector of toes", 30, 18, 0, 3, make_shared<OrcNeckSplitterAttack>());
        auto bloodfist = make_shared<mobile_object>("Bloodfist the cannibal", 30, 18, 0, 3, make_shared<OrcNeckSplitterAttack>());

        // warriors
        auto thaddeus = make_shared<mobile_object>("Thaddeus the Angry", 23, 18, 0, 2, make_shared<OrcNeckSplitterAttack>());
        auto diosthenes = make_shared<mobile_object>("Diosthenes the Pensive", 23, 18, 0, 0, make_shared<OrcNeckSplitterAttack>());

        // cowards
        auto timmidides = make_shared<mobile_object>("Timmidides the coward", 11, 18, 0, -1, make_shared<OrcNeckSplitterAttack>());
        auto squeaks = make_shared<mobile_object>("Squeaks", 10, 18, 0, -2, make_shared<OrcNeckSplitterAttack>());

        vector<shared_ptr<combat_team>> combatGroups;

        combatGroups.push_back(make_shared<combat_team>("Clan Foesmash", vector{ thaddeus, ugthar, squeaks }));
        combatGroups.push_back(make_shared<combat_team>("Clan Skullmugs", vector{ bloodfist, timmidides, diosthenes }));

        // enable when multiple groups work
        //combatGroups.push_back(make_shared<combat_team>("Clan Femurclub", vector{ diosthenes, squeaks }));

        auto the_battle = make_shared<battle>(battle_id_, combatGroups);

        combatAlgorithm->process_battle_until_only_one_team_is_concious(the_battle);
    }
};

void original_combat_stuff(shared_ptr<dice_manager> diceManager, shared_ptr<data_access> dal, shared_ptr<attack_abstraction> atk, bool run_single_threaded)
{

    auto ui_mutex = std::make_shared<std::shared_mutex>();
    auto userInterface = make_shared<user_interface>(ui_mutex, output_level::low);
    auto mobAi = make_shared <mob_ai>();
    auto db_mutex = std::make_shared<std::shared_mutex>();
    auto attackProcess = make_shared<attack_process>(dal, diceManager, atk);
    auto combatAlgorithm = make_shared<combat_process>(diceManager, userInterface, mobAi, attackProcess);

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




int main()
{
    auto run_single_threaded = true;
    auto seed = time(nullptr);

    srand(static_cast<unsigned int>(seed));

    cout << "Hit a key to begin...\n";

    if (!getchar()) return 0;

    auto rng = make_shared<dice_manager>();
    auto dal = make_shared<data_access>(make_shared<std::shared_mutex>());

    auto atk = make_shared<attack_abstraction>(rng, dal);

    original_combat_stuff(rng, dal, atk, run_single_threaded);
}


