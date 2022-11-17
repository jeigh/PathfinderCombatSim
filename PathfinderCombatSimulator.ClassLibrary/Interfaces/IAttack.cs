using PathfinderCombatSimulator.ClassLibrary.Constants;

namespace PathfinderCombatSimulator
{
    public interface IAttack
    {
        Dictionary<DamageType, int> RollDamage();
        int RollToHit(int numberOfPreviousAttacksThisTurn);
    }
}
