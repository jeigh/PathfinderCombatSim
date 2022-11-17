namespace PathfinderCombatSimulator
{
    public interface IUserInterface
    {
        void Die(Mob targetMob);
        void Flush();
        void KnockOut(Mob targetMob);
        void ProcessAttack(Mob mob, int damageRoll, Mob victim);
        void ReceiveDamage(AttackResults results);
        void RoundEnds(int turnId);
        void RoundStarts(int turnId);
        void OutputAggregates(Dictionary<string, int> winners);
        void AttackMisses(Mob mob, Mob targetMob);
    }
}
