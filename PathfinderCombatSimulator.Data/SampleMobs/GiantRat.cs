using PathfinderCombatSimulator.Data.Attacks;

namespace PathfinderCombatSimulator.Data.Mobs
{
    public class GiantRat : Mob
    {
        public GiantRat(IDiceManager rng, string id) : base(id, 8, 15, 5, new GiantRatBiteAttack(rng))
        {
        }
    }

}
