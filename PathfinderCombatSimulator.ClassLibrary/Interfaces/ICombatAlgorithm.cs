namespace PathfinderCombatSimulator
{
    public interface ICombatAlgorithm
    {
        CombatResults ExecuteCombat(List<AlliedCombatGroup> combatGroups);
    }
}
