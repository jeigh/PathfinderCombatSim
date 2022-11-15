namespace PathfinderCombatSimulator
{
    public class MobAI : IMobAI
    {

        private readonly ICombatHelper _combat;

        public MobAI(ICombatHelper combat)
        {
            _combat = combat;
        }

        public Mob? GetTargetFor(Mob item, List<CombatTeam> acgs)
        {
            foreach (CombatTeam combatGroup in acgs)
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

        public int MobBravery(Mob mob)
        {
            throw new NotImplementedException();
        }

        public bool MobIsPacifist(Mob mob)
        {
            throw new NotImplementedException();
        }

        public int MobMeleePreferenceWeight(Mob mob)
        {
            throw new NotImplementedException();
        }

        public int MobRangedPreferenceWeight(Mob mob)
        {
            throw new NotImplementedException();
        }

        public int MobSpellPreferenceWeight(Mob mob)
        {
            throw new NotImplementedException();
        }

        public int MobStealthPreferenceWeight(Mob mob)
        {
            throw new NotImplementedException();
        }
    }
}
