#pragma once

namespace PathfinderCombatSimulator
{
	class DieRoll
	{
	public:
		DieRoll(int dieCount, int dieSize, int modifierAfterAddition) : DieSize(dieSize), DieCount(dieCount), ModiferAfterAddition(modifierAfterAddition) {}
		DieRoll(int dieCount, int dieSize) : DieSize(dieSize), DieCount(dieCount) {}
		DieRoll(int dieSize) : DieSize(dieSize) {}
		DieRoll() = default;

		int DieSize = 0;
		int DieCount = 1;
		int ModiferAfterAddition = 0;
	};
}
	