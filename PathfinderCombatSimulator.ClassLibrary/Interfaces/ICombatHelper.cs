namespace PathfinderCombatSimulator
{
    public interface ICombatHelper
    {

        CombatTeam? GetWinningAcg(List<CombatTeam> combatGroups);
        
        bool IsDead(Mob theMob);
        
        bool IsUnconcious(Mob theMob);
        
        bool OnlyOneLivingACG(List<CombatTeam> combatGroups);

        void ResetCombatGroups(List<CombatTeam> combatGroups);

        void ResetMob(Mob combatant);
    }
}
