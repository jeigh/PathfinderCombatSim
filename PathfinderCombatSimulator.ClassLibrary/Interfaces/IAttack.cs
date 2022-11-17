using PathfinderCombatSimulator.ClassLibrary.Constants;

namespace PathfinderCombatSimulator
{
    public interface IAttack
    {
        //int RollToHit(int numberOfPreviousAttacksThisTurn);
        IList<DamageEffect> DamageEffects { get; set; }
        int AttackModifier { get; set; }

    }
}
