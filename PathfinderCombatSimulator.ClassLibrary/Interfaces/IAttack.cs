namespace PathfinderCombatSimulator
{
    public interface IAttack
    {
        int RollDamage();
        int RollToHit(int numberOfPreviousAttacksThisTurn);
    }
}
