using PathfinderCombatSimulator.ClassLibrary.Constants;

namespace PathfinderCombatSimulator
{
    public interface IAttack
    {
        //Dictionary<DamageType, int> RollDamage(IList<DamageEffect> damageEffects);
        int RollToHit(int numberOfPreviousAttacksThisTurn);
        IList<DamageEffect> DamageEffects { get; set; }

    }
}
