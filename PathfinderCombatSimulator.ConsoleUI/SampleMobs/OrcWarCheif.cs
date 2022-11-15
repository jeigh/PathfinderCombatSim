using PathfinderCombatSimulator.ConsoleUI;

namespace PathfinderCombatSimulator
{
    public class OrcWarCheif : Mob
    {
        public OrcWarCheif(IDiceManager rng, string id)
        {
            Id = id;
            Attack = new OrcGreatClubAttack(rng);

            MaxHitPoints = 32;
            CurrentArmorClass = 19;
            PerceptionSkillCheckModifier = 0;
        }




    }

}
