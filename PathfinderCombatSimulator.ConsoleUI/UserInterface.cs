namespace PathfinderCombatSimulator.ConsoleUI
{
    public class UserInterface : IUserInterface
    {
        public void Die(Mob targetMob)
        {
            //Console.WriteLine($"{targetMob.Id} is dead.");
        }

        public void Flush()
        {
            //Console.Clear();
        }

        public void KnockOut(Mob targetMob)
        {
            //Console.WriteLine($"{targetMob.Id} is unconcious.");
        }

        public void ProcessAttack(Mob mob, int damageRoll, Mob victim)
        {
            //Console.WriteLine($"{mob.Id} attacks {victim.Id} dealing {damageRoll} damage.");
            
        }

        public void ReceiveDamage(AttackResults results)
        {
            //Console.WriteLine($"{results.AttackingMob.Id} damages {results.TargetMob.Id} for {results.DamageDelivered}.");

        }

        public void RoundEnds(int turnId)
        {
            //Console.ReadKey();
            //Console.WriteLine($"Turn {turnId} ends.");
        }

        public void RoundStarts(int turnId)
        {
            //Console.WriteLine($"Turn {turnId} begins.");
        }

        public void OutputAggregates(Dictionary<string, int> winners)
        {
            foreach (KeyValuePair<string, int> kvp in winners)
            {
                Console.WriteLine($"{kvp.Key} Wins : {kvp.Value}");
            }
        }

        public void AttackMisses(Mob attacker, Mob targetMob)
        {
            //Console.WriteLine($"{attacker.Id} swings at {targetMob.Id} but misses.");
        }
    }





}
