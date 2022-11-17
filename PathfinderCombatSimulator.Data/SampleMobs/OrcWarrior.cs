
using PathfinderCombatSimulator.ClassLibrary.Constants;

namespace PathfinderCombatSimulator.StaticDefinitions.Mobs
{
    public class OrcWarrior : Mob
    {
        public OrcWarrior(IDiceManager rng, string id) : base(id, 23, 18, 0,  new OrcNecksplitterAttack(rng))
        {

        }

        public class OrcNecksplitterAttack : IAttack
        {
            private readonly IDiceManager _rng;

            public OrcNecksplitterAttack(IDiceManager rng)
            {
                _rng = rng;

                List<DieRoll> theDamageDice = new List<DieRoll>
                {
                    new DieRoll()
                    {
                        DieCount = 1,
                        DieSize = 8,
                        ModiferAfterAddition = 4
                    }
                };

                _damageEffect = new DamageEffect { DamageDice = theDamageDice, DamageType = DamageType.Physical };

            }

            private readonly DamageEffect _damageEffect;


            public Dictionary<DamageType, int> RollDamage()
            {
                var returnable = new Dictionary<DamageType, int>();
                returnable[_damageEffect.DamageType] = _rng.AddRolls(_damageEffect.DamageDice);                
                return returnable;

            }
                

            public int RollToHit(int numberOfPreviousAttacksThisTurn) =>
                _rng.Roll(20) + 7 - (numberOfPreviousAttacksThisTurn * 5);
        }
    }
}
