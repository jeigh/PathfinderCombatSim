using PathfinderCombatSimulator.ConsoleUI;

namespace PathfinderCombatSimulator
{
    public class OrcWarrior : Mob
    {
        public OrcWarrior(IDiceManager rng, string id) : base(id, 23, 18, 0,  new OrcNecksplitterAttack(rng))
        {

        }
        

    }
}
