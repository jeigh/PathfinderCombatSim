namespace PathfinderCombatSimulator.Data.Attacks;

public class GiantRatBiteAttack : IAttack
{
    private readonly IDiceManager _rng;

    public GiantRatBiteAttack(IDiceManager rng)
    {
        _rng = rng;
    }

    public int RollDamage() =>
        _rng.Roll(6) + 1;

    public int RollToHit(int numberOfPreviousAttacksThisTurn) => 
        _rng.Roll(20) + 7 - (numberOfPreviousAttacksThisTurn * 5);
}
