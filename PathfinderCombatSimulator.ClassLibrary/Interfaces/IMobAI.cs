namespace PathfinderCombatSimulator
{
    public interface IMobAI
    {
        Mob? GetTargetFor(Mob item, List<CombatTeam> acgs);

        int MobMeleePreferenceWeight(Mob mob);

        int MobStealthPreferenceWeight(Mob mob);

        int MobRangedPreferenceWeight(Mob mob);

        int MobSpellPreferenceWeight(Mob mob);
        
        bool MobIsPacifist(Mob mob);

        int MobBravery(Mob mob);

    }
}