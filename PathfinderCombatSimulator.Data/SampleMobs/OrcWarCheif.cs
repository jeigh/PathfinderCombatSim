
using PathfinderCombatSimulator.ClassLibrary.Constants;

namespace PathfinderCombatSimulator.StaticDefinitions.Mobs
{
    public class OrcWarCheif : Mob
    {
        public OrcWarCheif(IDiceManager rng, string id) : base(id, 32, 19, 0, new OrcGreatClubAttack(rng))
        {

        }

        public class OrcGreatClubAttack : IAttack
        {
            private readonly IDiceManager _rng;

            public OrcGreatClubAttack(IDiceManager rng)
            {
                _rng = rng;

                List<DieRoll> theDamageDice = new List<DieRoll>
                {
                    new DieRoll()
                    {
                        DieCount = 1,
                        DieSize = 10,
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
                _rng.Roll(20) + 10 - (numberOfPreviousAttacksThisTurn * 5);



        }




    }

}
