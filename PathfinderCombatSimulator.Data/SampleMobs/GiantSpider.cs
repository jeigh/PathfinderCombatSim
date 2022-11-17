
using PathfinderCombatSimulator.ClassLibrary.Constants;

namespace PathfinderCombatSimulator.StaticDefinitions.Mobs
{
    public class GiantSpider : Mob
    {
        public GiantSpider(IDiceManager rng, string id) : base(id, 16, 17, 7, new FangsAttack())
        {
        }


        public class FangsAttack : IAttack 
        {

            public IAttack? DefaultAttack { get; set; }

            public FangsAttack()
            {
                List<DieRoll> theDamageDice = new List<DieRoll>
                {
                    new DieRoll()
                    {
                        DieCount = 1,
                        DieSize = 6,
                        ModiferAfterAddition = 2
                    }
                };

                DamageEffects.Add(new DamageEffect { DamageDice = theDamageDice, DamageType = DamageType.Physical });

            }

            public IList<DamageEffect> DamageEffects { get; set; } = new List<DamageEffect>();

            public int AttackModifier { get; set; } = 9;

        }
    }
}
