
using PathfinderCombatSimulator.ClassLibrary.Constants;

namespace PathfinderCombatSimulator.StaticDefinitions.Mobs
{
    public class OrcBrute : Mob
    {
        public OrcBrute(IDiceManager rng, string id) : base(id, 23, 15, 0, new OrcKnuckleDaggerAttack(rng))
        {

        }

        public class OrcKnuckleDaggerAttack : IAttack
        {
            private readonly IDiceManager _rng;

            public OrcKnuckleDaggerAttack(IDiceManager rng)
            {
                _rng = rng;

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

            public int RollToHit(int numberOfPreviousAttacksThisTurn) =>
                _rng.Roll(20) + 3 - (numberOfPreviousAttacksThisTurn * 5);
        }
    }

}
