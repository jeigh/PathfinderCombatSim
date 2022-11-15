namespace PathfinderCombatSimulator
{
    public class GiantRat : Mob
    {
        public GiantRat(IDiceManager rng, string id)
        {
            Id = id;
            Attack = new GiantRatBiteAttack(rng);

            MaxHitPoints = 8;
            CurrentArmorClass = 15;
            PerceptionSkillCheckModifier = 5;
        }
    }

}
