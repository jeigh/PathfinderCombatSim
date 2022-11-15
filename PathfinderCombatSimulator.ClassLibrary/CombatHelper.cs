namespace PathfinderCombatSimulator
{
    public class CombatHelper : ICombatHelper
    {
        private IDiceManager _rng;

        public CombatHelper(IDiceManager rng)
        {
            _rng = rng;
        }

        public bool IsDead(Mob theMob) =>
            theMob.CurrentHitPoints <= -10;

        public bool IsUnconcious(Mob theMob) =>
            (theMob.CurrentHitPoints > -10 && theMob.CurrentHitPoints <= 0);

        public bool OnlyOneLivingACG(List<CombatTeam> combatGroups)
        {
            IEnumerable<CombatTeam> enumerable()
            {
                foreach (var cg in combatGroups)
                {
                    if (CombatGroupStillAlive(cg))
                    {
                        yield return cg;
                    }
                }
            }

            return enumerable().Count() == 1;
        }

        private bool CombatGroupStillAlive(CombatTeam combatGroup)
        {
            bool alive = false;
            foreach (Mob combatant in combatGroup.Combatants)
            {
                if (!IsDead(combatant)) alive = true;
            }
            return alive;
        }

        public void ResetCombatGroups(List<CombatTeam> combatGroups)
        {
            foreach(CombatTeam combatGroup in combatGroups)
            {
                foreach(Mob combatant in combatGroup.Combatants)
                {
                    ResetMob(combatant);
                }
            }
        }

        public void ResetMob(Mob combatant) =>
            combatant.CurrentHitPoints = combatant.MaxHitPoints;

        public CombatTeam? GetWinningAcg(List<CombatTeam> combatGroups)
        {
            foreach(CombatTeam acg in combatGroups)
            {
                if (CombatGroupStillAlive(acg)) return acg;
            }
            return null;
        }
    }


}
