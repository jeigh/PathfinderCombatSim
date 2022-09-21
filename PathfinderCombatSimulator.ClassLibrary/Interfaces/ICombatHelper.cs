namespace PathfinderCombatSimulator
{
    public interface ICombatHelper
    {

        AlliedCombatGroup? GetWinningAcg(List<AlliedCombatGroup> combatGroups);
        
        bool IsDead(Mob theMob);
        
        bool IsUnconcious(Mob theMob);
        
        bool OnlyOneLivingACG(List<AlliedCombatGroup> combatGroups);

        void ResetCombatGroups(List<AlliedCombatGroup> combatGroups);

        void ResetMob(Mob combatant);
    }
}
