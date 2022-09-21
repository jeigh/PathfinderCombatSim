namespace PathfinderCombatSimulator
{
    public class MobAI : IMobAI
    {

        private readonly ICombatHelper _combat;

        public MobAI(ICombatHelper combat)
        {
            _combat = combat;
        }

        public Mob? GetTargetFor(Mob item, List<AlliedCombatGroup> acgs)
        {
            foreach (AlliedCombatGroup combatGroup in acgs)
            {
                if (combatGroup.Contains(item)) continue;
                else
                {
                    foreach (Mob mob in combatGroup.Combatants)
                    {
                        if (!_combat.IsDead(mob)) return mob;
                    }
                }
            }
            return null;
        }

    }
}
