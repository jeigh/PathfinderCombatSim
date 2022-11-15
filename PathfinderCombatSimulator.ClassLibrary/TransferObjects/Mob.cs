namespace PathfinderCombatSimulator
{
    public class Mob 
    {
        


        public string Id { get; set; }
        public int CurrentHitPoints { get; set; }
        public int MaxHitPoints { get; set; }
        public int CurrentArmorClass { get;  set; }

        public int PerceptionSkillCheckModifier { get; set; }
        public IAttack? Attack { get; set; }
        public int CalculateCurrentInitiativeModifier() => this.PerceptionSkillCheckModifier;

    }
}
