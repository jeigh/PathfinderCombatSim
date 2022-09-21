namespace PathfinderCombatSimulator
{
    public interface IMobAI
    {
        Mob? GetTargetFor(Mob item, List<AlliedCombatGroup> acgs);
    }
}