
using PathfinderCombatSimulator.ClassLibrary.Constants;

namespace PathfinderCombatSimulator.StaticDefinitions.Mobs
{
    public class GiantSpider : Mob
    {
        public GiantSpider(IDiceManager rng, string id) : base(id, 16, 17, 7, new FangsAttack(rng))
        {
        }


        public class FangsAttack : IAttack 
        {
            private readonly IDiceManager _rng;
            public IAttack? DefaultAttack { get; set; }

            public FangsAttack(IDiceManager rng)
            {
                _rng = rng;
                
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

            public int RollToHit(int numberOfPreviousAttacksThisTurn) =>
                _rng.Roll(20) + 9;
        }
    }
}
