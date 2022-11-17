
using PathfinderCombatSimulator.ClassLibrary.Constants;

namespace PathfinderCombatSimulator.StaticDefinitions.Mobs
{





    public class GiantRat : Mob
    {
        public GiantRat(IDiceManager rng, string id) : base(id, 8, 15, 5, new GiantRatBiteAttack(rng))
        {
        }

        public class GiantRatBiteAttack : IAttack
        {
            private readonly IDiceManager _rng;


            public GiantRatBiteAttack(IDiceManager rng)
            {
                _rng = rng;

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




            public int RollToHit(int numberOfPreviousAttacksThisTurn) =>
                _rng.Roll(20) + 7 - (numberOfPreviousAttacksThisTurn * 5);
        }
    }

}
