namespace PathfinderCombatSimulator.Data.Attacks
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

        public int RollToHit(int numberOfPreviousAttacksThisTurn) => 
            _rng.Roll(20) + 10 - (numberOfPreviousAttacksThisTurn * 5);
        


    }




}
