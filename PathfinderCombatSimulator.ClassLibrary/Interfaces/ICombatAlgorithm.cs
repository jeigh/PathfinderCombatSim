namespace PathfinderCombatSimulator
{
    public interface ICombatAlgorithm
    {
        CombatResults ExecuteCombat(List<CombatTeam> combatGroups);
    }
}
