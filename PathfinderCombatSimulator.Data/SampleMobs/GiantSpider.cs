
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
                _rng.Roll(20) + 9;
        }
    }
}
