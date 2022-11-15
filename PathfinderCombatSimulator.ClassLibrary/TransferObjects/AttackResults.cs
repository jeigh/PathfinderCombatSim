namespace PathfinderCombatSimulator
{
    public class AttackResults
    {
        public int DamageDelivered { get; set; } = 0;
        public Mob? TargetMob { get; set; }
        public Mob? AttackingMob { get; set; }
    }
}
