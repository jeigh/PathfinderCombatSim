#include <iostream>


#include "dice_rolling_tests.h"
#include "combat_algorithm.h"
#include "types.h"

using std::cout;
using std::string;

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


class combat_algorithm_tests : public testClassBase
{
public:
	explicit combat_algorithm_tests(const shared_ptr<combat_algorithm>& combat_algorithm) : combatAlgorithm(combat_algorithm)	{ }

private:
    shared_ptr<combat_algorithm> combatAlgorithm;

public:
	void run_test() const override
	{
        auto orcWarrior = make_shared<mobile_object>("orcWarrior",  23, 18, 0, make_shared<OrcNeckSplitterAttack>());
        auto orcBrute = make_shared<mobile_object>("orcBrute",  23, 18, 0, make_shared<OrcNeckSplitterAttack>());
        
        vector<shared_ptr<combat_team>> combatGroups; 
        
        combatGroups.push_back(make_shared<combat_team>("orcWarriorTeam", vector{ orcWarrior }));
        combatGroups.push_back(make_shared<combat_team>("orcBruteTeam", vector{ orcBrute }));
        
		vector<shared_ptr<combat_team>> abc = combatAlgorithm->process_battle_until_only_one_team_is_concious(combatGroups);

        

	}


};


int main() 
{
    auto seed = time(nullptr);
    srand(static_cast<unsigned int> (seed));
    cout << "Hit a key to begin...\n";

    if (!getchar()) return 0;

    auto diceManager = make_shared<dice_manager>();
    auto userInterface = make_shared<user_interface>();

    auto mobAi = make_shared <mob_ai>();
    auto combatAlgorithm = make_shared<combat_algorithm>(diceManager, userInterface, mobAi);

	auto combatAlgorithmTests = make_shared<combat_algorithm_tests>(combatAlgorithm);
	auto tests = vector<shared_ptr<testClassBase>>();
    
    tests.push_back(combatAlgorithmTests);

    for (int i = 0; i < 10000; ++i)
    {
	    for (shared_ptr<testClassBase> test : tests)
	    {
            test->run_test();
	    }    	
    }
    cout << "yeah\n";

}



