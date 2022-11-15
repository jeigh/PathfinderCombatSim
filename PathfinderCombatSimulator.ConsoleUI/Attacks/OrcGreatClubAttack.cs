using PathfinderCombatSimulator.ConsoleUI;

namespace PathfinderCombatSimulator
{

    public class OrcGreatClubAttack : IAttack
    {
        private readonly IDiceManager _rng;

        public OrcGreatClubAttack(IDiceManager rng)
        {
            _rng = rng;
        }

        public int RollDamage() => 
            _rng.Roll(10) + 4;        

        public int RollToHit() => 
            _rng.Roll(20) + 10;
        


    }




}
