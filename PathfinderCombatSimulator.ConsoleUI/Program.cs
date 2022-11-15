﻿// See https://aka.ms/new-console-template for more information
using PathfinderCombatSimulator;
using PathfinderCombatSimulator.ConsoleUI;



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

    CombatResults? result = theAlgorithm.ExecuteCombat(combatGroups);

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

    var secondCombatGroup = new CombatTeam("Bolg's Team");
    secondCombatGroup.Combatants.Add(new OrcBrute(_rng, "Bolg the brute"));
    secondCombatGroup.Combatants.Add(new OrcBrute(_rng, "Ugthar the brute"));
    secondCombatGroup.Combatants.Add(new OrcBrute(_rng, "Thag the brute"));
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
