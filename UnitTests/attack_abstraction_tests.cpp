#include <memory>
#include <gtest/gtest.h>
#include "types.h"


using std::make_shared;

using namespace pathfinder_combat_simulator;

attack_abstraction create_unit_under_test() {
	return attack_abstraction(make_shared<dice_manager>());
}

TEST(AttackAbstractionTest, GetAttackOutcomeReturnsMissWhenAttackRollIs1) {
    // Arrange
    auto request = std::make_shared<attack_request>();

    request->unmodified_attack_roll = 1;
    request->attack_bonus = 2;
    request->minimum_crit = 18;
    request->armor_class = 15;
    request->crit_confirmation_roll = 15;

    auto abstraction = create_unit_under_test();

    // Act
    auto outcome = abstraction.get_attack_outcome(request);

    // Assert
    EXPECT_EQ(outcome, attack_outcome::miss);
}

TEST(AttackAbstractionTest, GetAttackOutcomeReturnsHitAndCritWhenAttackRollIs20AndCritConfirmationRollIs20) {
    // Arrange
    auto request = std::make_shared<attack_request>();
    request->unmodified_attack_roll = 20;
    request->attack_bonus = 2;
    request->minimum_crit = 18;
    request->armor_class = 15;
    request->crit_confirmation_roll = 20;

    auto abstraction = create_unit_under_test();

    // Act
    auto outcome = abstraction.get_attack_outcome(request);

    // Assert
    EXPECT_EQ(outcome, attack_outcome::hit_and_crit);
}

TEST(AttackAbstractionTest, GetAttackOutcomeReturnsHitNoCritWhenAttackRollIs20AndCritConfirmationRollIsNot20) {
    // Arrange
    auto request = std::make_shared<attack_request>();
    request->unmodified_attack_roll = 20;
    request->attack_bonus = 2;
    request->minimum_crit = 18;
    request->armor_class = 15;
    request->crit_confirmation_roll = 15;

    auto abstraction = create_unit_under_test();

    // Act
    auto outcome = abstraction.get_attack_outcome(request);

    // Assert
    EXPECT_EQ(outcome, attack_outcome::hit_no_crit);
}

TEST(AttackAbstractionTest, GetAttackOutcomeReturnsHitNoCritWhenAttackRollIsGreaterThanArmorClass) {
    // Arrange
    auto request = std::make_shared<attack_request>();
    request->unmodified_attack_roll = 18;
    request->attack_bonus = 2;
    request->minimum_crit = 18;
    request->armor_class = 15;
    request->crit_confirmation_roll = 15;

    auto abstraction = create_unit_under_test();
    // Act
    auto outcome = abstraction.get_attack_outcome(request);

    // Assert
    EXPECT_EQ(outcome, attack_outcome::hit_no_crit);
}

TEST(AttackAbstractionTest, GetAttackOutcomeReturnsMissWhenAttackRollIsLessThanArmorClass) {
    // Arrange
    auto request = std::make_shared<attack_request>();
    request->unmodified_attack_roll = 10;
    request->attack_bonus = 2;
    request->minimum_crit = 18;
    request->armor_class = 15;
    request->crit_confirmation_roll = 15;

    auto abstraction = create_unit_under_test();

    // Act
    auto outcome = abstraction.get_attack_outcome(request);

    // Assert
    EXPECT_EQ(outcome, attack_outcome::miss);
}




TEST(AttackAbstractionTest, GetDamageOutcome_HitAndCrit)
{
    // Arrange
    shared_ptr<damage_strategy> dmgStrategy = std::make_shared<statistical_mean_damage_strategy>(2, 6);
    shared_ptr<damage_request> request = std::make_shared<damage_request>(attack_outcome::hit_and_crit, 2, dmgStrategy, 3);
    auto abstraction = create_unit_under_test();
    // Act
    float result = abstraction.get_damage_outcome(request);

    // Assert
    EXPECT_FLOAT_EQ(result, 10.0f);
}

TEST(AttackAbstractionTest, GetDamageOutcome_HitNoCrit)
{
    // Arrange
    shared_ptr<damage_strategy> dmgStrategy = make_shared<roll_dice_damage_strategy>(make_shared<dice_manager>(), 3, 8);
    shared_ptr<damage_request> request = make_shared<damage_request>(attack_outcome::hit_no_crit, 1, dmgStrategy, 2);
    auto abstraction = create_unit_under_test();

    // Act
    float result = abstraction.get_damage_outcome(request);

    // Assert
    EXPECT_FLOAT_EQ(result, 15.0f);
}

TEST(AttackAbstractionTest, GetDamageOutcome_Default)
{
    // Arrange
    shared_ptr<damage_strategy> dmgStrategy = make_shared<statistical_mean_damage_strategy>(1, 4);
    shared_ptr<damage_request> request = make_shared<damage_request>(attack_outcome::miss, 1, dmgStrategy, 0);
    auto abstraction = create_unit_under_test();

    // Act
    float result = abstraction.get_damage_outcome(request);

    // Assert
    EXPECT_FLOAT_EQ(result, 0.0f);
}