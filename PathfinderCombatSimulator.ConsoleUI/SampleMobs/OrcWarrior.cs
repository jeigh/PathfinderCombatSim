using PathfinderCombatSimulator.ConsoleUI;

namespace PathfinderCombatSimulator
{
    public class OrcWarrior : Mob
    {
        private readonly IDiceManager _rng;

        public OrcWarrior(IDiceManager rng, string id)
        {
            _rng = rng;
            Id = id;
            Attack = new OrcNecksplitterAttack(_rng);

            MaxHitPoints = 23;
            CurrentArmorClass = 18;
            CurrentInitiativeModifier = 0;
        }
        

    }
}
