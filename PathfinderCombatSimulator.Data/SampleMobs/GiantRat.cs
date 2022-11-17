﻿
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

                _damageEffects.Add(new DamageEffect { DamageDice = theDamageDice, DamageType = DamageType.Physical });

            }

            private readonly IList<DamageEffect> _damageEffects = new List<DamageEffect>();

            public Dictionary<DamageType, int> RollDamage()
            {
                var _damageEffect = _damageEffects.First();

                var returnable = new Dictionary<DamageType, int>();
                returnable[_damageEffect.DamageType] = _rng.AddRolls(_damageEffect.DamageDice);
                return returnable;
            }


            public int RollToHit(int numberOfPreviousAttacksThisTurn) =>
                _rng.Roll(20) + 7 - (numberOfPreviousAttacksThisTurn * 5);
        }
    }

}
