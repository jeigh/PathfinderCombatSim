namespace PathfinderCombatSimulator
{
    public class CombatTeam
    {
        public CombatTeam(string acgName)
        {
            Name = acgName;
        }

        public string Name { get; set; }

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
