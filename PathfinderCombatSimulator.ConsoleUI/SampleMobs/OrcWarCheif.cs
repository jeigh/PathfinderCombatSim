using PathfinderCombatSimulator.ConsoleUI;

namespace PathfinderCombatSimulator
{
    public class OrcWarCheif : Mob
    {
        private readonly IDiceManager _rng;

        public OrcWarCheif(IDiceManager rng, string id)
        {
            _rng = rng;
            Id = id;
            Attack = new OrcGreatClubAttack(_rng);

            MaxHitPoints = 32;
            CurrentArmorClass = 19;
            CurrentInitiativeModifier = 0;
        }



    }

}
