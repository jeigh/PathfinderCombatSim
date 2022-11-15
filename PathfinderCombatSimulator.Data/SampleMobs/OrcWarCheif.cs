using PathfinderCombatSimulator.Data.Attacks;

namespace PathfinderCombatSimulator.Data.Mobs
{
    public class OrcWarCheif : Mob
    {
        public OrcWarCheif(IDiceManager rng, string id) : base(id, 32, 19, 0, new OrcGreatClubAttack(rng))
        {

        }




    }

}
