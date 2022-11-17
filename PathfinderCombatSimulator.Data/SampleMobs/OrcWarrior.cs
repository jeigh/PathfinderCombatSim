
using PathfinderCombatSimulator.ClassLibrary.Constants;

namespace PathfinderCombatSimulator.StaticDefinitions.Mobs
{
    public class OrcWarrior : Mob
    {
        public OrcWarrior(IDiceManager rng, string id) : base(id, 23, 18, 0,  new OrcNecksplitterAttack())
        {

        }

        public class OrcNecksplitterAttack : IAttack
        {


            public OrcNecksplitterAttack()
            {

                List<DieRoll> theDamageDice = new List<DieRoll>
                {
                    new DieRoll()
                    {
                        DieCount = 1,
                        DieSize = 8,
                        ModiferAfterAddition = 4
                    }
                };

                DamageEffects.Add(new DamageEffect { DamageDice = theDamageDice, DamageType = DamageType.Physical });

            }

            public IList<DamageEffect> DamageEffects { get; set; } = new List<DamageEffect>();

            public int AttackModifier { get; set; } = 7;
        }
    }
}
