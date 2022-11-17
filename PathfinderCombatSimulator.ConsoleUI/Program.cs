
using PathfinderCombatSimulator;
using PathfinderCombatSimulator.ConsoleUI;
using PathfinderCombatSimulator.StaticDefinitions.Mobs;

IDiceManager _rng = new DiceManager();
IUserInterface _ui = new UserInterface();
ICombatHelper _combat = new CombatHelper(_rng);
IMobAI _ai = new MobAI(_combat);

var theAlgorithm = new CombatAlgorithm(_rng, _ui, _combat, _ai);
List<CombatTeam> combatGroups = GenerateCombatGroups(_rng);
Dictionary<string, int> winners = InitializeWinners(combatGroups);

for (int i = 0; i < 100; i++)
{
    _ui.Flush();
    _combat.ResetCombatGroups(combatGroups);

    BattleResults? result = theAlgorithm.ProcessBattleUntilOnlyOneTeamIsConcious(combatGroups);

    if (_combat.OnlyOneLivingACG(combatGroups))
    {
        string? victorName = _combat.GetWinningAcg(combatGroups)?.Name;

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

static List<CombatTeam> GenerateCombatGroups(IDiceManager _rng)
{

    var firstCombatGroup = new CombatTeam("Ug's team");
    //firstCombatGroup.Combatants.Add(new OrcWarrior(_rng, "Ug the Warrior"));
    //firstCombatGroup.Combatants.Add(new OrcWarrior(_rng, "Grubs the Warrior"));

    firstCombatGroup.Combatants.Add(new OrcWarCheif(_rng, "Ug the Warcheif"));
    firstCombatGroup.Combatants.Add(new GiantRat(_rng, "rat #2"));

    var secondCombatGroup = new CombatTeam("Bolg's Team");
    secondCombatGroup.Combatants.Add(new OrcBrute(_rng, "Bolg the brute"));
    secondCombatGroup.Combatants.Add(new OrcBrute(_rng, "Ugthar the brute"));
    secondCombatGroup.Combatants.Add(new OrcBrute(_rng, "Thag the brute"));
    secondCombatGroup.Combatants.Add(new GiantRat(_rng, "rat #1"));
    //secondCombatGroup.Combatants.Add(new OrcBrute(_rng, "Carnog the brute"));

    var combatGroups = new List<CombatTeam>();

    combatGroups.Add(firstCombatGroup);
    combatGroups.Add(secondCombatGroup);

    return combatGroups;
}

static Dictionary<string, int> InitializeWinners(List<CombatTeam> combatGroups)
{
    Dictionary<string, int> winners = new();
    foreach (CombatTeam acg in combatGroups)
    {
        winners.Add(acg.Name, 0);
    }

    return winners;
}

