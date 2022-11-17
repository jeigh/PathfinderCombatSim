
using PathfinderCombatSimulator.ClassLibrary.Constants;

namespace PathfinderCombatSimulator.StaticDefinitions.Mobs
{
    public class OrcWarCheif : Mob
    {
        public OrcWarCheif(IDiceManager rng, string id) : base(id, 32, 19, 0, new OrcGreatClubAttack())
        {

        }

        public class OrcGreatClubAttack : IAttack
        {


            public OrcGreatClubAttack()
            {
                

                List<DieRoll> theDamageDice = new List<DieRoll>
                {
                    new DieRoll()
                    {
                        DieCount = 1,
                        DieSize = 10,
                        ModiferAfterAddition = 4
                    }
                };

                DamageEffects.Add(new DamageEffect { DamageDice = theDamageDice, DamageType = DamageType.Physical });

            }

            public IList<DamageEffect> DamageEffects { get; set; } = new List<DamageEffect>();
            public int AttackModifier { get; set; } = 10;




        }




    }

}
