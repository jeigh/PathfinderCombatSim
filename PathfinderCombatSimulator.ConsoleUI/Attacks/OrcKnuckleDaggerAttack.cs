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

        public int RollDamage()
        {
            return _rng.Roll(6) + 3;
        }

        public int RollToHit()
        {
            return _rng.Roll(20) + 3;
        }
    }




}
