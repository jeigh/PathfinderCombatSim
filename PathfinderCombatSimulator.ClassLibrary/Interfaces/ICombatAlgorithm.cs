namespace PathfinderCombatSimulator
{
    public interface ICombatAlgorithm
    {
        BattleResults ProcessBattleUntilOnlyOneTeamIsConcious(List<CombatTeam> combatGroups);
    }
}
