using PathfinderCombatSimulator.ConsoleUI;

namespace PathfinderCombatSimulator
{
    public class OrcKnuckleDaggerAttack : IAttack
    {
        private readonly IDiceManager _rng;

        public OrcKnuckleDaggerAttack(IDiceManager rng)
        {
            _rng = rng;
        }

        public int RollDamage() => 
            _rng.Roll(6) + 3;

        public int RollToHit(int numberOfPreviousAttacksThisTurn) => 
            _rng.Roll(20) + 3 - (numberOfPreviousAttacksThisTurn * 5);
    }




}
