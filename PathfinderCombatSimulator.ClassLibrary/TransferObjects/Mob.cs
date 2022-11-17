namespace PathfinderCombatSimulator
{
    public class Mob
    {
        public Mob(string id, int maxHitPoints, int currentArmorClass, int perceptionSkillCheckModifier, IAttack? attack)
        {
            Id = id; 
            CurrentHitPoints = maxHitPoints;
            MaxHitPoints = maxHitPoints;
            CurrentArmorClass= currentArmorClass;
            PerceptionSkillCheckModifier = perceptionSkillCheckModifier;
            DefaultAttack = attack;
        }

        public string Id { get; set; }
        public int CurrentHitPoints { get; set; }
        public int MaxHitPoints { get; set; }
        public int CurrentArmorClass { get; set; }
        public int PerceptionSkillCheckModifier { get; set; }
        public IAttack? DefaultAttack { get; set; }
    }
}
