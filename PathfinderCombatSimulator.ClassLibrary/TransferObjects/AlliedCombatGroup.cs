namespace PathfinderCombatSimulator
{
    public class AlliedCombatGroup
    {
        public AlliedCombatGroup(string acgName)
        {
            AcgName = acgName;
        }

        public string AcgName { get; set; }

        public List<Mob> Combatants { get; set; } = new List<Mob>();

        public bool Contains(Mob contained)
        {
            foreach(Mob mob in Combatants)
            {
                if (mob == contained) return true;
            }
            return false;
        }
            
    }
}
