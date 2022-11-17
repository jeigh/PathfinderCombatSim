
using PathfinderCombatSimulator.ClassLibrary.Constants;

namespace PathfinderCombatSimulator.StaticDefinitions.Mobs
{
    public class OrcBrute : Mob
    {
        public OrcBrute(IDiceManager rng, string id) : base(id, 23, 15, 0, new OrcKnuckleDaggerAttack())
        {

        }

        public class OrcKnuckleDaggerAttack : IAttack
        {

            public OrcKnuckleDaggerAttack()
            {
                List<DieRoll> theDamageDice = new List<DieRoll>
                {
                    new DieRoll()
                    {
                        DieCount = 1,
                        DieSize = 6,
                        ModiferAfterAddition = 3
                    }
                };

                DamageEffects.Add(new DamageEffect { DamageDice = theDamageDice, DamageType = DamageType.Physical });

            }

            public IList<DamageEffect> DamageEffects { get; set; } = new List<DamageEffect>();

            public int AttackModifier { get; set; } = 3;

        }
    }

}
