// See https://aka.ms/new-console-template for more information
using PathfinderCombatSimulator;
using PathfinderCombatSimulator.ConsoleUI;



IDiceManager _rng = new DiceManager();
IUserInterface _ui = new UserInterface();
ICombatHelper _combat = new CombatHelper(_rng);
IMobAI _ai = new MobAI(_combat);

var theAlgorithm = new CombatAlgorithm(_rng, _ui, _combat, _ai);
List<AlliedCombatGroup> combatGroups = GenerateCombatGroups(_rng);
Dictionary<string, int> winners = InitializeWinners(combatGroups);

for (int i = 0; i < 100; i++)
{
    _ui.Flush();
    _combat.ResetCombatGroups(combatGroups);

    CombatResults? result = theAlgorithm.ExecuteCombat(combatGroups);

    if (_combat.OnlyOneLivingACG(combatGroups))
    {
        string? victorName = _combat.GetWinningAcg(combatGroups)?.AcgName;

        if (!string.IsNullOrWhiteSpace(victorName))
        {
            var val = winners[victorName];
            winners[victorName] = val + 1;
        }
    }
    else
    {
        Console.WriteLine("breakpoint");
    }
}

_ui.OutputAggregates(winners);

static List<AlliedCombatGroup> GenerateCombatGroups(IDiceManager _rng)
{

    var firstCombatGroup = new AlliedCombatGroup("Ug's team");
    //firstCombatGroup.Combatants.Add(new OrcWarrior(_rng, "Ug the Warrior"));
    //firstCombatGroup.Combatants.Add(new OrcWarrior(_rng, "Grubs the Warrior"));

    firstCombatGroup.Combatants.Add(new OrcWarCheif(_rng, "Ug the Warcheif"));

    var secondCombatGroup = new AlliedCombatGroup("Bolg's Team");
    secondCombatGroup.Combatants.Add(new OrcBrute(_rng, "Bolg the brute"));
    secondCombatGroup.Combatants.Add(new OrcBrute(_rng, "Ugthar the brute"));
    secondCombatGroup.Combatants.Add(new OrcBrute(_rng, "Thag the brute"));
    //secondCombatGroup.Combatants.Add(new OrcBrute(_rng, "Carnog the brute"));

    var combatGroups = new List<AlliedCombatGroup>();

    combatGroups.Add(firstCombatGroup);
    combatGroups.Add(secondCombatGroup);

    return combatGroups;
}

static Dictionary<string, int> InitializeWinners(List<AlliedCombatGroup> combatGroups)
{
    Dictionary<string, int> winners = new();
    foreach (AlliedCombatGroup acg in combatGroups)
    {
        winners.Add(acg.AcgName, 0);
    }

    return winners;
}

