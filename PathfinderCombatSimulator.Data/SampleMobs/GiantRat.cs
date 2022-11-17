
using PathfinderCombatSimulator.ClassLibrary.Constants;

namespace PathfinderCombatSimulator.StaticDefinitions.Mobs
{





    public class GiantRat : Mob
    {
        public GiantRat(IDiceManager rng, string id) : base(id, 8, 15, 5, new GiantRatBiteAttack())
        {
        }

        public class GiantRatBiteAttack : IAttack
        {
            public GiantRatBiteAttack()
            {
                List<DieRoll> theDamageDice = new List<DieRoll>
                {
                    new DieRoll()
                    {
                        DieCount = 1,
                        DieSize = 6,
                        ModiferAfterAddition = 1
                    }
                };

                DamageEffects.Add(new DamageEffect { DamageDice = theDamageDice, DamageType = DamageType.Physical });
            }

            public IList<DamageEffect> DamageEffects { get; set; } = new List<DamageEffect>();


            public int AttackModifier { get; set; } = 7;


        }
    }

}
