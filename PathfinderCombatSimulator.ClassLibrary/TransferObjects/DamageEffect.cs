using PathfinderCombatSimulator.ClassLibrary.Constants;

namespace PathfinderCombatSimulator
{
    public class DamageEffect
    {
        public DamageType DamageType { get; set; } = DamageType.Physical;
        public IEnumerable<DieRoll> DamageDice { get; set; } = new List<DieRoll>();
        public IEnumerable<SavingThrow>? SavingThrow { get; set; } 
        public SavingThrowSuccessEffect SavingThrowSuccessEffect { get; set; }
    }


}
