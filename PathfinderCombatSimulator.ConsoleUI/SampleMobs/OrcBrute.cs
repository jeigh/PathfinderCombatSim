using PathfinderCombatSimulator.ConsoleUI;

namespace PathfinderCombatSimulator
{
    public class OrcBrute : Mob
    {
        private readonly IDiceManager _rng;

        public OrcBrute(IDiceManager rng, string id)
        {
            _rng = rng;
            Id = id;
            Attack = new OrcKnuckleDaggerAttack(_rng);

            MaxHitPoints = 23;
            CurrentArmorClass = 15;
            PerceptionSkillCheckModifier = 0;
        }

        

    }

}
