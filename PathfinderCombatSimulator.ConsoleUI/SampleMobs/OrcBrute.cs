using PathfinderCombatSimulator.ConsoleUI;

namespace PathfinderCombatSimulator
{
    public class OrcBrute : Mob
    {
        public OrcBrute(IDiceManager rng, string id) : base(id, 23, 15, 0, new OrcKnuckleDaggerAttack(rng))
        {

        }
    }

}
